// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCLIB_RENDER_EXAMPLES_COMMON_GET_DRAWABLE_MESH_H
#define VCLIB_RENDER_EXAMPLES_COMMON_GET_DRAWABLE_MESH_H

#include <vclib/algorithms/mesh/update/color.h>
#include <vclib/algorithms/mesh/update/normal.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

#include <vclib/render/drawable/drawable_mesh.h>

template<vcl::MeshConcept MeshType>
inline vcl::DrawableMesh<MeshType> getDrawableMesh(
    std::string filename              = "bimba.obj",
    bool        fromVCLibExamplesPath = true)
{
    if (fromVCLibExamplesPath) {
        filename = VCLIB_EXAMPLE_MESHES_PATH "/" + filename;
    }

    vcl::MeshInfo info;

    MeshType m = vcl::loadMesh<MeshType>(filename, info);

    if constexpr (vcl::FaceMeshConcept<MeshType>) {
        if (!info.hasPerFaceNormal()) {
            vcl::updatePerFaceNormals(m);
        }
        if (!info.hasPerVertexNormal()) {
            vcl::updatePerVertexNormalsFromFaceNormals(m);
        }
    }

    // create a DrawableMesh object from the mesh
    vcl::DrawableMesh<MeshType> drawable(m);

    return drawable;
}

#endif // VCLIB_RENDER_EXAMPLES_COMMON_GET_DRAWABLE_MESH_H
