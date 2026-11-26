// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_MESH_H
#define VCL_MESH_MESH_H

#include "containers.h"
#include "elements.h"
#include "mesh_components.h"
#include "views.h"

#include <vclib/algorithms/core.h>
#include <vclib/base.h>

#include <algorithm>
#include <array>
#include <concepts>
#include <cstring>
#include <istream>
#include <ostream>
#include <type_traits>
#include <utility>

namespace vcl {

/**
 * @defgroup mesh Mesh
 *
 * @brief List of classes, aliases, concepts and functions used for the
 * creation, customization, usage and manipulation of Mesh classes.
 */

/**
 * @brief The Mesh class represents a generic mesh composed of element
 * containers (VertexContainer, FaceContainer, etc.) and mesh-level components.
 *
 * The Mesh class will expose all the public members of its containers and its
 * components, and its role is to implement all the functionalities that allow
 * these containers to communicate (e.g. an operation on the vertex container
 * that requires to update also some face information).
 *
 * Therefore, in this page are documented only the functions that in some way
 * need to modify the status of more than one Container of the Mesh. Other
 * functions are inherited by the Container classes of the Elements of the Mesh,
 * or from its Components, and depend on all the templates that compose a
 * specific Mesh definition.
 *
 * @tparam Args: Container types (e.g. VertexContainer<...>, FaceContainer<...>)
 * and mesh-level components.
 *
 * @ingroup mesh
 */
template<typename... Args>
class Mesh : public mesh::detail::RebindContainer<Args, Mesh<Args...>>...
{
    template<typename T>
    using Rebound = mesh::detail::RebindContainer<T, Mesh<Args...>>;

    static constexpr uint ELEM_COUNT = toUnderlying(ElemId::ELEMENT_COUNT);

    static_assert(
        mesh::detail::FindContainer<ElemId::VERTEX, Rebound<Args>...>::found,
        "A Mesh must have a VertexContainer.");

    using VertexContainerType = typename mesh::detail::
        FindContainer<ElemId::VERTEX, Rebound<Args>...>::type;

    template<typename T, typename ExpectedIndex>
    static constexpr bool ContainerIndexMatches = []() {
        if constexpr (mesh::ElementContainerConcept<T>) {
            return std::is_same_v<typename T::IndexType, ExpectedIndex>;
        }
        else {
            return true;
        }
    }();

    template<typename T>
    struct IsMeshComponentPred
    {
        static constexpr bool value =
            !mesh::ElementContainerConcept<T> && comp::ComponentConcept<T>;
    };

    template<typename T>
    struct IsElementContainerPred
    {
        static constexpr bool value = mesh::ElementContainerConcept<T>;
    };

public:
    using IndexType = typename VertexContainerType::IndexType;

    /**
     * @brief The null/invalid sentinel value for this Mesh's index type.
     */
    static constexpr IndexType NULL_INDEX = vcl::NULL_INDEX<IndexType>;

    static_assert(
        (ContainerIndexMatches<Rebound<Args>, IndexType> && ...),
        "All element containers in a Mesh must share the exact same "
        "IndexType.");

    /**
     * @brief Components is an alias to a vcl::TypeWrapper that wraps all the
     * types from which the Mesh inherits (Args) that are Components (they
     * satisfy the ComponentConcept).
     */
    using Components = typename FilterTypesByCondition<
        IsMeshComponentPred,
        TypeWrapper<Rebound<Args>...>>::type;

    /**
     * @brief ElementContainers is an alias to a vcl::TypeWrapper that wraps
     * all the element container types of the Mesh (i.e. all the types from
     * which the Mesh inherits that satisfy the ElementContainerConcept, such
     * as VertexContainer, FaceContainer, EdgeContainer, …).
     *
     * This allows algorithms to iterate at compile-time over every element
     * container present in the mesh without having to enumerate specific
     * ELEM_IDs by hand:
     *
     * @code{.cpp}
     * // Call f<ContainerType>() for every element container in the mesh:
     * [&]<typename... Cont>(TypeWrapper<Cont...>) {
     *     (f.template operator()<Cont>(), ...);
     * }(typename MeshType::ElementContainers{});
     * @endcode
     */
    using ElementContainers = typename FilterTypesByCondition<
        IsElementContainerPred,
        TypeWrapper<Rebound<Args>...>>::type;

    /**
     * @brief Type alias for the container of the given ELEM_ID.
     */
    template<ElemId ELEM_ID>
    using ContainerType =
        typename mesh::detail::FindContainer<ELEM_ID, Rebound<Args>...>::type;

    /**
     * @brief Type alias for the element type of the container of the given
     * ELEM_ID.
     */
    template<ElemId ELEM_ID>
    using ElementType = typename ContainerType<ELEM_ID>::ElementType;

    /**
     * @brief Type alias for the const element type of the container of the
     * given ELEM_ID.
     */
    template<ElemId ELEM_ID>
    using ConstElementType = typename ContainerType<ELEM_ID>::ConstElementType;

    /* Constexpr static member functions */

    /**
     * @brief Returns true if this Mesh has a container of elements having the
     * same Element ID of the template ELEM_ID.
     *
     * Example of usage (Note: EdgeMesh has Vertices, but not Faces):
     *
     * @code{.cpp}
     * static_assert(vcl::EdgeMesh::hasContainerOf<vcl::ElemId::VERTEX>(),
     *                    "EdgeMesh does not have Vertices");
     * static_assert(!vcl::EdgeMesh::hasContainerOf<vcl::ElemId::FACE>(),
     *                    "EdgeMesh has Faces");
     * @endcode
     *
     * @tparam ELEM_ID: the Element ID to check.
     * @return true if this Mesh has a container of elements having the same
     * Element ID of the template ELEM_ID.
     */
    template<ElemId ELEM_ID>
    static constexpr bool hasContainerOf()
    {
        return mesh::detail::FindContainer<ELEM_ID, Rebound<Args>...>::found;
    }

