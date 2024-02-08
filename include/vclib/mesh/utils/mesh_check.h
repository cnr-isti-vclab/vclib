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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_MESH_UTILS_MESH_CHECK_H
#define VCL_MESH_UTILS_MESH_CHECK_H

#include <vclib/concepts/mesh.h>
#include <vclib/exceptions/mesh_exceptions.h>
#include <vclib/misc/string.h>

namespace vcl {

namespace detail {

/** check parent mesh pointers **/

template<uint ELEM_ID, MeshConcept MeshType>
void checkParentMeshPointers(const MeshType& mesh)
{
    for (const auto& el : mesh.template elements<ELEM_ID>()) {
        if (el.parentMesh() != &mesh) {
            throw InconsistentMeshException(
                "The " + vcl::elementEnumString<ELEM_ID>() + " n. " +
                vcl::toString(el.index()) + " has a wrong Parent Mesh.\n" +
                "Expected: " + vcl::toString(&mesh) + "; " +
                "Found: " + vcl::toString(el.parentMesh()));
        }
    }
}

template<MeshConcept MeshType, typename ...Containers>
void checkParentMeshPointers(const MeshType& mesh, TypeWrapper<Containers...>)
{
    (checkParentMeshPointers<Containers::ElementType::ELEMENT_ID>(mesh), ...);
}

/** check element mesh pointers **/

template<uint ELEM_ID, typename Comp, MeshConcept MeshType, typename ElemType>
void checkElementPointersInElementContainerOnComponent(
    const MeshType& mesh,
    const ElemType* first,
    const ElemType* last)
{
    if constexpr (comp::HasPointersOfType<Comp, ElemType>) {
        auto loop = [&]() {
            for (const auto& el : mesh.template elements<ELEM_ID>()) {
                const Comp& comp = static_cast<const Comp&>(el);
                for (const ElemType* ptr : comp.pointers()) {
                    if (ptr < first || ptr >= last) {
                        throw InconsistentMeshException(
                            "The " + vcl::elementEnumString<ELEM_ID>() +
                            " n. " + vcl::toString(el.index()) +
                            " has a wrong pointer in " +
                            vcl::componentEnumString<Comp::COMPONENT_ID>() +
                            " component.\n" + "The pointer " +
                            vcl::toString(ptr) + " is out of range [" +
                            vcl::toString(first) + ", " + vcl::toString(last) +
                            ")");
                    }
                }
            }
        };

        if constexpr (comp::HasOptionalPointersOfType<Comp, ElemType>) {
            if (mesh.template isPerElementComponentEnabled<
                    ELEM_ID,
                    Comp::COMPONENT_ID>())
            {
                loop();
            }
        }
        else {
            loop();
        }
    }
}

template<uint ELEM_ID, MeshConcept MeshType, typename ElemType, typename... Comps>
void checkElementPointersInElementContainerOnComponents(
    const MeshType& mesh,
    const ElemType* first,
    const ElemType* last,
    TypeWrapper<Comps...>)
{
    (checkElementPointersInElementContainerOnComponent<ELEM_ID, Comps>(
         mesh, first, last),
     ...);
}

template<uint ELEM_ID, MeshConcept MeshType, typename ElemType>
void checkElementPointersInElementContainer(
    const MeshType& mesh,
    const ElemType* first,
    const ElemType* last)
{
    using ThisElemType = MeshType::template ElementType<ELEM_ID>;
    using ThisElemComponents = ThisElemType::Components;

    checkElementPointersInElementContainerOnComponents<ELEM_ID>(
        mesh, first, last, ThisElemComponents());
}

template<uint ELEM_ID, MeshConcept MeshType, typename... Containers>
void checkElementPointers(const MeshType& mesh, TypeWrapper<Containers...>)
{
    using ElemType = MeshType::template ElementType<ELEM_ID>;

    const ElemType* first = &mesh.template element<ELEM_ID>(0);
    uint size = mesh.template containerSize<ELEM_ID>();
    const ElemType* last = first + size;

    // now, for each Container, I need to check whether the pointers to
    // ElemType* are in the right range
    (checkElementPointersInElementContainer<Containers::ElementType::ELEMENT_ID>(
         mesh, first, last),
     ...);
}

template<MeshConcept MeshType, typename ...Containers>
void checkMeshPointers(const MeshType& mesh, TypeWrapper<Containers...>)
{
    (checkElementPointers<Containers::ElementType::ELEMENT_ID>(
         mesh, typename MeshType::Containers()),
     ...);
}

} // namespace detail

template<MeshConcept MeshType>
void checkMeshPointers(const MeshType& mesh)
{
    detail::checkParentMeshPointers(mesh, typename MeshType::Containers());

    detail::checkMeshPointers(mesh, typename MeshType::Containers());
}

} // namespace vcl

#endif // VCL_MESH_UTILS_MESH_CHECK_H
