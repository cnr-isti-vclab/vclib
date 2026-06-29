// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_PARENT_MESH_POINTER_H
#define VCL_MESH_COMPONENTS_PARENT_MESH_POINTER_H

namespace vcl::comp {

/**
 * @private
 * @brief The ParentMeshPointer class
 */
template<typename MeshType>
class ParentMeshPointer
{
    MeshType* mParent = nullptr;

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

    constexpr MeshType* parentMesh() { return mParent; }

    constexpr const MeshType* parentMesh() const { return mParent; }

protected:
    void setParentMesh(void* parentMesh)
    {
        mParent = static_cast<MeshType*>(parentMesh);
    }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_PARENT_MESH_POINTER_H