    /**
     * @brief Returns true if this Mesh has a container of elements having the
     * same Element ID of the template ELEM_ID and that container has a
     * Component having the same Component ID of the template COMP_ID.
     *
     * Example of usage (Note: TriMesh has per Vertex TexCoords, but not adj
     * edges):
     *
     * @code{.cpp}
     * static_assert(vcl::TriMesh::hasElementComponent<
     *         vcl::ElemId::VERTEX, vcl::CompId::TEX_COORD>(),
     *         "TriMesh does not have per Vertex TexCoords");
     *
     * static_assert(!vcl::TriMesh::hasElementComponent<
     *        vcl::ElemId::VERTEX, vcl::CompId::ADJACENT_EDGES>(),
     *        "TriMesh has per Vertex Adjacent Edges");
     * @endcode
     *
     * @tparam ELEM_ID: the Element ID to check.
     * @tparam COMP_ID: the Component ID to check.
     * @return `true` if this Mesh has a container of elements having ID ELEM_ID
     * and that container has a component having ID COMP_ID, `false` otherwise.
     */
    template<ElemId ELEM_ID, CompId COMP_ID>
    static constexpr bool hasElementComponent()
    {
        if constexpr (hasContainerOf<ELEM_ID>()) {
            return comp::HasComponent<COMP_ID, ContainerType<ELEM_ID>>;
        }
        else {
            return false;
        }
    }

    /**
     * @brief Returns true if this Mesh has a container of elements having the
     * same Element ID of the template ELEM_ID and that container has an
     * Optional Component having the same Component ID of the template COMP_ID.
     *
     * Example of usage (Note: TriMesh has per Vertex optional TexCoords, and
     * non-optional Normals):
     *
     * @code{.cpp}
     * static_assert(vcl::TriMesh::hasElementOptionalComponent<
     *         vcl::ElemId::VERTEX, vcl::CompId::TEX_COORD>(),
     *         "TriMesh does not have per Vertex Optional TexCoords");
     *
     * static_assert(!vcl::TriMesh::hasElementOptionalComponent<
     *        vcl::ElemId::VERTEX, vcl::CompId::NORMAL>(),
     *        "TriMesh has per Vertex Optional Normals");
     * @endcode
     *
     * @tparam ELEM_ID: the Element ID to check.
     * @tparam COMP_ID: the Component ID to check.
     * @return `true` if this Mesh has a container of elements having ID ELEM_ID
     * and that container has an optional component having ID COMP_ID, `false`
     * otherwise.
     */
    template<ElemId ELEM_ID, CompId COMP_ID>
    static constexpr bool hasElementOptionalComponent()
    {
        if constexpr (hasContainerOf<ELEM_ID>()) {
            return comp::HasOptionalComponent<COMP_ID, ContainerType<ELEM_ID>>;
        }
        else {
            return false;
        }
    }

    /* Constructors */

    /**
     * @brief Empty constructor, constructs an empty mesh.
     */
    Mesh() { setParentMeshPointersToContainers(); }

    /**
     * @brief Copy constructor: deep copies containers and components.
     *
     * @param other: the mesh from which to construct this Mesh.
     */
    Mesh(const Mesh& other) :
            Rebound<Args>(static_cast<const Rebound<Args>&>(other))...
    {
        setParentMeshPointersToContainers();
    }

    /**
     * @brief Move constructor, moves the given mesh into this one, without any
     * other resource acquisition.
     *
     * @param other: the mesh that will be moved into this Mesh.
     */
    Mesh(Mesh&& other) noexcept :
            Rebound<Args>(static_cast<Rebound<Args>&&>(other))...
    {
        setParentMeshPointersToContainers();
        other.setParentMeshPointersToContainers();
    }

    /**
     * @brief Assignment operator of the Mesh using the Copy & Swap idiom.
     * Handles both copy assignment and move assignment seamlessly and provides
     * the strong exception safety guarantee.
     *
     * @param other: The mesh from which to copy or move data.
     * @return Reference to this Mesh after assignment.
     */
    Mesh& operator=(Mesh other) noexcept
    {
        swap(other);
        return *this;
    }

    /* Member functions */

    /**
     * @brief Swaps the content of this Mesh with another.
     */
    void swap(Mesh& other) noexcept
    {
        using std::swap;
        (swap(
             static_cast<Rebound<Args>&>(*this),
             static_cast<Rebound<Args>&>(other)),
         ...);
        setParentMeshPointersToContainers();
        other.setParentMeshPointersToContainers();
    }

    friend void swap(Mesh& a, Mesh& b) noexcept { a.swap(b); }

    /**
     * @brief Checks if all containers in the mesh are compact (no deleted
     * elements).
     */
    bool isCompact() const
    {
        return (isContainerCompact<Rebound<Args>>() && ...);
    }

    /**
     * @brief Clears all elements across all containers in the mesh.
     */
    void clear() { (clearContainer<Rebound<Args>>(), ...); }

    /**
     * @brief Compacts all the containers of the mesh.
     *
     * Removes all the deleted elements from each container, compacting
     * the containers and then updating automatically all the reference indices.
     */
    void compact() { (compactContainer<Rebound<Args>>(), ...); }

    /**
     * @brief Checks if all containers in the mesh are empty.
     */
    bool empty() const { return (isContainerEmpty<Rebound<Args>>() && ...); }

    /**
     * @brief Enables all the optional components of the elements of the
     * containers of the mesh.
     */
    void enableAllOptionalComponents()
    {
        (enableAllContainer<Rebound<Args>>(), ...);
    }

