/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MESH_MESH_H
#define VCL_MESH_MESH_H

#include <vclib/concepts/mesh.h>

#include "mesh_components.h"
#include "mesh_containers.h"

namespace vcl {

/**
 * @defgroup mesh Mesh
 *
 * @brief List of classes, aliases, concepts and functions used for the
 * creation, customization, usage and manipulation of Mesh classes.
 */

/**
 * @defgroup components Components
 * @ingroup mesh
 *
 * @brief List of all the Component classes, along with their concepts and
 * functions.
 */

/**
 * @brief The Mesh class represents a generic 3D mesh. A mesh is composed of a
 * generic number of containers of Elements (which can be vertices, faces,
 * edges...), plus some other components.
 *
 * The Mesh class will expose all the public members of its containers and is
 * components, and its role is to implement all the functionalities that allow
 * these containers to comunicate (e.g. an operation on the vertex container
 * that requires to update also some face information).
 *
 * Therefore, in this page are documented only the functions that in some way
 * need to modify the status of more than one Container of the Mesh. Other
 * functions are inherited by the Container classes of the Elements of the Mesh,
 * or from its Components, and depend on all the templates that compose a
 * specific Mesh definition.
 *
 * @ingroup mesh
 */
template<typename... Args>
class Mesh : public Args...
{
    static_assert(HasVertices<Args...>, "A Mesh must have a VertexContainer.");

    template<typename El, bool b>
    friend struct comp::detail::CustomComponentsData;

    template<typename El, bool b>
    friend struct comp::detail::ComponentData;

    template<ElementConcept T>
    friend class mesh::ElementContainer;

    template<uint ELEM_ID, typename MeshType, typename... Comps>
    friend class Element;

    // Predicate structures

    template<uint ELEM_ID>
    struct ContainerOfElement :
            public mesh::ContainerOfElement<ELEM_ID, Mesh<Args...>>
    {
    };

    /**
     * @brief The ContainerOf struct allows to get the Container of an Element
     * on this Mesh.
     *
     * Usage:
     *
     * ```cpp
     * using Container = ContainerOf<ElementType>::type;
     * ```
     */
    template<ElementConcept El>
    struct ContainerOf : public ContainerOfElement<El::ELEMENT_ID>
    {
    };

public:
    /**
     * @brief Containers is a vcl::TypeWrapper that wraps all the types from
     * which the Mesh inherits (Args) that are ElementContainers (they satisfy
     * the ElementContainerConcept).
     */
    using Containers = vcl::FilterTypesByCondition<
        mesh::IsElementContainerPred,
        vcl::TypeWrapper<Args...>>::type;

    /**
     * @brief Components is an alias to a vcl::TypeWrapper that wraps all the
     * types from which the Mesh inherits (Args) that are Components (they
     * satisfy the ComponentConcept).
     */
    using Components = vcl::FilterTypesByCondition<
        comp::IsComponentPred,
        vcl::TypeWrapper<Args...>>::type;

    /**
     * @brief ElementType is an alias that exposes the type of the Element
     * identified by the template parameter ELEM_ID.
     *
     * To be used, the Mesh must have an ElementContainer having ID ELEM_ID.
     *
     * Usage:
     * ```cpp
     * using VertexType = MeshType::template ElementType<ElemId::VERTEX>;
     * ```
     */
    template<uint ELEM_ID>
    using ElementType = ContainerOfElement<ELEM_ID>::type::ElementType;

    /* Constexpr static member functions */

    /**
     * @brief Returns true if this Mesh has a container of elements having the
     *same Element ID of the template Element El.
     *
     * This means that this the only value checked is the ELEMENT_ID unsigned
     *int exposed by the Element, meaning that it does not check if the Elements
     *of this mesh are exactly the same of El.
     *
     * In other words, it returns true also if we pass an Element of another
     *mesh that is of the same ELEMENT_ID (both Vertices, Faces, ecc).
     *
     * Example of usage (Note: EdgeMesh has Vertices, but not Faces):
     *
     * @code{.cpp}
     * static_assert(vcl::EdgeMesh::hasContainerOf<vcl::TriMesh::Vertex>(),
     *                    "EdgeMesh does not have Vertices");
     * static_assert(!vcl::EdgeMesh::hasContainerOf<vcl::TriMesh::Face>(),
     *                    "EdgeMesh has Faces");
     * @endcode
     *
     * @tparam El: the Element type to check.
     * @return true if this Mesh has a container of elements having the same
     * Element ID of the template Element El.
     */
    template<ElementConcept El>
    static constexpr bool hasContainerOf()
    {
        return mesh::HasContainerOfPred<El, Mesh<Args...>>::value;
    }

    /**
     * @brief Returns true if this Mesh has a container of elements having the
     *same Element ID of the template ELEM_ID.
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
    template<uint ELEM_ID>
    static constexpr bool hasContainerOf()
    {
        return mesh::HasContainerOfElementPred<ELEM_ID, Mesh<Args...>>::value;
    }

    /**
     * @brief Returns true if this Mesh has a container of elements having the
     * same Element ID of the template ELEM_ID and the Element of that container
     * has a Component having the same Component ID of the template COMP_ID.
     *
     * Example of usage (Note: TriMesh has per Vertex TexCoords, but not adj
     * edges):
     *
     * @code{.cpp}
     * static_assert(vcl::TriMesh::hasPerElementComponent<
     *         vcl::ElemId::VERTEX, vcl::TEX_COORDS>(),
     *         "TriMesh does not have per Vertex TexCoords");
     *
     * static_assert(!vcl::TriMesh::hasPerElementComponent<
     *        vcl::ElemId::VERTEX, vcl::ADJ_EDGES>(),
     *        "TriMesh has per Vertex Adjacent Edges");
     * @endcode
     */
    template<uint ELEM_ID, uint COMP_ID>
    static constexpr bool hasPerElementComponent()
    {
        return mesh::HasPerElementComponent<Mesh<Args...>, ELEM_ID, COMP_ID>;
    }

