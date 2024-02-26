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