    /**
     * @brief Disables all the optional components of the elements of the
     * containers of the mesh.
     */
    void disableAllOptionalComponents()
    {
        (disableAllContainer<Rebound<Args>>(), ...);
    }

    /**
     * @brief Returns the transformed bounding box of the mesh.
     *
     * If the mesh has both the BoundingBox and TransformMatrix components, the
     * bounding box is transformed by the transform matrix. Otherwise, it simply
     * returns the bounding box.
     *
     * @return The transformed bounding box, or the unmodified bounding box if
     * the transform matrix is not available.
     */
    auto transformedBoundingBox() const
        requires (mesh::HasBoundingBox<Mesh<Args...>>)
    {
        if constexpr (mesh::HasTransformMatrix<Mesh<Args...>>) {
            return transformBox(this->boundingBox(), this->transformMatrix());
        }
        else {
            return this->boundingBox();
        }
    }

    /**
     * @brief Enables all the OptionalComponents of this mesh according to the
     * Components available on the OtherMeshType m.
     *
     * This function is useful to call before importing data from another
     * MeshType, to be sure that all the available data contained in the
     * MeshType mesh will be imported.
     *
     * This function enables all the optional components of this mesh that are
     * available (optional or not) in m. Existing components already enabled in
     * this mesh are not disabled.
     *
     * Example of usage:
     *
     * @code{.cpp}
     * MeshType m1;
     * OtherMeshType m2;
     *
     * // do stuff on m2
     *
     * m1.enableSameOptionalComponentsOf(m2); // m1 enables all the available
     *                                        // components of m2
     * m1.importFrom(m2); // m1 will import all the data contained
     *                    // in m2 that can be stored in m1
     * @endcode
     *
     * @param[in] m: the mesh from which to enable the optional components.
     */
    template<typename OtherMeshType>
    void enableSameOptionalComponentsOf(const OtherMeshType& m)
    {
        (enableSameOptionalComponentsOfContainer<Rebound<Args>>(m), ...);
    }

    /**
     * @brief Appends all the elements contained in the mesh m to this mesh.
     *
     * @note This function does not enable the optional components that are
     * disabled in this mesh, but are available in m. Enabling the components
     * that are available in m, but not in this mesh, is up to the user.
     *
     * @warning This function is applied only to the containers of the meshes.
     * It does not import all the other components of the mesh m (e.g
     * BoundingBox, TransformMatrix, ...).
     *
     * @param[in] m: The mesh to append to this mesh.
     */
    void append(const Mesh& m)
    {
        if (this == &m) {
            Mesh copy = m;
            append(copy);
            return;
        }

        std::array<IndexType, ELEM_COUNT> initialSizes = {0};
        (saveInitialContainerSize<Rebound<Args>>(initialSizes), ...);

        (appendContainer<Rebound<Args>>(m), ...);

        (shiftReferencesOfContainer<Rebound<Args>>(initialSizes), ...);

        if constexpr (mesh::HasTransformMatrix<Mesh<Args...>>) {
            using MatrixType = typename Mesh<Args...>::TransformMatrixType;

            MatrixType matrix = this->transformMatrix();
            matrix            = matrix.inverse().eval();
            matrix *= m.transformMatrix();

            if (matrix != MatrixType::Identity()) {
                (updatePosAndNormalsOfContainerAfterAppend<Rebound<Args>>(
                     initialSizes, matrix),
                 ...);
            }
        }

        if constexpr (mesh::HasMaterials<Mesh<Args...>>) {
            ushort nMaterials = this->materialCount();

            // mapping from material indices of m to material indices of this
            std::vector<ushort> mapping(m.materialCount());

            // for each material of the other mesh, add it to this mesh
            // if it does not exist yet
            for (ushort i = 0; i < m.materialCount(); ++i) {
                auto it = std::find(
                    this->materialBegin(), this->materialEnd(), m.material(i));

                if (it == this->materialEnd()) {
                    this->pushMaterial(m.material(i));
                    mapping[i] = nMaterials++;
                }
                else {
                    mapping[i] = static_cast<ushort>(
                        std::distance(this->materialBegin(), it));
                }
            }

            for (const auto& [path, img] : m.textureImages()) {
                this->pushTextureImage(path, img);
            }

            if (!mapping.empty()) {
                (updateMaterialIndicesOfContainerAfterAppend<Rebound<Args>>(
                     initialSizes, mapping),
                 ...);
            }
        }
    }

    /**
     * @brief Imports all the components that can be imported from another type
     * of mesh.
     *
     * This function can be called from any Mesh type having all the Elements
     * and Components that implement the member function importFrom.
     *
     * @note this function does not enable optional components that are
     * disabled. If you want to import all the possible data including also
     * disabled components of this mesh, you should call the function
     * m1.enableSameOptionalComponentsOf(m2) before this function.
     *
     * @param[in] m: the mesh from which to import all the data.
     */
    template<typename OtherMeshType>
    void importFrom(const OtherMeshType& m)
    {
        (importFromHelper<Rebound<Args>>(m), ...);
    }

    /**
     * @brief Serializes the mesh to the given binary output stream.
     *
     * Writes the binary format magic header ("vcl_mesh", 8 bytes), the format
     * version (uint32_t = 1), and then sequentially serializes all element
     * containers (with their SoA column buffers and component enable states)
     * and serializable mesh-level components.
     *
     * @param[out] os: The binary output stream to write to.
     */
    void serialize(std::ostream& os) const
    {
        constexpr char MAGIC[8] = {'v', 'c', 'l', '_', 'm', 'e', 's', 'h'};
        os.write(MAGIC, 8);
        constexpr uint32_t VERSION = 1;
        vcl::serialize(os, VERSION);

        (serializeBase<Rebound<Args>>(os), ...);
    }

