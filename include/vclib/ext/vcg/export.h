/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_EXT_VCG_EXPORT_H
#define VCL_EXT_VCG_EXPORT_H

#include <vclib/mesh/requirements.h>

#include <vcg/complex/complex.h>

namespace vcl::vc {

// todo - manage polygonal meshes
template<MeshConcept MeshType, typename VCGMeshType>
void exportMeshToVCGMesh(const MeshType& mesh, VCGMeshType& vcgMesh)
{
    vcg::tri::Allocator<VCGMeshType>::AddVertices(vcgMesh, mesh.vertexNumber());

    uint vi = 0;
    for (auto v : mesh.vertices()) {
        vcgMesh.vert[vi].P().X() = v.coord().x();
        vcgMesh.vert[vi].P().Y() = v.coord().y();
        vcgMesh.vert[vi].P().Z() = v.coord().z();

        // manage here other components

        vi++;
    }

    if constexpr (HasFaces<MeshType>) {
        vcg::tri::Allocator<VCGMeshType>::AddFaces(vcgMesh, mesh.faceNumber());

        uint fi = 0;
        for (auto f : mesh.faces()) {
            vcgMesh.face[fi].V(0) = &vcgMesh.vert[mesh.index(f.vertex(0))];
            vcgMesh.face[fi].V(1) = &vcgMesh.vert[mesh.index(f.vertex(1))];
            vcgMesh.face[fi].V(2) = &vcgMesh.vert[mesh.index(f.vertex(2))];


            // manage here other components

            fi++;
        }
    }
}

} // namespace vcl::vc

#endif // VCL_EXT_VCG_EXPORT_H