    /**
     * @brief Returns true if this Mesh has a container of elements having the
     * same Element ID of the template ELEM_ID and the Element of that container
     * has an Optional Component having the same Component ID of the template
     * COMP_ID.
     *
     * Example of usage (Note: TriMesh has per Vertex optional TexCoords, and
     * non-optional Normals):
     *
     * @code{.cpp}
     * static_assert(vcl::TriMesh::hasPerElementOptionalComponent<
     *         vcl::ElemId::VERTEX, vcl::TEX_COORDS>(),
     *         "TriMesh does not have per Vertex Optional TexCoords");
     *
     * static_assert(!vcl::TriMesh::hasPerElementOptionalComponent<
     *        vcl::ElemId::VERTEX, vcl::NORMALS>(),
     *        "TriMesh has per Vertex Optional Normals");
     * @endcode
     */
    template<uint ELEM_ID, uint COMP_ID>
    static constexpr bool hasPerElementOptionalComponent()
    {
        return mesh::
            HasPerElementOptionalComponent<Mesh<Args...>, ELEM_ID, COMP_ID>;
    }

    /* Constructors */

    /**
     * @brief Empty constructor, constructs an empty mesh.
     */
    Mesh()
    {
        // Set to all element containers their parent mesh (this)
        updateAllParentMeshPointers();
    }

    /**
     * @brief Copy constructor of the Mesh. Will create a deep copy of the given
     * input mesh, taking care of copying everithing and then update all the
     * pointers
     *
     * @param oth: the mesh from which constructo this Mesh.
     */
    Mesh(const Mesh& oth) :
            Args(oth)... // call auto copy constructors for all the container
                         // elements and components
    {
        // Set to all element containers their parent mesh (this)
        updateAllParentMeshPointers();

        // for each container of oth, save its base pointer
        // will need it to update all the pointers of this mesh
        constexpr uint N_CONTAINERS =
            NumberOfTypes<typename Mesh<Args...>::Containers>::value;
        std::array<const void*, N_CONTAINERS> othBases =
            Mesh<Args...>::getContainerBases(oth);

        // update all the pointers contained on each container
        // use the base pointer of each container of oth to update all the
        // pointers in this mesh. Each pointer of oth that was copied in this
        // mesh, will be updated computing its offset wrt the base of oth, and
        // then adding that offset to the new base pointer of this mesh
        (updateReferencesOfContainerType<Args>(*this, othBases), ...);
    }

    /**
     * @brief Move constructor, moves the given mesh into this one, without any
     * other resource acquisition.
     *
     * @param oth: the mesh that will be moved into this Mesh.
     */
    Mesh(Mesh&& oth)
    {
        swap(oth); // use copy and swap idiom: this (empty) mesh is swapped with
                   // the input one
    }

    /* Member functions */

    /**
     * @brief Returns true if this mesh is compact, meaning that all its
     * containers have no deleted elements (size == element number).
     *
     * @return true if this mesh is compact, false otherwise.
     */
    bool isCompact() const
    {
        bool isCompact = true;
        ((isCompact &= isContainerCompact<Args>()), ...);
        return isCompact;
    }

    /**
     * @brief Clears all the Elements contained in the mesh.
     * @todo manage also other components
     */
    void clear() { (clearContainer<Args>(), ...); }

    /**
     * @brief Compacts all the containers of the mesh.
     *
     * Removes all the deleted elements from each container, compacting the
     * the containers and then updating automatically all the pointers/indices.
     */
    void compact() { (compactContainer<Args>(), ...); }

    /**
     * @brief Enables all the optional components of the elements of the
     * containers if the mesh.
     */
    void enableAllOptionalComponents()
    {
        (enableAllOptionalComponentsInContainer<Args>(), ...);
    }

    /**
     * @brief Disables all the optional components of the elements of the
     * containers if the mesh.
     */
    void disableAllOptionalComponents()
    {
        (disableAllOptionalComponentsInContainer<Args>(), ...);
    }