    /**
     * @brief Deserializes the mesh from the given binary input stream.
     *
     * Validates the binary format magic header ("vcl_mesh") and format
     * version (= 1), deserializes all element containers and mesh-level
     * components, and reconnects all child container parent pointers back
     * to this mesh.
     *
     * @param[in] is: The binary input stream to read from.
     * @throws std::runtime_error: If the magic header or format version is
     * invalid.
     */
    void deserialize(std::istream& is)
    {
        char magic[8];
        is.read(magic, 8);
        if (is.gcount() != 8 || std::memcmp(magic, "vcl_mesh", 8) != 0) {
            throw std::runtime_error(
                "Invalid mesh file header: expected 'vcl_mesh'");
        }
        uint32_t version = 0;
        vcl::deserialize(is, version);
        if (version != 1) {
            throw std::runtime_error(
                "Unsupported mesh binary format version: " +
                std::to_string(version));
        }

        (deserializeBase<Rebound<Args>>(is), ...);

        setParentMeshPointersToContainers();
    }

    /*** Generic Element functions ***/

    /**
     * @brief Returns the element of the given type at the given index inside
     * its container of this mesh.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * The function does not perform any check on the index. If the index is out
     * of range, the behaviour is undefined.
     *
     * @tparam ELEM_ID: the ID of the element to return.
     * @return the element of the given type ID at the given index inside its
     * container of this mesh.
     */
    template<ElemId ELEM_ID>
    decltype(auto) element(this auto&& self, IndexType i)
        requires (hasContainerOf<ELEM_ID>())
    {
        return self.ContainerType<ELEM_ID>::element(i);
    }

    /**
     * @brief Returns the number of elements of the given type in this mesh.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @return the number of elements of the given type in this mesh.
     */
    template<ElemId ELEM_ID>
    std::size_t count() const requires (hasContainerOf<ELEM_ID>())
    {
        return ContainerType<ELEM_ID>::elementCount();
    }

    /**
     * @brief Returns the size of the container of elements of the given type in
     * this mesh.
     *
     * The size of a container may be different from the number of elements, if
     * the container has some deleted elements.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @return the size of the container of elements of the given type in this
     * mesh.
     */
    template<ElemId ELEM_ID>
    std::size_t containerSize() const requires (hasContainerOf<ELEM_ID>())
    {
        return ContainerType<ELEM_ID>::elementContainerSize();
    }

    /**
     * @brief Returns the number of deleted elements of the given type in this
     * mesh.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @return the number of deleted elements of the given type in this mesh.
     */
    template<ElemId ELEM_ID>
    std::size_t deletedCount() const requires (hasContainerOf<ELEM_ID>())
    {
        return ContainerType<ELEM_ID>::deletedElementCount();
    }

    /**
     * @brief Adds a new element of the given type into its container, returning
     * the index of the added element in its container.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @return the index of the added element in its container.
     */
    template<ElemId ELEM_ID>
    decltype(auto) add() requires (hasContainerOf<ELEM_ID>())
    {
        return ContainerType<ELEM_ID>::addElement();
    }

    /**
     * @brief Adds `n` new elements of the given type into its container,
     * returning the index of the first added element in its container.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @param[in] n: the number of elements to add.
     * @return the index of the first added element in its container.
     */
    template<ElemId ELEM_ID>
    decltype(auto) add(std::size_t n) requires (hasContainerOf<ELEM_ID>())
    {
        return ContainerType<ELEM_ID>::addElements(n);
    }

    /**
     * @brief Clears the container of ELEM_ID elements in the Mesh, deleting all
     * the Elements.
     *
     * The contained elements are actually removed from the container, not only
     * marked as deleted. Therefore, the container will have size 0
     * (`mesh.containerSize<ELEM_ID>() == 0`) after the call of this function.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @note This function does not cause a reallocation of the container.
     *
     * @warning Any references to ELEM_ID elements in the Mesh will be left
     * unchanged, and therefore will point to invalid indices.
     *
     * @tparam ELEM_ID: the ID of the element.
     */
    template<ElemId ELEM_ID>
    void clearElements() requires (hasContainerOf<ELEM_ID>())
    {
        ContainerType<ELEM_ID>::clearElements();
    }

    /**
     * @brief Resizes the Element container to contain `n` Elements of type
     * ELEM_ID.
     *
     * If the new size is greater than the old one, new Elements are added to
     * the container, and a reallocation may happen. If the new size is smaller
     * than the old one, the container is truncated to `n` Elements (elements
     * beyond `n` are removed).
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @warning Any references or indices to removed Elements in the Mesh will
     * be left unchanged, and therefore will point to invalid Elements. This
     * means that if you call this member function with a lower number of
     * Elements, you'll need to manually manage the references/indices to the
     * removed Elements.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @param[in] n: the new size of the container in the mesh.
     */
    template<ElemId ELEM_ID>
    void resize(std::size_t n) requires (hasContainerOf<ELEM_ID>())
    {
        ContainerType<ELEM_ID>::resizeElements(n);
    }

    /**
     * @brief Reserves a number of elements of the given type in its container.
     * The function does not add any element to the container, but it just
     * reserves a number of elements that can be added without causing a
     * reallocation of the container.
     *
     * This is useful when you know (or you have an idea) of how much elements
     * are going to add in a mesh, and you want to avoid multiple (expansive)
     * reallocations of the container during the addition of the elements.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the type ID of the element.
     * @param[in] n: the new capacity of the container in the mesh.
     */
    template<ElemId ELEM_ID>
    void reserve(std::size_t n) requires (hasContainerOf<ELEM_ID>())
    {
        ContainerType<ELEM_ID>::reserveElements(n);
    }

