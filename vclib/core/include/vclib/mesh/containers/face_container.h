// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_CONTAINERS_FACE_CONTAINER_H
#define VCL_MESH_CONTAINERS_FACE_CONTAINER_H

#include <vclib/mesh/components/base/component_predicates.h>
#include <vclib/mesh/containers/base/element_container.h>
#include <vclib/mesh/containers/base/macros.h>
#include <vclib/mesh/elements/face.h>
#include <vclib/mesh/elements/face_components.h>
#include <vclib/mesh/views/components/positions.h>

#include <vclib/algorithms/core.h>
#include <vclib/base.h>

namespace vcl::mesh {

/**
 * @brief FaceContainer stores the per-face data in a Structure of Arrays (SoA),
 * and it is meant to be used as a template parameter of the Mesh class.
 *
 * It accepts the Face component list (satisfying HasRequiredFaceComponents),
 * instantiates the underlying storage and exposes the Face element view.
 *
 * @tparam CompList: The face components type (e.g. FaceComponents<BitFlags,
 * TriangleVertexReferences<uint>>).
 * @tparam Index: The index type (default uint).
 * @tparam ParentMesh: The parent mesh type. On Mesh instantiation, this is
 * automatically set to the Mesh type. Default is void.
 *
 * @ingroup containers
 */
template<
    HasRequiredFaceComponents CompList,
    std::unsigned_integral    Index = uint,
    typename ParentMesh             = void>
class FaceContainer :
        public ElementContainer<
            typename comp::detail::ComponentListHelper<CompList>::type,
            ParentMesh,
            Index,
            Face>
{
    using Base = ElementContainer<
        typename comp::detail::ComponentListHelper<CompList>::type,
        ParentMesh,
        Index,
        Face>;

public:
    using Face           = typename Base::ElementType;
    using ConstFace      = typename Base::ConstElementType;
    using FaceComponents = typename Base::Components;
    using Components     = FaceComponents;
    using Base::NULL_INDEX;

    // Rebind idiom to allow changing the ParentMesh type while keeping the same
    // component list and index type.
    template<typename NewParent>
    using Rebind = FaceContainer<CompList, Index, NewParent>;

    /**
     * @brief Empty constructor that creates an empty container of Faces.
     */
    FaceContainer() = default;

    friend void swap(FaceContainer& a, FaceContainer& b) noexcept { a.swap(b); }

    /**
     * @brief Returns the Face at the i-th position in the Face Container of
     * the Mesh, which will be the face having index = i.
     *
     * This function does not perform any sanity check: if i is less than
     * faceContainerSize(), this function will return a valid Face
     * (note that the Face may have been flagged as deleted).
     *
     * @param[in] i: the index of the face that will be returned.
     *
     * @return The i-th Face of the Face Container.
     */
    Face face(Index i) { return Base::element(i); }

    /**
     * @brief Returns the const Face at the i-th position in the Face
     * Container of the Mesh, which will be the face having index = i.
     *
     * This function does not perform any sanity check: if i is less than
     * faceContainerSize(), this function will return a valid ConstFace
     * (note that the Face may have been flagged as deleted).
     *
     * @param[in] i: the index of the face that will be returned.
     *
     * @return The i-th const Face of the Face Container.
     */
    ConstFace face(Index i) const { return Base::element(i); }

    /**
     * @brief Returns the number of **non-deleted** faces contained in the
     * Face container of the Mesh.
     *
     * If faceCount() != faceContainerSize(), it means that there are some
     * faces that are flagged as deleted.
     *
     * @return The number of non-deleted faces of the Mesh.
     */
    std::size_t faceCount() const { return Base::elementCount(); }

    /**
     * @brief Returns the number of faces (also deleted) contained in the
     * Face container of the Mesh.
     *
     * If faceCount() != faceContainerSize(), it means that there are some
     * faces that are flagged as deleted.
     *
     * @return The number of all the faces contained in the Mesh.
     */
    std::size_t faceContainerSize() const
    {
        return Base::elementContainerSize();
    }

    /**
     * @brief Returns the number of deleted faces in the Face container,
     * that is faceContainerSize() - faceCount().
     *
     * @return The number of deleted faces in the container.
     */
    std::size_t deletedFaceCount() const { return Base::deletedElementCount(); }

    /**
     * @brief Adds a new face into the Face container, returning the index of
     * the added face.
     *
     * @return The index of the new face.
     */
    Index addFace() { return Base::addElement(); }

    /**
     * @brief Adds a new face with the given vertices or vertex indices into the
     * Face container, returning the index of the added face.
     *
     * @tparam Args: The types of the vertex arguments (references or indices).
     * @param[in] args: The vertices or vertex indices of the new face.
     * @return The index of the new face.
     */
    template<typename... Args>
    Index addFace(const Args&... args) requires (
        sizeof...(Args) >= 3 &&
        (sizeof...(Args) == Face::VERTEX_COUNT || Face::VERTEX_COUNT < 0))
    {
        Index fid = addFace();
        face(fid).setVertices(args...);
        return fid;
    }

    /**
     * @brief Adds a new face with the vertices or vertex indices contained in
     * the given range, returning the index of the added face.
     *
     * @tparam Rng: An input range type containing vertices or vertex indices.
     * @param[in] rng: The range of vertices or vertex indices of the new face.
     * @return The index of the new face.
     */
    template<std::ranges::input_range Rng>
    Index addFace(Rng&& rng)
    {
        Index fid = addFace();
        face(fid).setVertices(std::forward<Rng>(rng));
        return fid;
    }

    /**
     * @brief Adds an arbitrary number of n faces, returning the index of the
     * first added face.
     *
     * @param[in] n: the number of faces to add to the mesh.
     * @return The index of the first added face.
     */
    Index addFaces(std::size_t n) { return Base::addElements(n); }

    /**
     * @brief Adds an arbitrary number of faces with the vertices or vertex
     * indices contained in the given range of ranges, returning the index of
     * the first added face.
     *
     * @tparam Rng: A range of ranges containing vertices or vertex indices.
     * @param[in] rng: A range of ranges containing the vertices or vertex
     * indices for each face.
     * @return The index of the first added face.
     */
    template<std::ranges::input_range Rng>
    requires std::ranges::input_range<std::ranges::range_value_t<Rng>>
    Index addFaces(Rng&& rng)
    {
        Index baseId = faceContainerSize();
        if constexpr (std::ranges::sized_range<Rng>) {
            reserveFaces(baseId + std::ranges::size(rng));
        }
        for (auto&& f : rng) {
            addFace(f);
        }
        return baseId;
    }

    /**
     * @brief Clears the Face container of the Mesh, deleting all the
     * faces.
     *
     * The contained face components are actually removed from the container,
     * not only marked as deleted. Therefore, the container will have size 0
     * (`faceContainerSize() == 0`) after the call of this function.
     *
     * @warning Any index or reference to faces in the Mesh will be left
     * unchanged, and therefore will point to invalid faces.
     */
    void clearFaces() { Base::clearElements(); }

    /**
     * @brief Resizes the Face container to contain `n` faces.
     *
     * If the new size is greater than the old one, new faces are added to
     * the container. If the new size is smaller than the old one, the container
     * is truncated to `n` faces (faces beyond `n` are removed).
     *
     * @warning Any index to removed faces in the Mesh will be left
     * unchanged, and therefore will point to invalid faces.
     *
     * @param[in] n: the new size of the Face container.
     */
    void resizeFaces(std::size_t n) { Base::resizeElements(n); }

    /**
     * @brief Reserves a number of faces in the container of Faces. This is
     * useful when you know (or you have an idea) of how many faces will be
     * added to a new or existing mesh. Calling this function before any
     * addFace() call will avoid unnecessary reallocations of the container,
     * saving execution time.
     *
     * The philosophy of this function is similar to the one of the
     * [reserve()](https://en.cppreference.com/w/cpp/container/vector/reserve)
     * function of the [std::vector
     * class](https://en.cppreference.com/w/cpp/container/vector).
     *
     * @param[in] n: the new capacity of the face container.
     */
    void reserveFaces(std::size_t n) { Base::reserveElements(n); }

    /**
     * @brief Compacts the Face Container, removing all the faces marked as
     * deleted. Face references will automatically change accordingly in all
     * the containers of the Mesh.
     *
     * @return A vector that maps each old face index to its new index in the
     * compacted container (or NULL_INDEX if it was deleted).
     */
    std::vector<Index> compactFaces() { return Base::compactElements(); }

    /**
     * @brief Marks as deleted the face with the given id.
     *
     * This member function does not perform any reallocation of the faces:
     * the deleted faces will stay in the Face Container, but will be
     * marked as deleted.
     *
     * Deleted faces are automatically jumped by the iterators provided by
     * the Face Container.
     *
     * @param[in] i: the index of the face that will be marked as deleted.
     */
    void deleteFace(Index i) { Base::deleteElement(i); }

    /**
     * @brief Marks as deleted the given face.
     *
     * This member function does not perform any reallocation of the faces:
     * the deleted faces will stay in the Face Container, but will be
     * marked as deleted.
     *
     * Deleted faces are automatically jumped by the iterators provided by
     * the Face Container.
     *
     * @param[in] f: the face that will be marked as deleted.
     */
    void deleteFace(const Face& f) { Base::deleteElement(f.index()); }

    /**
     * @brief This is a utility member function that returns the index of an
     * element if the container were compact, that is the number of
     * non-deleted elements before the face with the given index.
     *
     * Complexity: O(n), with n the number of faces in the container.
     *
     * This function does not perform any sanity check on the given index.
     *
     * @param[in] i: the index of a face of the container.
     * @return The index that the face with index i would have if this
     * container were compact.
     */
    Index faceIndexIfCompact(Index i) const
    {
        return Base::elementIndexIfCompact(i);
    }

    /**
     * @brief Returns a vector that tells, for each actual face index, the new
     * index that the face would have in a compacted container. For each
     * deleted face index, the value of the vector will be NULL_INDEX.
     *
     * This is useful if you need to know the indices of the faces that they
     * would have in a compact container, without considering the deleted ones.
     *
     * @return A vector containing, for each face index, its index if the
     * container were compact.
     */
    std::vector<Index> faceCompactIndices() const
    {
        return Base::elementCompactIndices();
    }

    /**
     * @brief Updates references to the faces of this mesh according to the
     * mapping stored in the newIndices vector.
     *
     * This function is useful when faces are deleted, compacted, or
     * permuted, and you want to update all references stored across the
     * mesh containers to point to their new indices.
     *
     * @note This function does not change the position of the faces in this
     * container. It updates face references stored in this and other
     * containers of the mesh.
     *
     * @param[in] newIndices: A vector mapping old face indices to new ones.
     * If a face was deleted or should become unreferenced, its mapping entry
     * must be NULL_INDEX.
     */
    void updateFaceReferences(const std::vector<Index>& newIndices)
    {
        Base::updateElementReferences(newIndices);
    }

    /**
     * @brief Returns an ElementContainerProxy object that allows to iterate
     * over the faces of the container, providing begin() and end() member
     * functions.
     *
     * This member function is very useful when you want to iterate over the
     * faces using the C++ range-based for loop syntax:
     *
     * @code{.cpp}
     * for (Face f : m.faces()) {
     *     // do something with this face
     * }
     * @endcode
     *
     * The iterator used to iterate over faces is automatically initialized
     * to jump deleted faces of the container. You can change this option by
     * calling this function with jumpDeleted = false.
     *
     * @param[in] jumpDeleted (default: true): boolean that tells if the
     * iterator should jump deleted faces.
     * @return An object having begin() and end() functions, allowing to
     * iterate over the container.
     */
    auto faces(bool jumpDeleted = true) { return Base::elements(jumpDeleted); }

    /**
     * @brief Returns an ElementContainerProxy object that allows to iterate
     * over the faces of the container, providing begin() and end() member
     * functions.
     *
     * This member function is very useful when you want to iterate over the
     * faces using the C++ range-based for loop syntax:
     *
     * @code{.cpp}
     * for (ConstFace f : m.faces()) {
     *     // do something with this face
     * }
     * @endcode
     *
     * The iterator used to iterate over faces is automatically initialized
     * to jump deleted faces of the container. You can change this option by
     * calling this function with jumpDeleted = false.
     *
     * @param[in] jumpDeleted (default: true): boolean that tells if the
     * iterator should jump deleted faces.
     * @return An object having begin() and end() functions, allowing to
     * iterate over the container.
     */
    auto faces(bool jumpDeleted = true) const
    {
        return Base::elements(jumpDeleted);
    }

    /**
     * @brief Returns a view object that allows to iterate over the faces of
     * the container in the range [begin, end), providing begin() and end()
     * member functions.
     *
     * @note Unlike the faces() function, this member function does not
     * automatically jump deleted faces, but it iterates over the
     * faces in the given range, regardless of whether they are deleted or
     * not.
     *
     * @param[in] begin: the index of the first face to be included in the
     * range. It must be less than or equal to faceContainerSize() and less than
     * or equal to the end index.
     * @param[in] end: the index of the last face to be included in the
     * range (exclusive). If NULL_INDEX (default) or greater than
     * faceContainerSize(), it will be set to faceContainerSize().
     * @return A View object allowing to iterate over the given range of the
     * container.
     */
    auto faces(Index begin, Index end = NULL_INDEX)
    {
        return Base::elements(begin, end);
    }

    /**
     * @brief Returns a view object that allows to iterate over the faces of
     * the container in the range [begin, end), providing begin() and end()
     * member functions.
     *
     * @note Unlike the faces() function, this member function does not
     * automatically jump deleted faces, but it iterates over the
     * faces in the given range, regardless of whether they are deleted or
     * not.
     *
     * @param[in] begin: the index of the first face to be included in the
     * range. It must be less than or equal to faceContainerSize() and less than
     * or equal to the end index.
     * @param[in] end: the index of the last face to be included in the
     * range (exclusive). If NULL_INDEX (default) or greater than
     * faceContainerSize(), it will be set to faceContainerSize().
     * @return A View object allowing to iterate over the given range of the
     * container.
     */
    auto faces(Index begin, Index end = NULL_INDEX) const
    {
        return Base::elements(begin, end);
    }

    // --- Components ---

    /**
     * @brief Checks if the face component with the given COMP_ID is
     * available (present at compile time and enabled at run time).
     *
     * Non-optional components are always available (true).
     * Optional components return true if currently enabled, false otherwise.
     *
     * @tparam COMP_ID: The ID of the component to check.
     * @return `true` if currently available, `false` otherwise.
     */
    template<CompId COMP_ID>
    bool isFaceComponentAvailable() const
    {
        return Base::template isComponentAvailable<COMP_ID>();
    }

    /**
     * @brief Enables the optional face component with the given COMP_ID.
     *
     * @tparam COMP_ID: The ID of the component to enable.
     */
    template<CompId COMP_ID>
    void enableFaceComponent()
        requires comp::HasOptionalComponent<COMP_ID, FaceComponents>
    {
        Base::template enableOptionalComponent<COMP_ID>();
    }

    /**
     * @brief Disables the optional face component with the given COMP_ID.
     * Note: Non-optional components cannot be disabled.
     *
     * @tparam COMP_ID: The ID of the component to disable.
     */
    template<CompId COMP_ID>
    void disableFaceComponent()
        requires comp::HasOptionalComponent<COMP_ID, FaceComponents>
    {
        Base::template disableOptionalComponent<COMP_ID>();
    }

    /**
     * @brief Enables all optional face components in the container.
     */
    void enableAllFaceOptionalComponents()
    {
        Base::enableAllOptionalComponents();
    }

    /**
     * @brief Disables all optional face components in the container.
     */
    void disableAllFaceOptionalComponents()
    {
        Base::disableAllOptionalComponents();
    }

    /**
     * @brief Enables all optional face components in the container.
     * Alias for enableAllFaceOptionalComponents().
     */
    void enableAllFaceComponents() { Base::enableAllOptionalComponents(); }

    /**
     * @brief Disables all optional face components in the container.
     * Alias for disableAllFaceOptionalComponents().
     */
    void disableAllFaceComponents() { Base::disableAllOptionalComponents(); }

    VCL_COMMON_CONTAINER_COMPONENT_METHODS(Face)
    VCL_CONTAINER_COMPONENT_METHODS(Face, AdjacentEdges, ADJACENT_EDGES)
    VCL_CONTAINER_COMPONENT_METHODS(Face, AdjacentFaces, ADJACENT_FACES)
    VCL_CONTAINER_COMPONENT_METHODS(Face, MaterialIndex, MATERIAL_INDEX)
    VCL_CONTAINER_COMPONENT_METHODS(Face, WedgeColors, WEDGE_COLORS)
    VCL_CONTAINER_COMPONENT_METHODS(Face, WedgeNormals, WEDGE_NORMALS)
    VCL_CONTAINER_COMPONENT_METHODS(Face, WedgeTexCoords, WEDGE_TEX_COORDS)

    // --- Custom Components ---

    /**
     * @brief Checks if Faces have a custom component with the given name.
     *
     * This function does not take into account the type of the custom
     * component.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @param[in] name: The name of the custom component to check.
     * @return `true` if the Face Element has a custom component with the given
     * name.
     */
    bool hasFaceCustomComponent(const std::string& name) const
        requires face::HasCustomComponents<FaceComponents>
    {
        return Base::hasElemCustomComponent(name);
    }

    /**
     * @brief Returns a vector containing all the names of the custom components
     * of any type associated to the Face Element.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @return A vector of strings representing all the names of the custom
     * components.
     */
    std::vector<std::string> faceCustomComponentNames() const
        requires face::HasCustomComponents<FaceComponents>
    {
        return Base::elemCustomComponentNames();
    }

    /**
     * @brief Checks if the custom component of the Face Element having the
     * given name has the same type of the given template argument type of this
     * function.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component to check.
     * @param[in] name: The name of the custom component to check.
     * @throws std::out_of_range if no custom component of the given name was
     * found.
     * @return `true` if the custom component is of the same type of the
     * template argument.
     */
    template<typename K>
    bool isFaceCustomComponentOfType(const std::string& name) const
        requires face::HasCustomComponents<FaceComponents>
    {
        return Base::template isElemCustomComponentOfType<K>(name);
    }

    /**
     * @brief Returns the std::type_index of the custom component of the Face
     * Element having the given input name.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @param[in] name: The name of the custom component to get the
     * std::type_index from.
     * @throws std::out_of_range if no custom component of the given name was
     * found.
     * @return The std::type_index of the custom component having the given
     * input name.
     */
    std::type_index faceCustomComponentType(const std::string& name) const
        requires face::HasCustomComponents<FaceComponents>
    {
        return Base::elemCustomComponentType(name);
    }

    /**
     * @brief Returns a vector containing all the names of the custom components
     * associated to the Face Element having the same type of the given
     * template argument type of this function.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component names.
     * @return A vector of strings representing the names of the custom
     * components of a given type.
     */
    template<typename K>
    std::vector<std::string> faceCustomComponentNamesOfType() const
        requires face::HasCustomComponents<FaceComponents>
    {
        return Base::template elemCustomComponentNamesOfType<K>();
    }

    /**
     * @brief Adds a custom component of type K to the Face, having the given
     * name.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component added to the Face.
     * @param[in] name: The name of the custom component added to the Face.
     */
    template<typename K>
    void addFaceCustomComponent(const std::string& name)
        requires face::HasCustomComponents<FaceComponents>
    {
        Base::template addElemCustomComponent<K>(name);
    }

    /**
     * @brief Deletes the custom component of the given name from the Face
     * Element.
     *
     * The function does nothing if the custom component does not exist.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @param[in] name: The name of the custom component that will be removed
     * from the Face.
     */
    void deleteFaceCustomComponent(const std::string& name)
        requires face::HasCustomComponents<FaceComponents>
    {
        Base::deleteElemCustomComponent(name);
    }

    /**
     * @brief Returns a vector handle to the custom component having the type K
     * and the given name.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component for which to return the
     * handle.
     * @param[in] name: Name of the custom component for which to return the
     * handle.
     * @return A vector handle that allows access to the custom component.
     */
    template<typename K>
    CustomComponentVectorHandle<K> faceCustomComponentVectorHandle(
        const std::string& name)
        requires face::HasCustomComponents<FaceComponents>
    {
        return Base::template customComponentVectorHandle<K>(name);
    }

    /**
     * @brief Returns a const vector handle to the custom component having type
     * K and the given name.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component for which to return the
     * handle.
     * @param[in] name: Name of the custom component for which to return the
     * handle.
     * @return A const vector handle that allows access to the custom
     * component.
     */
    template<typename K>
    ConstCustomComponentVectorHandle<K> faceCustomComponentVectorHandle(
        const std::string& name) const
        requires face::HasCustomComponents<FaceComponents>
    {
        return Base::template customComponentVectorHandle<K>(name);
    }

    /**
     * @brief Serializes all per-face custom components of type K stored in
     * this container to the given output stream.
     *
     * @tparam K: The type of the custom components to serialize.
     * @param os: The output stream to write to.
     */
    template<typename K>
    void serializeFaceCustomComponentsOfType(std::ostream& os) const
        requires face::HasCustomComponents<FaceComponents>
    {
        Base::template serializeElemCustomComponentsOfType<K>(os);
    }

    /**
     * @brief Deserializes all per-face custom components of type K from the
     * given input stream and stores them into this container.
     *
     * @tparam K: The type of the custom components to deserialize.
     * @param is: The input stream to read from.
     */
    template<typename K>
    void deserializeFaceCustomComponentsOfType(std::istream& is)
        requires face::HasCustomComponents<FaceComponents>
    {
        Base::template deserializeElemCustomComponentsOfType<K>(is);
    }

protected:
    /**
     * @brief Imports all faces from the corresponding container of the
     * other mesh into this container.
     *
     * If importing into a triangle mesh from a mesh with non-triangle faces
     * (e.g. quad or polygonal), the faces are triangulated using ear-cutting.
     *
     * @param[in] m: The other mesh to import from.
     */
    template<typename OtherMesh>
    void importFrom(const OtherMesh& m)
    {
        Base::importFrom(m);
        manageImportTriFromPoly(m);
    }

    /**
     * @brief Manages conversion from polygonal or n-gon faces to triangles
     * when importing into a triangle mesh.
     *
     * Non-triangular faces are triangulated using the ear-cutting algorithm.
     * The first triangle occupies the original face index, and additional
     * triangles are appended to the container.
     */
    template<typename OtherMesh>
    void manageImportTriFromPoly(const OtherMesh& m)
    {
        if constexpr (OtherMesh::template hasContainerOf<ElemId::FACE>()) {
            using OtherFace = OtherMesh::ConstFace;

            // if this is not a triangle mesh nor a polygon mesh (meaning that
            // we can't control the number of vertex references in this mesh),
            // and this mesh does not have the same number of vertex references
            // of the other, it means that we don't know how to convert these
            // type of meshes (e.g. we don't know how to convert a polygon mesh
            // into a quad mesh, or convert a quad mesh into a pentagonal
            // mesh...)
            static_assert(
                !(Face::VERTEX_COUNT != 3 && Face::VERTEX_COUNT > 0 &&
                  Face::VERTEX_COUNT != OtherFace::VERTEX_COUNT),
                "Cannot import from that type of Mesh. Don't know how to "
                "convert faces.");

            // we need to manage conversion from poly or faces with cardinality
            // > 3 (e.g. quads) to triangle meshes. In this case, we triangulate
            // the polygon using the earcut algorithm.
            if constexpr (
                Face::VERTEX_COUNT == 3 &&
                (OtherFace::VERTEX_COUNT > 3 || OtherFace::VERTEX_COUNT < 0)) {
                for (const auto& mf : m.faces()) {
                    // if the current face has the same number of vertices of
                    // this mf face (3), then the vertex indices have been
                    // correctly imported from the component importFrom
                    // function. The component importFrom function does
                    // nothing when importing from a face with at least 4
                    // vertices
                    if (mf.vertexCount() != 3) {
                        // triangulate mf; the first triangle of the
                        // triangulation will be this->face(mf.index()); the
                        // other triangles will be added at the end of the
                        // container

                        std::vector<uint> tris =
                            vcl::earCut(mf.vertices() | vcl::views::positions);

                        if (tris.size() >= 3) {
                            Face f = face(mf.index());
                            importTriCorners(f, mf, tris, 0);

                            // number of other faces to add
                            uint  nf      = tris.size() / 3 - 1;
                            Index baseFid = addFaces(nf);

                            uint t = 3; // index that cycles into tris
                            for (uint k = 0; k < nf; ++k, t += 3) {
                                Face fk = face(baseFid + k);
                                fk.importFrom(mf, false);
                                importTriCorners(fk, mf, tris, t);
                            }
                        }
                        else {
                            this->deleteFace(mf.index());
                        }
                    }
                }
            }
        }
    }

private:
    template<std::unsigned_integral OtherIndex>
    static constexpr Index castIndex(OtherIndex idx) noexcept
    {
        if constexpr (std::same_as<Index, OtherIndex>) {
            return idx;
        }
        else {
            return idx == vcl::NULL_INDEX<OtherIndex> ? NULL_INDEX :
                                                        static_cast<Index>(idx);
        }
    }

    template<typename OtherFace>
    static void importTriCorners(
        Face&                    f,
        const OtherFace&         mf,
        const std::vector<uint>& tris,
        uint                     basetri)
    {
        for (uint j = 0; j < 3; ++j) {
            f.setVertex(j, castIndex(mf.vertexIndex(tris[basetri + j])));
        }

        // Wedge Colors
        if constexpr (
            comp::HasWedgeColors<Face> && comp::HasWedgeColors<OtherFace>) {
            if (f.template isComponentAvailable<CompId::WEDGE_COLORS>() &&
                mf.template isComponentAvailable<CompId::WEDGE_COLORS>()) {
                for (uint j = 0; j < 3; ++j) {
                    f.wedge(j).color() = mf.wedge(tris[basetri + j]).color();
                }
            }
        }

        // Wedge TexCoords
        if constexpr (
            comp::HasWedgeTexCoords<Face> &&
            comp::HasWedgeTexCoords<OtherFace>) {
            if (f.template isComponentAvailable<CompId::WEDGE_TEX_COORDS>() &&
                mf.template isComponentAvailable<CompId::WEDGE_TEX_COORDS>()) {
                using S = typename std::remove_cvref_t<
                    decltype(f.wedge(0).texCoord())>::ScalarType;
                for (uint j = 0; j < 3; ++j) {
                    f.wedge(j).texCoord() = mf.wedge(tris[basetri + j])
                                                .texCoord()
                                                .template cast<S>();
                }
            }
        }

        // Wedge Normals
        if constexpr (
            comp::HasWedgeNormals<Face> && comp::HasWedgeNormals<OtherFace>) {
            if (f.template isComponentAvailable<CompId::WEDGE_NORMALS>() &&
                mf.template isComponentAvailable<CompId::WEDGE_NORMALS>()) {
                using S = typename std::remove_cvref_t<
                    decltype(f.wedge(0).normal())>::ScalarType;
                for (uint j = 0; j < 3; ++j) {
                    f.wedge(j).normal() =
                        mf.wedge(tris[basetri + j]).normal().template cast<S>();
                }
            }
        }
    }
};

} // namespace vcl::mesh

#endif // VCL_MESH_CONTAINERS_FACE_CONTAINER_H