    /**
     * @brief Enables all the OptionalComponents of this mesh according to the
     * Components available on the OtherMeshType m.
     *
     * This function is useful to call before importing data from another
     * MeshType, to be sure that all the available data contained in the
     * MeshType mesh will be imported.
     *
     * This function:
     * - disables all the optional components that are not available in m
     * - enables all the optional components that are available in m (which can
     *   be both optional or not)
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
     * @param m
     */
    template<typename OtherMeshType>
    void enableSameOptionalComponentsOf(const OtherMeshType& m)
    {
        // enable all optional components of this Mesh depending on what's
        // available in the OtherMeshType

        (enableSameOptionalComponentsOf<Args>(m), ...);
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
     * @param m
     */
    void append(const Mesh& m)
    {
        // for each container of this mesh, save its size
        // will need it to update all the pointers/indices of this mesh
        constexpr uint N_CONTAINERS =
            NumberOfTypes<typename Mesh<Args...>::Containers>::value;
        std::array<std::size_t, N_CONTAINERS> sizes =
            Mesh<Args...>::getContainerSizes(*this);

        // for each container of the other mesh m, save its bases
        // will need it to update all the pointers of this mesh
        std::array<const void*, N_CONTAINERS> bases =
            Mesh<Args...>::getContainerBases(m);

        // call the append function for each container
        (appendContainer<Args>(m), ...);

        // update all the pointers/indices contained on each container
        (updateReferencesOfContainerTypeAfterAppend<Args>(*this, bases, sizes),
         ...);
    }

    /**
     * @brief Imports all the components that can be imported from another type
     * of mesh.
     *
     * This function can be called from any Mesh type having all the Elements
     * and Components that implement the member function importFrom.
     *
     * Note that this function does not enable optional components that are
     * disabled. If you want to import all the possible data including also
     * disabled components of this mesh, you should call the function
     * m1.enableSameOptionalComponentsOf(m2) before this function.
     *
     * @param[in] m: the mesh from which import all the data.
     */
    template<typename OtherMeshType>
    void importFrom(const OtherMeshType& m)
    {
        // This function will first:
        // Call, for each Container and Component of the mesh, its importFrom
        // function. In case of containers, it first creates the same number of
        // elements in the container, and then calls the importFrom function for
        // each new element.

        (Args::importFrom(m), ...);

        // Set to all element containers their parent mesh (this)
        updateAllParentMeshPointers();

        if constexpr (mesh::HasFaceContainer<Mesh<Args...>>) {
            // Now I need to manage imports between different types of meshes
            // (same type of meshes are already managed from importFrom member
            // function).
            //
            // Generally speaking, Polygon meshes can import from any other type
            // of mesh. We need to take care when this mesh has static vertex
            // references number in the face container (VERTEX_NUMBER >= 3).
            //
            // The follwing case don't need to be managed:
            // - import polygon mesh from triangle mesh
            //
            // I can manage the following cases:
            // - import triangle mesh from polygon mesh: need triangulation
            //
            // I cannot manage the follwing cases:
            // - import static non-triangle mesh from polygon mesh or from a
            //   mesh with different VERTEX_NUMBER

            // in case of import from poly to triangle mesh, I need to manage
            // triangulation of polygons and create additional triangle faces
            // for each of the imported polygons. This function statically
            // asserts that the import can be done.
            using FaceContainer = typename Mesh<Args...>::FaceContainer;
            FaceContainer::manageImportTriFromPoly(m);
        }
    }

    /**
     * @brief Swaps this mesh with the other input Mesh m2.
     * @param m2: the Mesh to swap with this Mesh.
     */
    void swap(Mesh& m2)
    {
        Mesh<Args...>& m1 = *this;

        constexpr uint N_CONTAINERS =
            NumberOfTypes<typename Mesh<Args...>::Containers>::value;
        static_assert(N_CONTAINERS != 0);

        // container bases of each container for m1 and m2
        // we save the bases of the containers before swap
        std::array<const void*, N_CONTAINERS> m1Bases =
            Mesh<Args...>::getContainerBases(m1);
        std::array<const void*, N_CONTAINERS> m2Bases =
            Mesh<Args...>::getContainerBases(m2);

        // actual swap of all the containers and the components of the mesh
        // using pack expansion: swap will be called for each of the containers
        // (or components!) that compose the Mesh
        using std::swap;
        (swap((Args&) m1, (Args&) m2), ...);

        // Set to all elements their parent mesh
        m1.updateAllParentMeshPointers();
        m2.updateAllParentMeshPointers();

        // update all the refs to m1 and m2: old base of m1 is now "old
        // base" of m2, and viceversa
        (updateReferencesOfContainerType<Args>(m1, m2Bases), ...);
        (updateReferencesOfContainerType<Args>(m2, m1Bases), ...);
    }

    /**
     * @brief Assignment operator of the Mesh.
     * @param oth: the Mesh from which will create a copy and assign to this
     * Mesh
     * @return a reference to this Mesh after the assignemnt.
     */
    Mesh& operator=(Mesh oth)
    {
        swap(oth);
        return *this;
    }

    /*** Generic Element functions ***/

    /**
     * @brief Returns the index of the given element in its Container of the
     * Mesh.
     *
     * The function requires that the Mesh has a Container of Elements of type
     * El. Otherwise, a compiler error will be triggered.
     *
     * @tparam El: The type of the Element, it must satisfy the ElementConcept.
     * @param e: a reference of an element of the Mesh.
     * @return the index of the given element.
     */
    template<ElementConcept El>
    uint index(const El& e) const requires (hasContainerOf<El>())
    {
        using Container = ContainerOf<El>::type;
        return Container::index(&e);
    }

    /**
     * @brief Returns the index of the given element in its Container of the
     * Mesh.
     *
     * The function requires that the Mesh has a Container of Elements of type
     * El. Otherwise, a compiler error will be triggered.
     *
     * @tparam El: The type of the Element, it must satisfy the ElementConcept.
     * @param e: a pointer to an element of the Mesh.
     * @return the index of the given element.
     */
    template<ElementConcept El>
    uint index(const El* e) const requires (hasContainerOf<El>())
    {
        using Container = ContainerOf<El>::type;
        return Container::index(e);
    }

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
    template<uint ELEM_ID>
    const auto& element(uint i) const requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::element(i);
    }

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
    template<uint ELEM_ID>
    auto& element(uint i) requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::element(i);
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
    template<uint ELEM_ID>
    uint number() const requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::elementNumber();
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
    template<uint ELEM_ID>
    uint containerSize() const requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::elementContainerSize();
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
    template<uint ELEM_ID>
    uint deletedNumber() const requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::deletedElementNumber();
    }