    /**
     * @brief Compacts the Container of the given element, removing all the
     * elements marked as deleted. Element indices will change accordingly. The
     * function will automatically take care of updating all the Element
     * references contained in the Mesh.
     */
    template<ElemId ELEM_ID>
    auto compactElements() requires (hasContainerOf<ELEM_ID>())
    {
        return this->ContainerType<ELEM_ID>::compactElements();
    }

    /**
     * @brief Marks as deleted the element at the given index from its
     * container, deduced from the template index ELEM_ID.
     *
     * The function does not remove the element from the container, and
     * therefore it does not cause reallocation or compacting of the container.
     * The element will be removed from the container only when the container
     * will be compacted.
     *
     * The complexity of this function is O(1).
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @param[in] i: the index of the element to delete.
     */
    template<ElemId ELEM_ID>
    void deleteElement(std::size_t i) requires (hasContainerOf<ELEM_ID>())
    {
        ContainerType<ELEM_ID>::deleteElement(i);
    }

    template<ElementConcept El>
    void deleteElement(const El& e)
        requires (hasContainerOf<std::remove_cvref_t<El>::ELEMENT_ID>())
    {
        deleteElement<std::remove_cvref_t<El>::ELEMENT_ID>(e.index());
    }

    /**
     * @brief Returns a vector that tells, for each element of the container of
     * ELEM_ID in the mesh, the new index of the element after the container has
     * been compacted. For each deleted element, its position will be set to
     * NULL_INDEX.
     *
     * This function is useful when you need to know the indices of the elements
     * that they would have in a compact container, without considering the
     * deleted ones.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @return a vector containing, for each element index, the new index if the
     * container would be compacted.
     */
    template<ElemId ELEM_ID>
    auto compactIndices() const requires (hasContainerOf<ELEM_ID>())
    {
        return ContainerType<ELEM_ID>::elementCompactIndices();
    }

    /**
     * @brief Returns the new index of the element at index `i` of the container
     * of ELEM_ID after compaction, or NULL_INDEX if the element is deleted.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @param[in] i: the index of the element.
     * @return the new index after compaction, or NULL_INDEX if deleted.
     */
    template<ElemId ELEM_ID>
    IndexType indexIfCompact(IndexType i) const
        requires (hasContainerOf<ELEM_ID>())
    {
        return ContainerType<ELEM_ID>::elementIndexIfCompact(i);
    }

    /**
     * @brief Updates all the indices and references of the elements of the
     * container of ELEM_ID in the mesh, according to the mapping stored in the
     * newIndices vector, that tells for each old element index, the new index
     * of the element in the same container (or NULL_INDEX if the element must
     * be left as unreferenced - useful when an element is deleted).
     *
     * This function is useful when you move some vertices (e.g. it is
     * automatically called after every compacting of a container), and you want
     * to update the indices/references of the elements stored in all the
     * containers of the mesh.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @note This function *does not change the position of the elements in
     * their container*. It just updates the indices/references of the elements
     * stored in their or other containers. This function should be called after
     * the elements have been actually moved in their container.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @param[in] newIndices: a vector that tells, for each old element index,
     * the new element index. If the old element must be left as unreferenced,
     * the value of the vector must be NULL_INDEX.
     */
    template<ElemId ELEM_ID>
    void updateReferences(const std::vector<IndexType>& newIndices)
        requires (hasContainerOf<ELEM_ID>())
    {
        (updateContainerReferences<Rebound<Args>, ELEM_ID>(newIndices), ...);
    }

    /**
     * Returns a lightweight view object that stores the begin and end iterators
     * of the container of the elements having ID ELEM_ID in the mesh. The view
     * object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (auto& el : mesh.elements<ElemId::VERTEX>()) {
     *     // Do something with el
     * }
     * @endcode
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @param[in] jumpDeleted: if `true`, the view will be initialized to jump
     * deleted elements in the container.
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over elements.
     */
    template<ElemId ELEM_ID>
    decltype(auto) elements(this auto&& self, bool jumpDeleted = true)
        requires (hasContainerOf<ELEM_ID>())
    {
        return self.ContainerType<ELEM_ID>::elements(jumpDeleted);
    }

    /**
     * @brief Returns a view object that allows to iterate over the elements of
     * the container of type ELEM_ID in the range [begin, end), providing
     * begin() and end() member functions.
     *
     * @note Unlike the elements() function, this member function does not
     * automatically jump deleted elements, but it iterates over the
     * elements in the given range, regardless of whether they are deleted or
     * not.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @param[in] begin: the index of the first element to be included in the
     * range. It must be less than or equal to elementContainerSize() and less
     * than or equal to the end index.
     * @param[in] end: the index of the last element to be included in the
     * range (exclusive). If NULL_INDEX (default) or greater than
     * elementContainerSize(), it will be set to elementContainerSize().
     * @return A View object allowing to iterate over the given range of the
     * container.
     */
    template<ElemId ELEM_ID>
    decltype(auto) elements(
        this auto&& self,
        IndexType   begin,
        IndexType   end = NULL_INDEX) requires (hasContainerOf<ELEM_ID>())
    {
        return self.ContainerType<ELEM_ID>::elements(begin, end);
    }

    /**
     * @brief Returns `true` if Component having ID `COMP_ID` is available
     * (present at compile time and enabled at run time) for elements having ID
     * `ELEM_ID` in the mesh.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @tparam COMP_ID: the ID of the component.
     * @return `true` if Component `COMP_ID` is available for elements of
     * type `ELEM_ID` in the mesh.
     */
    template<ElemId ELEM_ID, CompId COMP_ID>
    bool isElementComponentAvailable() const
        requires (hasContainerOf<ELEM_ID>())
    {
        return ContainerType<ELEM_ID>::template isComponentAvailable<COMP_ID>();
    }

