/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#ifndef PARENT_MESH_POINTER_H
#define PARENT_MESH_POINTER_H

namespace vcl::comp {

/**
 * @private
 * @brief The ParentMeshPointer class
 */
template<typename MeshType>
class ParentMeshPointer
{
    MeshType* parent = nullptr;

public:
    using ParentMeshType = MeshType;

    ParentMeshPointer() {}

    // copy and move semantics: nothing will be done here
    // this instance will be updated only by the mesh, and optional data
    // can be accessed only when an element is part of a mesh
    ParentMeshPointer(const ParentMeshPointer<MeshType>&) {}

    ParentMeshPointer(ParentMeshPointer<MeshType>&&) {}

    ParentMeshPointer& operator=(const ParentMeshPointer<MeshType>&)
    {
        return *this;
    }

    constexpr MeshType* parentMesh() { return parent; }

    constexpr const MeshType* parentMesh() const { return parent; }

protected:
    void setParentMesh(void* parentMesh)
    {
        parent = static_cast<MeshType*>(parentMesh);
    }
};

} // namespace vcl::comp

#endif // PARENT_MESH_POINTER_H