    /**
     * @brief Adds a new element of the given type into its container, returning
     * the index of the added element in its container.
     *
     * If the call of this function will cause a reallocation of the container,
     * the function will automatically take care of updating all the pointers to
     * the elements stored in all the containers of the Mesh.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @return the index of the added element in its container.
     */
    template<uint ELEM_ID>
    uint add() requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::addElement();
    }

    /**
     * @brief Adds `n` new elements of the given type into its container,
     * returning the index of the first added element in its container.
     *
     * If the call of this function will cause a reallocation of the container,
     * the function will automatically take care of updating all the pointers to
     * the elements stored in all the containers of the Mesh.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @param[in] n: the number of elements to add.
     * @return the index of the first added element in its container.
     */
    template<uint ELEM_ID>
    uint add(uint n) requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::addElements(n); // add the number elements
    }

    /**
     * @brief Clears the container of ELEM_ID elements the Mesh, deleting all
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
     * @warning Any pointer to ELEM_ID elements in the Mesh will be left
     * unchanged, and therefore will point to invalid elements. This means that,
     * if you have a pointer to a ELEM_ID element and you call this function,
     * you will have a dangling pointer.
     *
     * @tparam ELEM_ID: the ID of the element.
     */
    template<uint ELEM_ID>
    void clearElements() requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        Cont::clearElements();
    }

    /**
     * @brief Resizes the Element container to contain `n` Elements of type
     * ELEM_ID.
     *
     * If the new size is greater than the old one, new Elements are added to
     * the container, and a reallocation may happen. If the new size is smaller
     * than the old one, the container will keep its first non-deleted `n`
     * Elements, and the remaining Elements are marked as deleted.
     *
     * If the call of this function will cause a reallocation of the container,
     * the function will automatically take care of updating all the pointers to
     * the elements stored in all the containers of the Mesh.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @warning The given size `n` is relative to the number of non-deleted
     * Elements, not to the size of the Element container. For example, if you
     * have a mesh with 10 Faces and faceContainerSize() == 20, calling
     * resize<ElemId::FACE>(5) will not cause a reallocation of the container,
     * but will mark as deleted the least 5 non-deleted Faces of the container.
     * In the same scenario, calling resize<ElemId::FACE>(15) will result in a
     * Face container having 15 new Faces and faceContainerSize() == 25. The
     * latest 5 Faces will be the newly added.
     *
     * @warning Any pointer to deleted Elements in the Mesh will be left
     * unchanged, and therefore will point to invalid Elements. This means that
     * if you call this member function with a lower number of Elements, you'll
     * need to manually manage the pointers/indices to the deleted Elements.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @param[in] n: the new size of the container in the mesh.
     */
    template<uint ELEM_ID>
    void resize(uint n) requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        Cont::resizeElements(n);
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
     * If the call of this function will cause a reallocation of the container,
     * the function will automatically take care of updating all the pointers to
     * the elements stored in all the containers of the Mesh.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the type ID of the element.
     * @param[in] n: the new capacity of the container in the mesh.
     */
    template<uint ELEM_ID>
    void reserve(uint n) requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        Cont::reserveElements(n);
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
    template<uint ELEM_ID>
    void deleteElement(uint i) requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        Cont::deleteElement(i);
    }

    /**
     * @brief Marks as deleted the given element from its container.
     *
     * The function does not remove the element from the container, and
     * therefore it does not cause reallocation or compacting of the container.
     * The element will be removed from the container only when the container
     * will be compacted.
     *
     * The complexity of this function is O(1).
     *
     * The function requires that the Mesh has a Container of Elements of type
     * of the argument. Otherwise, a compiler error will be triggered.
     *
     * @tparam El: the type of the element to delete. It must satisfy the
     * ElementConcept.
     * @param[in] e: a pointer to the element to delete.
     */
    template<ElementConcept El>
    void deleteElement(const El* e) const requires (hasContainerOf<El>())
    {
        using Cont = ContainerOf<El>::type;
        return Cont::deleteElement(e);
    }

    /**
     * @brief Marks as deleted the given element from its container.
     *
     * The function does not remove the element from the container, and
     * therefore it does not cause reallocation or compacting of the container.
     * The element will be removed from the container only when the container
     * will be compacted.
     *
     * The complexity of this function is O(1).
     *
     * The function requires that the Mesh has a Container of Elements of type
     * of the argument. Otherwise, a compiler error will be triggered.
     *
     * @tparam El: the type of the element to delete. It must satisfy the
     * ElementConcept.
     * @param[in] e: the element to delete.
     */
    template<ElementConcept El>
    void deleteElement(const El& e) const requires (hasContainerOf<El>())
    {
        using Cont = ContainerOf<El>::type;
        return Cont::deleteElement(&e);
    }

    /**
     * @brief Returns a vector that tells, for each element of the container of
     * ELEM_ID in the mesh, the new index of the element after the container has
     * been compacted. For each deleted element, its position will be set to
     * UINT_NULL.
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
    template<uint ELEM_ID>
    std::vector<uint> conpactIndices() const
        requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::elementConpactIndices();
    }

    /**
     * @brief Updates all the indices and pointers of the elements of the
     * container of ELEM_ID in the mesh, according to the mapping stored in the
     * newIndices vector, that tells for each old element index, the new index
     * of the element in the same container (or UINT_NULL if the element must be
     * left as unreferenced - useful when a vertex is deleted).
     *
     * This function is useful when you move some vertices (e.g. it is
     * automatically called after every compacting of a container), and you want
     * to update the indices/pointers of the elements stored in all the
     * containers of the mesh.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @note This function *does not change the position of the elements in
     * their container*. It just updates the indices/pointers of the elements
     * stored in their or other containers. This function should be called after
     * the elements have been actually moved in their container.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @param[in] newIndices: a vector that tells, for each old element index,
     * the new element index. If the old element must be left as unreferenced
     * (setting `nullptr` to the pointers), the value of the vector must be
     * UINT_NULL.
     */
    template<uint ELEM_ID>
    void updateIndices(const std::vector<uint>& newIndices)
        requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::updateElementIndices(newIndices);
    }

    void serialize(std::ostream& os) const
    {
        auto serializeOptionalComponentsAndElementsNumber =
            [this, &os]<typename Cont>() {
                Cont::serializeOptionalComponentsAndElementsNumber(os);
            };

        vcl::ForEachType<Containers>::apply(
            serializeOptionalComponentsAndElementsNumber);

        auto serializeElements = [this, &os]<typename Cont>() {
            Cont::serializeElements(os);
        };

        vcl::ForEachType<Containers>::apply(serializeElements);

        auto serializeComponents = [this, &os]<typename Comp>() {
            Comp::serialize(os);
        };

        vcl::ForEachType<Components>::apply(serializeComponents);
    }

    void deserialize(std::istream& is)
    {
        // First - deserialize the number of elements for each container,
        // and whether the optional components of the elements are enabled or
        // not.

        auto deserializeOptionalComponentsAndElementsNumber =
            [this, &is]<typename Cont>() {
                Cont::deserializeOptionalComponentsAndElementsNumber(is);
            };

        vcl::ForEachType<Containers>::apply(
            deserializeOptionalComponentsAndElementsNumber);

        // Now, I can actually deserialize all the elements and their components
        // safely (e.g. I could not load adjacent faces from the vertex
        // container if the face container is not resized yet).

        auto deserializeElements = [this, &is]<typename Cont>() {
            Cont::deserializeElements(is);
        };

        vcl::ForEachType<Containers>::apply(deserializeElements);

        // Deserialize the mesh components (e.g. bounding box, transform matrix)

        auto deserializeComponents = [this, &is]<typename Comp>() {
            Comp::deserialize(is);
        };

        vcl::ForEachType<Components>::apply(deserializeComponents);
    }

    /**
     * @brief Returns an iterator to the begining of the container of the
     * elements having ID ELEM_ID in the mesh.
     *
     * The iterator is automatically initialized to jump deleted elements in the
     * container. You can change this option by calling this member function
     * with the `jumpDeleted` parameter set to `false`.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @param[in] jumpDeleted: if `true`, the iterator will be initialized to
     * jump deleted elements in the container.
     * @return an iterator to the begining of the container of the elements of
     * type ELEM_ID in the mesh.
     */
    template<uint ELEM_ID>
    auto begin(bool jumpDeleted = true) requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::elementBegin(jumpDeleted);
    }

    /**
     * @brief Returns an iterator to the end of the container of the elements
     * having ID ELEM_ID in the mesh.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @return an iterator to the end of the container of the elements of type
     * ELEM_ID in the mesh.
     */
    template<uint ELEM_ID>
    auto end() requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::elementEnd();
    }

    /**
     * @brief Returns a const iterator to the begining of the container of the
     * elements having ID ELEM_ID in the mesh.
     *
     * The iterator is automatically initialized to jump deleted elements in the
     * container. You can change this option by calling this member function
     * with the `jumpDeleted` parameter set to `false`.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @param[in] jumpDeleted: if `true`, the iterator will be initialized to
     * jump deleted elements in the container.
     * @return a const iterator to the begining of the container of the elements
     * of type ELEM_ID in the mesh.
     */
    template<uint ELEM_ID>
    auto begin(bool jumpDeleted = true) const
        requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::elementBegin(jumpDeleted);
    }

    /**
     * @brief Returns a const iterator to the end of the container of the
     * elements having ID ELEM_ID in the mesh.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @return a const iterator to the end of the container of the elements of
     * type ELEM_ID in the mesh.
     */
    template<uint ELEM_ID>
    auto end() const requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::elementEnd();
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
    template<uint ELEM_ID>
    auto elements(bool jumpDeleted = true) requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::elements(jumpDeleted);
    }

    /**
     * Returns a lightweight const view object that stores the begin and end
     * const iterators of the container of the elements having ID ELEM_ID in the
     * mesh. The view object exposes the iterators trough the `begin()` and
     * `end()` member functions, and therefore the returned object can be used
     * in range-based for loops:
     *
     * @code{.cpp}
     * for (const auto& el : mesh.elements<ElemId::VERTEX>()) {
     *     // Do something read-only with el
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
    template<uint ELEM_ID>
    auto elements(bool jumpDeleted = true) const
        requires (hasContainerOf<ELEM_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::elements(jumpDeleted);
    }

    /**
     * @brief Returns `true` if optional Component having ID `COMP_ID` is
     * enabled for elements having ID `ELEM_ID` in the mesh.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID, and that the Element has a optional component having ID COMP_ID.
     * Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @tparam COMP_ID: the ID of the component.
     * @return `true` if optional Component `COMP_ID` is enabled for elements of
     * type `ELEM_ID` in the mesh.
     */
    template<uint ELEM_ID, uint COMP_ID>
    bool isPerElementComponentEnabled() const
        requires (hasPerElementOptionalComponent<ELEM_ID, COMP_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::template isOptionalComponentEnabled<COMP_ID>();
    }

    /**
     * @brief Enables the optional Component having ID `COMP_ID` for elements
     * having ID `ELEM_ID` in the mesh.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID, and that the Element has a optional component having ID
     * COMP_TYPE. Otherwise, a compiler error will be triggered.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @tparam COMP_ID: the ID of the component.
     */
    template<uint ELEM_ID, uint COMP_ID>
    void enablePerElementComponent()
        requires (hasPerElementOptionalComponent<ELEM_ID, COMP_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        Cont::template enableOptionalComponent<COMP_ID>();
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
    template<uint ELEM_ID, uint COMP_ID>
    void disablePerElementComponent()
        requires (hasPerElementOptionalComponent<ELEM_ID, COMP_ID>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        Cont::template disableOptionalComponent<COMP_ID>();
    }

    /**
     * @brief Returns `true` if the Element having ID `ELEM_ID` has a custom
     * component with the given name.
     *
     * This function does not take into account the type ot the custom
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
    template<uint ELEM_ID>
    bool hasPerElementCustomComponent(const std::string& name) const
        requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::template hasElemCustomComponent(name);
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
    template<uint ELEM_ID>
    std::vector<std::string> perElementCustomComponentNames() const
        requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::template elemCustomComponentNames();
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
    template<uint ELEM_ID, typename K>
    bool isPerElementCustomComponentOfType(const std::string& name) const
        requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::template isElemCustomComponentOfType<K>(name);
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
    template<uint ELEM_ID>
    std::type_index perElementCustomComponentType(const std::string& name) const
        requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::template elemCustomComponentType(name);
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
    template<uint ELEM_ID, typename K>
    std::vector<std::string> perElementCustomComponentNamesOfType() const
        requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::template elemCustomComponentNamesOfType<K>();
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
    template<uint ELEM_ID, typename K>
    void addPerElementCustomComponent(const std::string& name)
        requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        Cont::template addElemCustomComponent<K>(name);
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
    template<uint ELEM_ID>
    void deletePerElementCustomComponent(const std::string& name)
        requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        Cont::template deleteElemCustomComponent(name);
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
     * auto handle = m.perElementCustomComponentVectorHandle<
     *      ElemId::VERTEX, int>("cc");
     * for (Vertex& v : m.vertices() {
     *    handle[m.index(v)] = 5; // v.customComponent<int>("cc") == 5
     *    assert(v.customComponent<int>("cc") == 5);
     * }
     * @endcode
     *
     * Using handles allows to access more efficiently to custom components
     * rather accessing from an element object. However, note that references
     * are binded to the container of the mesh.
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
    template<uint ELEM_ID, typename K>
    CustomComponentVectorHandle<K> perElementCustomComponentVectorHandle(
        const std::string& name)
        requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::template customComponentVectorHandle<K>(name);
    }

    /**
     * @brief Returns a const vector handle to the custom component of type `K`
     * having the given name associated to the Element having ID `ELEM_ID`.
     *
     * The function requires that the Mesh has a Container of Elements having ID
     * ELEM_ID, and that the Element has CustomComponents.
     * Otherwise, a compiler error will be triggered.
     *
     * The handle can be used like a normal std::vector, but does not have
     * access to the modifiers member functions (resize, push_back...). The
     * handle contains **const references** to the custom component, therefore
     * you can access to the custom component by accessing the element of the
     * handle vector normally. Since the handle stores references, there are no
     * copies performed when calling this function.
     *
     * For example, assuming that the mesh has a vertex custom component named
     * "cc" of type int:
     *
     * @code{.cpp}
     * auto handle = m.perElementCustomComponentVectorHandle<
     *      ElemId::VERTEX, int>("cc");
     * for (const Vertex& v : m.vertices() {
     *     sum += handle[m.index(v)];
     *     // handle[m.index(v)] = 5; // not allowed, const handle
     * }
     * @endcode
     *
     * Using handles allows to access more efficiently to custom components
     * rather accessing from an element object. However, note that references
     * are binded to the container of the mesh.
     *
     * @note Since the handle contains references, any operation that changes
     * the size of the container could be destructive and invalidate the
     * references contained in the handle.
     *
     * @tparam ELEM_ID: the ID of the element.
     * @tparam K: the type of the custom component.
     * @param[in] name: the name of the custom component.
     * @return A const handle to the custom component of type `K` having the
     * given name associated to the Element having ID `ELEM_ID`.
     */
    template<uint ELEM_ID, typename K>
    ConstCustomComponentVectorHandle<K> perElementCustomComponentVectorHandle(
        const std::string& name) const
        requires (hasPerElementComponent<ELEM_ID, CompId::CUSTOM_COMPONENTS>())
    {
        using Cont = ContainerOfElement<ELEM_ID>::type;

        return Cont::template customComponentVectorHandle<K>(name);
    }

protected:
    template<typename Cont>
    bool isContainerCompact() const
    {
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            return Cont::elementNumber() == Cont::elementContainerSize();
        }
        else {
            return true; // does not count as a container
        }
    }

    /**
     * This function will compact the Cont container of this mesh (if Cont is
     * actually a container).
     *
     * This function is made to be called trough pack expansion:
     * @code{.cpp}
     * (compactContainer<Args>(), ...);
     * @endcode
     */
    template<typename Cont>
    void compactContainer()
    {
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            if (Cont::elementNumber() != Cont::elementContainerSize()) {
                Cont::compactElements();
            }
        }
    }

    /**
     * This function will enable all the optional components of the element of
     * the Cont container of this mesh (if Cont is actually a container).
     *
     * This function is made to be called trough pack expansion:
     * @code{.cpp}
     * (enableAllOptionalComponentsInContainer<Args>(), ...);
     * @endcode
     */
    template<typename Cont>
    void enableAllOptionalComponentsInContainer()
    {
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            Cont::enableAllOptionalComponents();
        }
    }

    /**
     * This function will disable all the optional components of the element of
     * the Cont container of this mesh (if Cont is actually a container).
     *
     * This function is made to be called trough pack expansion:
     * @code{.cpp}
     * (disableAllOptionalComponentsInContainer<Args>(), ...);
     * @endcode
     */
    template<typename Cont>
    void disableAllOptionalComponentsInContainer()
    {
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            Cont::disableAllOptionalComponents();
        }
    }

    /**
     * This function will clear the Cont container of this mesh (if Cont is
     * actually a container).
     *
     * This function is made to be called trough pack expansion:
     * @code{.cpp}
     * (clearContainer<Args>(), ...);
     * @endcode
     */
    template<typename Cont>
    void clearContainer()
    {
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            Cont::clearElements();
        }
    }

    template<ElementConcept Element>
    void updateAllReferences(const Element* oldBase)
    {
        if (oldBase != nullptr)
            (updateReferences<Args>(oldBase), ...);
    }

    // this function is required in order to get msvc compile
    template<typename Element, typename... A>
    void updateReferences(const Element* oldBase, TypeWrapper<A...>)
    {
        updateReferences(
            oldBase, TypeWrapper<A...>(), std::array<std::size_t, 0>(), 0);
    }

    // this function is required in order to get msvc compile
    template<typename Cont, typename Element>
    void updateReferences(const Element* oldBase)
    {
        updateReferences<Cont>(oldBase, std::array<std::size_t, 0>(), 0);
    }

    // this additional function is necessary because otherwise msvc jumps
    // totally the pack expansion if called directly in the function
    // updateReferencesOfContainerTypeAfterAppend
    template<typename Element, std::size_t N, typename... A>
    void updateReferences(
        const Element* oldBase,
        TypeWrapper<A...>,
        const std::array<std::size_t, N>& sizes  = std::array<std::size_t, 0>(),
        uint                              offset = 0)
    {
        (updateReferences<A>(oldBase, sizes, offset), ...);
    }

    template<typename Cont, typename Element, std::size_t N>
    void updateReferences(
        const Element*                    oldBase,
        const std::array<std::size_t, N>& sizes  = std::array<std::size_t, 0>(),
        uint                              offset = 0)
    {
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            if constexpr (N > 0) {
                using Containers = Mesh<Args...>::Containers;
                constexpr uint I = IndexInTypes<Cont, Containers>::value;
                static_assert(I >= 0 && I != UINT_NULL);
                Cont::updateReferences(oldBase, sizes[I], offset);
            }
            else {
                Cont::updateReferences(oldBase);
            }
        }
    }

    template<ElementConcept Element>
    void updateAllReferences(const std::vector<uint>& newIndices)
    {
        (updateReferences<Args, Element>(newIndices), ...);
    }

    template<typename Cont, typename Element>
    void updateReferences(const std::vector<uint>& newIndices)
    {
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            Cont::template updateReferences<Element>(newIndices);
        }
    }