    /**
     * @brief Enables the optional Component having ID `COMP_ID` for elements
     * having ID `ELEM_ID` in the mesh.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID, and that the Element has a optional component having ID
     * COMP_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @tparam COMP_ID: the ID of the component.
     */
    template<ElemId ELEM_ID, CompId COMP_ID>
    void enableElementComponent()
        requires (hasElementOptionalComponent<ELEM_ID, COMP_ID>())
    {
        ContainerType<ELEM_ID>::template enableOptionalComponent<COMP_ID>();
    }

    /**
     * @brief Disables the optional Component having ID `COMP_ID` for elements
     * having ID `ELEM_ID` in the mesh.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID, and that the Element has a optional component having ID COMP_ID.
     * Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @tparam COMP_ID: the ID of the component.
     */
    template<ElemId ELEM_ID, CompId COMP_ID>
    void disableElementComponent()
        requires (hasElementOptionalComponent<ELEM_ID, COMP_ID>())
    {
        ContainerType<ELEM_ID>::template disableOptionalComponent<COMP_ID>();
    }

    template<ElemId ELEM_ID>
    void enableAllElementOptionalComponents()
        requires (hasContainerOf<ELEM_ID>())
    {
        ContainerType<ELEM_ID>::enableAllOptionalComponents();
    }

    template<ElemId ELEM_ID>
    void disableAllElementOptionalComponents()
        requires (hasContainerOf<ELEM_ID>())
    {
        ContainerType<ELEM_ID>::disableAllOptionalComponents();
    }

    /**
     * @brief Returns `true` if the Element having ID `ELEM_ID` has a custom
     * component with the given name.
     *
     * This function does not take into account the type of the custom
     * component.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID, and that the Element has CustomComponents.
     * Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @param[in] name: the name of the custom component.
     * @return `true` if the Element having ID `ELEM_ID` has a custom component
     * having the given name.
     */
    template<ElemId ELEM_ID>
    bool hasElementCustomComponent(const std::string& name) const
        requires (hasElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        return ContainerType<ELEM_ID>::hasElemCustomComponent(name);
    }

    /**
     * @brief Returns a vector containing all the names of the custom components
     * of any type associated to the Element having ID `ELEM_ID`.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID, and that the Element has CustomComponents.
     * Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @return A vector of strings representing all the names of the custom
     * components for the Element having ID `ELEM_ID`.
     */
    template<ElemId ELEM_ID>
    std::vector<std::string> elementCustomComponentNames() const
        requires (hasElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        return ContainerType<ELEM_ID>::elemCustomComponentNames();
    }

    /**
     * @brief Returns `true` if the Element having ID `ELEM_ID` has a custom
     * component of type `K` with the given name.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID, and that the Element has CustomComponents.
     * Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @tparam K: the type of the custom component.
     * @param[in] name: the name of the custom component.
     * @return `true` if the Element having ID `ELEM_ID` has a custom component
     * of type `K` having the given name.
     */
    template<ElemId ELEM_ID, typename K>
    bool isElementCustomComponentOfType(const std::string& name) const
        requires (hasElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        return ContainerType<ELEM_ID>::template isElemCustomComponentOfType<K>(
            name);
    }

    /**
     * @brief Returns the std::type_index of the custom component having the
     * given name associated to the Element having ID `ELEM_ID`.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID, and that the Element has CustomComponents.
     * Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @param[in] name: the name of the custom component.
     * @return The std::type_index of the custom component having the given name
     * associated to the Element having ID `ELEM_ID`.
     */
    template<ElemId ELEM_ID>
    std::type_index elementCustomComponentType(const std::string& name) const
        requires (hasElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        return ContainerType<ELEM_ID>::elemCustomComponentType(name);
    }

    /**
     * @brief Returns a vector containing all the names of the custom components
     * of type `K` associated to the Element having ID `ELEM_ID`.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID, and that the Element has CustomComponents.
     * Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @tparam K: the type of the custom component.
     * @return A vector of strings representing all the names of the custom
     * components of type `K` for the Element having ID `ELEM_ID`.
     */
    template<ElemId ELEM_ID, typename K>
    std::vector<std::string> elementCustomComponentNamesOfType() const
        requires (hasElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        return ContainerType<ELEM_ID>::template elemCustomComponentNamesOfType<
            K>();
    }

    /**
     * @brief Adds a custom component of type K having the given name to the
     * Element having ID `ELEM_ID`.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID, and that the Element has CustomComponents.
     * Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @tparam K: the type of the custom component.
     * @param[in] name: the name of the custom component.
     */
    template<ElemId ELEM_ID, typename K>
    void addElementCustomComponent(const std::string& name)
        requires (hasElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        ContainerType<ELEM_ID>::template addElemCustomComponent<K>(name);
    }

    /**
     * @brief Deletes the custom component of the given name from the Element
     * having ID `ELEM_ID`.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID, and that the Element has CustomComponents.
     * Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @param[in] name: the name of the custom component.
     */
    template<ElemId ELEM_ID>
    void deleteElementCustomComponent(const std::string& name)
        requires (hasElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        ContainerType<ELEM_ID>::deleteElemCustomComponent(name);
    }

