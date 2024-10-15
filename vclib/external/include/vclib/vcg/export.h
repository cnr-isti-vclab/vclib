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
    for (const auto& v : mesh.vertices()) {
        vcgMesh.vert[vi].P().X() = v.coord().x();
        vcgMesh.vert[vi].P().Y() = v.coord().y();
        vcgMesh.vert[vi].P().Z() = v.coord().z();

        // flags
        vcgMesh.vert[vi].Flags() = v.exportFlagsToVCGFormat();

        // normal
        if constexpr (HasPerVertexNormal<MeshType>) {
            if (isPerVertexNormalAvailable(mesh)) {
                if (vcg::tri::HasPerVertexNormal(vcgMesh)) {
                    vcgMesh.vert[vi].N().X() = v.normal().x();
                    vcgMesh.vert[vi].N().Y() = v.normal().y();
                    vcgMesh.vert[vi].N().Z() = v.normal().z();
                }
            }
        }

        // color
        if constexpr (HasPerVertexColor<MeshType>) {
            if (isPerVertexColorAvailable(mesh)) {
                if (vcg::tri::HasPerVertexColor(vcgMesh)) {
                    vcgMesh.vert[vi].C()[0] = v.color().red();
                    vcgMesh.vert[vi].C()[1] = v.color().green();
                    vcgMesh.vert[vi].C()[2] = v.color().blue();
                    vcgMesh.vert[vi].C()[3] = v.color().alpha();
                }
            }
        }

        // quality
        if constexpr (HasPerVertexQuality<MeshType>) {
            if (isPerVertexQualityAvailable(mesh)) {
                if (vcg::tri::HasPerVertexQuality(vcgMesh)) {
                    vcgMesh.vert[vi].Q() = v.quality();
                }
            }
        }

        // texcoord
        if constexpr (HasPerVertexTexCoord<MeshType>) {
            if (isPerVertexTexCoordAvailable(mesh)) {
                if (vcg::tri::HasPerVertexTexCoord(vcgMesh)) {
                    vcgMesh.vert[vi].T().U() = v.texCoord().u();
                    vcgMesh.vert[vi].T().V() = v.texCoord().v();
                }
            }
        }

        // todo - custom components

        vi++;
    }

    if constexpr (HasFaces<MeshType>) {
        vcg::tri::Allocator<VCGMeshType>::AddFaces(vcgMesh, mesh.faceNumber());

        uint fi = 0;
        for (const auto& f : mesh.faces()) {
            vcgMesh.face[fi].V(0) = &vcgMesh.vert[mesh.index(f.vertex(0))];
            vcgMesh.face[fi].V(1) = &vcgMesh.vert[mesh.index(f.vertex(1))];
            vcgMesh.face[fi].V(2) = &vcgMesh.vert[mesh.index(f.vertex(2))];

            // flags
            vcgMesh.face[fi].Flags() = f.exportFlagsToVCGFormat();

            // normal
            if constexpr (HasPerFaceNormal<MeshType>) {
                if (isPerFaceNormalAvailable(mesh)) {
                    if (vcg::tri::HasPerFaceNormal(vcgMesh)) {
                        vcgMesh.face[fi].N().X() = f.normal().x();
                        vcgMesh.face[fi].N().Y() = f.normal().y();
                        vcgMesh.face[fi].N().Z() = f.normal().z();
                    }
                }
            }

            // color
            if constexpr (HasPerFaceColor<MeshType>) {
                if (isPerFaceColorAvailable(mesh)) {
                    if (vcg::tri::HasPerFaceColor(vcgMesh)) {
                        vcgMesh.face[fi].C()[0] = f.color().red();
                        vcgMesh.face[fi].C()[1] = f.color().green();
                        vcgMesh.face[fi].C()[2] = f.color().blue();
                        vcgMesh.face[fi].C()[3] = f.color().alpha();
                    }
                }
            }

            // quality
            if constexpr (HasPerFaceQuality<MeshType>) {
                if (isPerFaceQualityAvailable(mesh)) {
                    if (vcg::tri::HasPerFaceQuality(vcgMesh)) {
                        vcgMesh.face[fi].Q() = f.quality();
                    }
                }
            }

            // wedge texcoords
            if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
                if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                    if (vcg::tri::HasPerWedgeTexCoord(vcgMesh)) {
                        vcgMesh.face[fi].WT(0).U() = f.wedgeTexCoord(0).u();
                        vcgMesh.face[fi].WT(0).V() = f.wedgeTexCoord(0).v();
                        vcgMesh.face[fi].WT(0).N() = f.textureIndex();
                        vcgMesh.face[fi].WT(1).U() = f.wedgeTexCoord(1).u();
                        vcgMesh.face[fi].WT(1).V() = f.wedgeTexCoord(1).v();
                        vcgMesh.face[fi].WT(1).N() = f.textureIndex();
                        vcgMesh.face[fi].WT(2).U() = f.wedgeTexCoord(2).u();
                        vcgMesh.face[fi].WT(2).V() = f.wedgeTexCoord(2).v();
                        vcgMesh.face[fi].WT(2).N() = f.textureIndex();
                    }
                }
            }

            // todo - custom components

            fi++;
        }
    }
}

} // namespace vcl::vc

#endif // VCL_EXT_VCG_EXPORT_H
