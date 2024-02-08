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

// check parent mesh pointers

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

} // namespace detail

template<MeshConcept MeshType>
void checkMeshPointers(const MeshType& mesh)
{
    detail::checkParentMeshPointers(mesh, typename MeshType::Containers());

    // todo: check all the pointers in the containers
}

} // namespace vcl

#endif // VCL_MESH_UTILS_MESH_CHECK_H