    /**
     * @brief Returns a vector handle to the custom component of type `K` having
     * the given name associated to the Element having ID `ELEM_ID`.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID, and that the Element has CustomComponents.
     * Otherwise, a compiler error will be triggered.
     *
     * The handle can be used like a normal std::vector, but does not have
     * access to the modifiers member functions (resize, push_back...). The
     * handle contains **references** to the custom component, therefore you can
     * modify the custom component by modifying the element of the handle vector
     * normally. Since the handle stores references, there are no copies
     * performed when calling this function.
     *
     * For example, assuming that the mesh has a vertex custom component named
     * "cc" of type int:
     *
     * @code{.cpp}
     * auto handle = m.elementCustomComponentVectorHandle<
     *      ElemId::VERTEX, int>("cc");
     * for (Vertex v : m.vertices()) {
     *    handle[v.index()] = 5; // v.customComponent<int>("cc") == 5
     *    assert(v.customComponent<int>("cc") == 5);
     * }
     * @endcode
     *
     * Using handles allows to access more efficiently to custom components
     * rather accessing from an element object. However, note that references
     * are bound to the container of the mesh.
     *
     * @note Since the handle contains references, any operation that changes
     * the size of the container could be destructive and invalidate the
     * references contained in the handle.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @tparam K: the type of the custom component.
     * @param[in] name: the name of the custom component.
     * @return A handle to the custom component of type `K` having the given
     * name associated to the Element having ID `ELEM_ID`.
     */
    template<ElemId ELEM_ID, typename K>
    decltype(auto) elementCustomComponentVectorHandle(
        this auto&&        self,
        const std::string& name)
        requires (hasElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        return self
            .ContainerType<ELEM_ID>::template customComponentVectorHandle<K>(
                name);
    }

    /**
     * @brief Serializes the custom components of type `CompType` of the element
     * with ID ELEM_ID.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * E.g. suppose to have a Mesh having vertices with three custom components,
     * two of type `float` and one of type `int`. To serialize ONLY the custom
     * components of type `float`, you can call:
     *
     * @code{.cpp}
     * MeshType m;
     * std::ofstream os("customComponents.bin");
     * m.template
     *     serializeElementCustomComponentsOfType<
     *         ElemId::VERTEX, float>(os);
     * os.close();
     * @endcode
     *
     * @tparam ELEM_ID: the ID of the element.
     * @tparam CompType: the type of the component to serialize.
     * @param[in] os: the output stream to which serialize the custom
     * components.
     */
    template<ElemId ELEM_ID, typename CompType>
    void serializeElementCustomComponentsOfType(std::ostream& os) const
        requires (hasElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        ContainerType<ELEM_ID>::template serializeElemCustomComponentsOfType<
            CompType>(os);
    }

    /**
     * @brief Deserializes the custom components of type `CompType` of the
     * element with ID ELEM_ID.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @tparam CompType: the type of the component to deserialize.
     * @param[in] is: the input stream from which deserialize the custom
     * components.
     */
    template<ElemId ELEM_ID, typename CompType>
    void deserializeElementCustomComponentsOfType(std::istream& is)
        requires (hasElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        ContainerType<ELEM_ID>::template deserializeElemCustomComponentsOfType<
            CompType>(is);
    }

private:
    /**
     * @brief Returns a reference to the container for the given ELEM_ID.
     */
    template<ElemId ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    ContainerType<ELEM_ID>& container()
    {
        return static_cast<ContainerType<ELEM_ID>&>(*this);
    }

    /**
     * @brief Returns a const reference to the container for the given ELEM_ID.
     */
    template<ElemId ELEM_ID>
    requires (hasContainerOf<ELEM_ID>())
    const ContainerType<ELEM_ID>& container() const
    {
        return static_cast<const ContainerType<ELEM_ID>&>(*this);
    }

    template<typename T>
    void setParentMeshPointer()
    {
        if constexpr (mesh::ElementContainerConcept<T>) {
            this->T::setParentMesh(this);
        }
    }

    void setParentMeshPointersToContainers()
    {
        (setParentMeshPointer<Rebound<Args>>(), ...);
    }

    template<typename T>
    void enableAllContainer()
    {
        if constexpr (mesh::ElementContainerConcept<T>) {
            this->T::enableAllOptionalComponents();
        }
    }

    template<typename T>
    void disableAllContainer()
    {
        if constexpr (mesh::ElementContainerConcept<T>) {
            this->T::disableAllOptionalComponents();
        }
    }

    template<typename Cont, typename OtherMeshType>
    void enableSameOptionalComponentsOfContainer(const OtherMeshType& m)
    {
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            if constexpr (OtherMeshType::template hasContainerOf<
                              Cont::ELEMENT_ID>()) {
                this->Cont::enableSameOptionalComponentsOf(
                    m.template container<Cont::ELEMENT_ID>());
            }
        }
    }

    template<typename BaseType, typename OtherMeshType>
    void importFromHelper(const OtherMeshType& m)
    {
        this->BaseType::importFrom(m);
    }

