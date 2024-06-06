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

#ifndef VCL_MESH_UTILS_MESH_CHECK_H
#define VCL_MESH_UTILS_MESH_CHECK_H

#include <vclib/concepts/mesh.h>
#include <vclib/exceptions/mesh.h>
#include <vclib/misc/string.h>

namespace vcl {

namespace detail {

/** check parent mesh pointers **/

template<uint ELEM_ID, MeshConcept MeshType>
bool checkParentMeshPointers(const MeshType& mesh)
{
    for (const auto& el : mesh.template elements<ELEM_ID>()) {
        if (el.parentMesh() != &mesh) {
            throw InconsistentMeshException(
                "The " + vcl::elementEnumString<ELEM_ID>() + " n. " +
                vcl::toString(el.index()) + " has a wrong Parent Mesh.\n" +
                "Expected: " + vcl::toString(&mesh) + "; " +
                "Found: " + vcl::toString(el.parentMesh()));
            return false;
        }
    }
    return true;
}

template<MeshConcept MeshType, typename... Containers>
bool checkParentMeshPointers(const MeshType& mesh, TypeWrapper<Containers...>)
{
    return (
        checkParentMeshPointers<Containers::ElementType::ELEMENT_ID>(mesh) &&
        ...);
}

/** check element mesh pointers **/

template<uint ELEM_ID, typename Comp, MeshConcept MeshType, typename ElemType>
bool checkElementPointersInElementContainerOnComponent(
    const MeshType& mesh,
    const ElemType* first,
    const ElemType* last)
{
    // actual check:
    // We now need to check, for the component Comp of each element ELEM_ID,
    // if it has pointers to ElemType, and if so, if they are in the range
    // [first, last).

    // if Comp has pointers to ElemType
    if constexpr (comp::HasPointersOfType<Comp, ElemType>) {
        // we create a lambda to loop over the elements of the mesh
        // to avoid code duplication

        auto pointersLoop = [&]() {
            // for each element of type ELEM_ID in the mesh
            for (const auto& el : mesh.template elements<ELEM_ID>()) {
                // take its Comp component - we know that Comp is a component
                // that has pointers to ElemType
                const Comp& comp = static_cast<const Comp&>(el);

                // take the pointers in the component and look at them
                for (const ElemType* ptr : comp.template pointers<ElemType>()) {
                    if (ptr != nullptr) {
                        if (ptr < first || ptr >= last) {
                            throw InconsistentMeshException(
                                "The " + vcl::elementEnumString<ELEM_ID>() +
                                " n. " + vcl::toString(el.index()) +
                                " has a wrong pointer in " +
                                vcl::componentEnumString<Comp::COMPONENT_ID>() +
                                " component.\n" + "The pointer " +
                                vcl::toString(ptr) + " is out of range [" +
                                vcl::toString(first) + ", " +
                                vcl::toString(last) + ")");
                            return false;
                        }
                    }
                }
            }
            return true;
        };

        // if Comp is optional, we first need to check if it is enabled
        if constexpr (comp::HasOptionalPointersOfType<Comp, ElemType>) {
            if (mesh.template isPerElementComponentEnabled<
                    ELEM_ID,
                    Comp::COMPONENT_ID>())
            {
                return pointersLoop();
            }
            return true;
        }
        else {
            return pointersLoop();
        }
    }

    // if Comp has indices to ElemType
    if constexpr (comp::HasIndicesOfType<Comp, ElemType>) {
        // we create a lambda to loop over the elements of the mesh
        // to avoid code duplication

        auto indicesLoop = [&]() {
            // for each element of type ELEM_ID in the mesh
            for (const auto& el : mesh.template elements<ELEM_ID>()) {
                // take its Comp component - we know that Comp is a component
                // that has indices to ElemType
                const Comp& comp = static_cast<const Comp&>(el);

                // take the pointers in the component and look at them
                for (uint i : comp.template indices<ElemType>()) {
                    if (i != UINT_NULL) {
                        if (i >= last - first) {
                            throw InconsistentMeshException(
                                "The " + vcl::elementEnumString<ELEM_ID>() +
                                " n. " + vcl::toString(el.index()) +
                                " has a wrong index in " +
                                vcl::componentEnumString<Comp::COMPONENT_ID>() +
                                " component.\n" + "The index " +
                                vcl::toString(i) + " is out of range [ 0, " +
                                vcl::toString(last - first) + ")");
                            return false;
                        }
                    }
                }
            }
            return true;
        };

        // if Comp is optional, we first need to check if it is enabled
        if constexpr (comp::HasOptionalIndicesOfType<Comp, ElemType>) {
            if (mesh.template isPerElementComponentEnabled<
                    ELEM_ID,
                    Comp::COMPONENT_ID>())
            {
                return indicesLoop();
            }
            return true;
        }
        else {
            return indicesLoop();
        }
    }

    return true;
}

template<
    uint        ELEM_ID,
    MeshConcept MeshType,
    typename ElemType,
    typename... Comps>
bool checkElementPointersInElementContainerOnComponents(
    const MeshType& mesh,
    const ElemType* first,
    const ElemType* last,
    TypeWrapper<Comps...>)
{
    // this function calls the checker for each component of ELEM_ID element

    return (
        checkElementPointersInElementContainerOnComponent<ELEM_ID, Comps>(
            mesh, first, last) &&
        ...);
}

template<uint ELEM_ID, MeshConcept MeshType, typename ElemType>
bool checkElementPointersInElementContainer(
    const MeshType& mesh,
    const ElemType* first,
    const ElemType* last)
{
    using ThisElemType       = MeshType::template ElementType<ELEM_ID>;
    using ThisElemComponents = ThisElemType::Components;

    // for the ELEM_ID container, check the pointers of the ElemType
    // on each component of ELEM_ID

    // loop into the components of ELEM_ID element
    return checkElementPointersInElementContainerOnComponents<ELEM_ID>(
        mesh, first, last, ThisElemComponents());
}

template<uint ELEM_ID, MeshConcept MeshType, typename... Containers>
bool checkElementPointers(const MeshType& mesh, TypeWrapper<Containers...>)
{
    using ElemType = MeshType::template ElementType<ELEM_ID>;

    const ElemType* first = &mesh.template element<ELEM_ID>(0);
    uint            size  = mesh.template containerSize<ELEM_ID>();
    const ElemType* last  = first + size;

    // now, for each Container, I need to check whether the pointers to
    // ElemType* are in the right range
    return (
        checkElementPointersInElementContainer<
            Containers::ElementType::ELEMENT_ID>(mesh, first, last) &&
        ...);
}

template<MeshConcept MeshType, typename... Containers>
bool checkMeshPointers(const MeshType& mesh, TypeWrapper<Containers...>)
{
    return (
        checkElementPointers<Containers::ElementType::ELEMENT_ID>(
            mesh, TypeWrapper<Containers...>()) &&
        ...);
}

} // namespace detail

template<MeshConcept MeshType>
bool checkMeshPointers(const MeshType& mesh)
{
    bool res =
        detail::checkParentMeshPointers(mesh, typename MeshType::Containers());

    res =
        res && detail::checkMeshPointers(mesh, typename MeshType::Containers());
    return res;
}

} // namespace vcl

#endif // VCL_MESH_UTILS_MESH_CHECK_H