private:
    // hide init and isAvailable members
    void init() {};

    bool isAvailable() const { return true; }

    // enable optional components

    template<typename Cont, typename OtherMeshType>
    void enableSameOptionalComponentsOf(const OtherMeshType& m)
    {
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            Cont::enableOptionalComponentsOf(m);
        }
    }

    // private parent mesh pointers functions

    void updateAllParentMeshPointers() { (setParentMeshPointers<Args>(), ...); }

    template<typename Cont>
    void setParentMeshPointers()
    {
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            Cont::setParentMeshPointers(this);
        }
    }

    template<typename Cont>
    void appendContainer(const Mesh& m)
    {
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            Cont::append((const Cont&) m);
        }
    }

    // private copy and swap member functions

    /**
     * This function sets the Ith position of the array bases, where I is an
     * index of a container in a TypeWrapper of containers
     *
     * In the Ith position will be placed the base pointer of the vector of the
     * elements contained in the container Cont.
     */
    template<uint I, typename Cont, typename Array, typename... A>
    static void setContainerBase(const Mesh<A...>& m, Array& bases)
    {
        // since this function is called using pack expansion, it means that
        // Cont could be a mesh component and not a cointainer. We check if Cont
        // is a container
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            static_assert(I >= 0 && I != UINT_NULL);
            bases[I] = m.Cont::mElemVec.data();
        }
    }

    template<typename... A>
    static auto getContainerBases(const Mesh<A...>& m)
    {
        using Containers = Mesh<A...>::Containers;

        // the number of containers in Mesh<A...>
        constexpr uint N_CONTAINERS = NumberOfTypes<Containers>::value;
        // each element of this array will contain the base pointer of the
        // vector of elements contained in each container of Mesh<A...>
        std::array<const void*, N_CONTAINERS> bases;

        // for each container/component of Mesh<A...>, we set call the function
        // that sets the base of the container in its index
        (setContainerBase<IndexInTypes<A, Containers>::value, A>(m, bases),
         ...);

        return bases;
    }

    /**
     * This function sets the Ith position of the array sizes, where I is an
     * index of a container in a TypeWrapper of containers
     *
     * In the Ith position will be placed the size of the vector of the
     * elements contained in the container Cont.
     */
    template<uint I, typename Cont, typename Array, typename... A>
    static void setContainerSize(const Mesh<A...>& m, Array& sizes)
    {
        // since this function is called using pack expansion, it means that
        // Cont could be a mesh component and not a cointainer. We check if Cont
        // is a container
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            static_assert(I >= 0 && I != UINT_NULL);
            sizes[I] = m.Cont::mElemVec.size();
        }
    }

    template<typename... A>
    static auto getContainerSizes(const Mesh<A...>& m)
    {
        using Containers = Mesh<A...>::Containers;

        // the number of containers in Mesh<A...>
        constexpr uint N_CONTAINERS = NumberOfTypes<Containers>::value;
        // each element of this array will contain the size of the
        // vector of elements contained in each container of Mesh<A...>
        std::array<std::size_t, N_CONTAINERS> sizes;

        // for each container/component of Mesh<A...>, we set call the function
        // that sets the size of the container in its index
        (setContainerSize<IndexInTypes<A, Containers>::value, A>(m, sizes),
         ...);

        return sizes;
    }

    /**
     * This function *is static*, and is generally called for each container of
     * the mesh, that is the template argument Cont.
     *
     * In general, for each container, we need to update all the references
     * contained in it, that may be of any element of the mesh (example: in the
     * VertexContainer there could be Vertex references, but also Face or Edge
     * references).
     *
     * Here in this function, we loop into the containers of the Mesh m using
     * pack expansion, and we use the Cont type to choose which references type
     * we are updating:
     *
     * ```pseudo
     * for each Element E in Mesh:
     *    Cont::updateReferencesToElement(E)
     * ```
     *
     * The foreach loop is done using pack expansion inside the last call
     * m.updateReferences, trough the argument ContainerWrapper
     *
     * bases contains the old bases (the ones of the other mesh) for each
     * container.
     */
    template<typename Cont, typename Array, typename... A>
    static void updateReferencesOfContainerType(
        Mesh<A...>&  m,
        const Array& bases)
    {
        // since this function is called using pack expansion, it means that
        // Cont could be a mesh component and not a cointainer. We check if Cont
        // is a container
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            // The element type contained in the container
            // We need it to get back the actual type of the element from the
            // old bases
            using ElType = Cont::ElementType;

            using Containers = Mesh<A...>::Containers;
            constexpr uint I = IndexInTypes<Cont, Containers>::value;
            static_assert(I >= 0 && I != UINT_NULL);

            using ContainerWrapper = TypeWrapper<A...>;

            // for each Container A in m, we update the references of ElType.
            // old base is contained in the array bases, the new base is the
            // base of the container
            m.updateReferences(
                reinterpret_cast<const ElType*>(bases[I]), ContainerWrapper());
        }
    }

    template<typename Cont, typename ArrayB, typename ArrayS, typename... A>
    static void updateReferencesOfContainerTypeAfterAppend(
        Mesh<A...>&   m,
        const ArrayB& bases,
        const ArrayS& sizes)
    {
        // since this function is called using pack expansion, it means that
        // Cont could be a mesh component and not a cointainer. We check if Cont
        // is a container
        if constexpr (mesh::ElementContainerConcept<Cont>) {
            // The element type contained in the container
            // We need it to get back the actual type of the element from the
            // old bases
            using ElType = Cont::ElementType;

            using Containers = Mesh<A...>::Containers;
            constexpr uint I = IndexInTypes<Cont, Containers>::value;
            static_assert(I >= 0 && I != UINT_NULL);

            using ContainerWrapper = TypeWrapper<A...>;

            // for each Container A in m, we update the references of ElType.
            // old base is contained in the array bases
            m.updateReferences(
                reinterpret_cast<const ElType*>(bases[I]),
                ContainerWrapper(),
                sizes,
                sizes[I]);
        }
    }

    // member functions used by friends

    template<uint ELEM_ID, typename T>
    uint elementIndex(const T* el) const
    {
        using Cont   = ContainerOfElement<ELEM_ID>::type;
        using ElType = Cont::ElementType;
        return index(static_cast<const ElType*>(el));
    }

    template<typename El>
    auto& customComponents()
    {
        using ElCont = ContainerOf<El>::type;

        return ElCont::mCustomCompVecMap;
    }

    template<typename El>
    const auto& customComponents() const
    {
        using ElCont = ContainerOf<El>::type;

        return ElCont::mCustomCompVecMap;
    }

    template<typename El>
    auto& verticalComponents()
    {
        using ElCont = ContainerOf<El>::type;

        return ElCont::mVerticalCompVecTuple;
    }

    template<typename El>
    const auto& verticalComponents() const
    {
        using ElCont = ContainerOf<El>::type;

        return ElCont::mVerticalCompVecTuple;
    }
};

} // namespace vcl

#endif // VCL_MESH_MESH_H