    template<typename Cont>
    void saveInitialContainerSize(
        std::array<IndexType, ELEM_COUNT>& sizes) const
    {
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            sizes[toUnderlying(Cont::ELEMENT_ID)] =
                this->Cont::elementContainerSize();
        }
    }

    template<typename Cont>
    void appendContainer(const Mesh& m)
    {
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            this->Cont::append(static_cast<const Cont&>(m));
        }
    }

    template<typename Cont, ElemId TARGET_ELEM_ID>
    void shiftContainerReferences(IndexType startFrom, IndexType offset)
    {
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            this->Cont::template shiftReferences<TARGET_ELEM_ID>(
                startFrom, offset);
        }
    }

    template<typename Cont>
    void shiftReferencesOfContainer(
        const std::array<IndexType, ELEM_COUNT>& initialSizes)
    {
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            const IndexType startFrom =
                initialSizes[toUnderlying(Cont::ELEMENT_ID)];
            [&]<uint... TargetIDs>(std::integer_sequence<uint, TargetIDs...>) {
                (shiftContainerReferences<Cont, static_cast<ElemId>(TargetIDs)>(
                     startFrom, initialSizes[TargetIDs]),
                 ...);
            }(std::make_integer_sequence<uint, ELEM_COUNT> {});
        }
    }

    template<typename Cont, typename MatrixType>
    void updatePosAndNormalsOfContainerAfterAppend(
        const std::array<IndexType, ELEM_COUNT>& initialSizes,
        const MatrixType&                        matrix)
    {
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            constexpr ElemId ELEM_ID   = Cont::ELEMENT_ID;
            const IndexType  startFrom = initialSizes[toUnderlying(ELEM_ID)];
            if (startFrom >= this->Cont::elementContainerSize()) {
                return;
            }

            if constexpr (comp::HasComponent<CompId::POSITION, Cont>) {
                if (this->Cont::template isComponentAvailable<
                        CompId::POSITION>()) {
                    auto posview = this->template elements<ELEM_ID>(startFrom) |
                                   views::positions;
                    multiplyPointsByMatrix(posview, matrix);
                }
            }

            if constexpr (comp::HasComponent<CompId::NORMAL, Cont>) {
                if (this->Cont::template isComponentAvailable<
                        CompId::NORMAL>()) {
                    auto norview = this->template elements<ELEM_ID>(startFrom) |
                                   views::normals;
                    multiplyNormalsByMatrix(norview, matrix);
                }
            }

            if constexpr (comp::HasComponent<CompId::WEDGE_NORMALS, Cont>) {
                if (this->Cont::template isComponentAvailable<
                        CompId::WEDGE_NORMALS>()) {
                    for (auto f : this->template elements<ELEM_ID>(startFrom)) {
                        for (auto w : f.wedges()) {
                            w.normal() =
                                multiplyNormalByMatrix(w.normal(), matrix);
                        }
                    }
                }
            }

            if constexpr (comp::HasComponent<CompId::TANGENT, Cont>) {
                if (this->Cont::template isComponentAvailable<
                        CompId::TANGENT>()) {
                    auto tanview = this->template elements<ELEM_ID>(startFrom) |
                                   views::tangents;
                    multiplyNormalsByMatrix(tanview, matrix);

                    Matrix33<double> m33 =
                        matrix.template cast<double>().block(0, 0, 3, 3);
                    if (m33.determinant() < 0) {
                        for (auto e :
                             this->template elements<ELEM_ID>(startFrom)) {
                            e.tangentRightHanded() = !e.tangentRightHanded();
                        }
                    }
                }
            }

            if constexpr (comp::
                              HasComponent<CompId::PRINCIPAL_CURVATURE, Cont>) {
                if (this->Cont::template isComponentAvailable<
                        CompId::PRINCIPAL_CURVATURE>()) {
                    for (auto e : this->template elements<ELEM_ID>(startFrom)) {
                        e.principalCurvature().maxDir() =
                            multiplyNormalByMatrix(
                                e.principalCurvature().maxDir(), matrix);
                        e.principalCurvature().minDir() =
                            multiplyNormalByMatrix(
                                e.principalCurvature().minDir(), matrix);
                    }
                }
            }
        }
    }

    template<typename Cont, typename MappingType>
    void updateMaterialIndicesOfContainerAfterAppend(
        const std::array<IndexType, ELEM_COUNT>& initialSizes,
        const MappingType&                       mapping)
    {
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            constexpr ElemId ELEM_ID   = Cont::ELEMENT_ID;
            const IndexType  startFrom = initialSizes[toUnderlying(ELEM_ID)];
            if (startFrom >= this->Cont::elementContainerSize()) {
                return;
            }

            if constexpr (comp::HasComponent<CompId::MATERIAL_INDEX, Cont>) {
                if (this->Cont::template isComponentAvailable<
                        CompId::MATERIAL_INDEX>()) {
                    for (auto e : this->template elements<ELEM_ID>(startFrom)) {
                        if (e.materialIndex() != USHORT_NULL &&
                            e.materialIndex() < mapping.size()) {
                            e.materialIndex() = mapping[e.materialIndex()];
                        }
                    }
                }
            }
        }
    }

    template<typename T>
    bool isContainerCompact() const
    {
        if constexpr (mesh::ElementContainerConcept<T>) {
            return this->T::deletedElementCount() == 0;
        }
        else {
            return true;
        }
    }

    template<typename T>
    void clearContainer()
    {
        if constexpr (mesh::ElementContainerConcept<T>) {
            this->T::clearElements();
        }
    }

    template<typename T>
    void compactContainer()
    {
        if constexpr (mesh::ElementContainerConcept<T>) {
            this->T::compactElements();
        }
    }

    template<typename T, ElemId ELEM_ID>
    void updateContainerReferences(const std::vector<IndexType>& newIndices)
    {
        if constexpr (mesh::ElementContainerConcept<T>) {
            this->T::template updateReferences<ELEM_ID>(newIndices);
        }
    }

    template<typename T>
    bool isContainerEmpty() const
    {
        if constexpr (mesh::ElementContainerConcept<T>) {
            return this->T::elementCount() == 0;
        }
        else {
            return true;
        }
    }

    template<typename T>
    void serializeBase(std::ostream& os) const
    {
        if constexpr (requires { this->T::serialize(os); }) {
            this->T::serialize(os);
        }
    }

    template<typename T>
    void deserializeBase(std::istream& is)
    {
        if constexpr (requires { this->T::deserialize(is); }) {
            this->T::deserialize(is);
        }
    }

    template<typename...>
    friend class Mesh;

    template<
        typename ComponentList,
        typename ParentMesh,
        std::unsigned_integral Index,
        template<typename...> class Element>
    friend class mesh::ElementContainer;
};

/* Mesh Concepts */

/**
 * @brief Concept that checks whether a type T satisfies the Mesh concept.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a Mesh class.
 *
 * @tparam T: The type to be tested for conformity to the MeshConcept.
 *
 * @ingroup mesh
 * @ingroup mesh_concepts
 */
template<typename T>
concept MeshConcept = IsDerivedFromSpecializationOfV<T, Mesh>;

} // namespace vcl

#endif // VCL_MESH_MESH_H
