// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_VCG_EXPORT_H
#define VCL_VCG_EXPORT_H

#include <vclib/algorithms/mesh/utility.h>
#include <vclib/mesh/requirements.h>

#include <vcg/complex/complex.h>

namespace vcl::vc {

// todo - manage polygonal meshes
template<MeshConcept MeshType, typename VCGMeshType>
void exportMeshToVCGMesh(const MeshType& mesh, VCGMeshType& vcgMesh)
{
    vcg::tri::Allocator<VCGMeshType>::AddVertices(vcgMesh, mesh.vertexCount());

    uint vi = 0;
    for (const auto& v : mesh.vertices()) {
        vcgMesh.vert[vi].P().X() = v.position().x();
        vcgMesh.vert[vi].P().Y() = v.position().y();
        vcgMesh.vert[vi].P().Z() = v.position().z();

        // flags
        vcgMesh.vert[vi].Flags() = v.exportFlagsToVCGFormat();

        // normal
        if constexpr (HasVertexNormal<MeshType>) {
            if (mesh.isVertexNormalAvailable()) {
                if (vcg::tri::HasPerVertexNormal(vcgMesh)) {
                    vcgMesh.vert[vi].N().X() = v.normal().x();
                    vcgMesh.vert[vi].N().Y() = v.normal().y();
                    vcgMesh.vert[vi].N().Z() = v.normal().z();
                }
            }
        }

        // color
        if constexpr (HasVertexColor<MeshType>) {
            if (mesh.isVertexColorAvailable()) {
                if (vcg::tri::HasPerVertexColor(vcgMesh)) {
                    vcgMesh.vert[vi].C()[0] = v.color().red();
                    vcgMesh.vert[vi].C()[1] = v.color().green();
                    vcgMesh.vert[vi].C()[2] = v.color().blue();
                    vcgMesh.vert[vi].C()[3] = v.color().alpha();
                }
            }
        }

        // quality
        if constexpr (HasVertexQuality<MeshType>) {
            if (mesh.isVertexQualityAvailable()) {
                if (vcg::tri::HasPerVertexQuality(vcgMesh)) {
                    vcgMesh.vert[vi].Q() = v.quality();
                }
            }
        }

        // texcoord
        if constexpr (HasVertexTexCoord<MeshType>) {
            if (mesh.isVertexTexCoordAvailable()) {
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
        vcg::tri::Allocator<VCGMeshType>::AddFaces(vcgMesh, mesh.faceCount());

        auto vCompIndices = vcl::detail::vertCompactIndices(mesh, true);
        auto vIndex       = vcl::detail::vIndexLambda(mesh, vCompIndices);

        uint fi = 0;
        for (const auto& f : mesh.faces()) {
            vcgMesh.face[fi].V(0) = &vcgMesh.vert[vIndex(f, 0)];
            vcgMesh.face[fi].V(1) = &vcgMesh.vert[vIndex(f, 1)];
            vcgMesh.face[fi].V(2) = &vcgMesh.vert[vIndex(f, 2)];

            // flags
            vcgMesh.face[fi].Flags() = f.exportFlagsToVCGFormat();

            // normal
            if constexpr (HasFaceNormal<MeshType>) {
                if (mesh.isFaceNormalAvailable()) {
                    if (vcg::tri::HasPerFaceNormal(vcgMesh)) {
                        vcgMesh.face[fi].N().X() = f.normal().x();
                        vcgMesh.face[fi].N().Y() = f.normal().y();
                        vcgMesh.face[fi].N().Z() = f.normal().z();
                    }
                }
            }

            // color
            if constexpr (HasFaceColor<MeshType>) {
                if (mesh.isFaceColorAvailable()) {
                    if (vcg::tri::HasPerFaceColor(vcgMesh)) {
                        vcgMesh.face[fi].C()[0] = f.color().red();
                        vcgMesh.face[fi].C()[1] = f.color().green();
                        vcgMesh.face[fi].C()[2] = f.color().blue();
                        vcgMesh.face[fi].C()[3] = f.color().alpha();
                    }
                }
            }

            // quality
            if constexpr (HasFaceQuality<MeshType>) {
                if (mesh.isFaceQualityAvailable()) {
                    if (vcg::tri::HasPerFaceQuality(vcgMesh)) {
                        vcgMesh.face[fi].Q() = f.quality();
                    }
                }
            }

            // wedge texcoords
            if constexpr (HasFaceWedgeTexCoords<MeshType>) {
                if (mesh.isFaceWedgeTexCoordsAvailable()) {
                    if (vcg::tri::HasPerWedgeTexCoord(vcgMesh)) {
                        uint ti = 0;
                        if constexpr (HasFaceMaterialIndex<MeshType>) {
                            if (mesh.isFaceMaterialIndexAvailable()) {
                                ti = f.materialIndex();
                            }
                        }

                        vcgMesh.face[fi].WT(0).U() = f.wedge(0).texCoord().u();
                        vcgMesh.face[fi].WT(0).V() = f.wedge(0).texCoord().v();
                        vcgMesh.face[fi].WT(0).N() = ti;
                        vcgMesh.face[fi].WT(1).U() = f.wedge(1).texCoord().u();
                        vcgMesh.face[fi].WT(1).V() = f.wedge(1).texCoord().v();
                        vcgMesh.face[fi].WT(1).N() = ti;
                        vcgMesh.face[fi].WT(2).U() = f.wedge(2).texCoord().u();
                        vcgMesh.face[fi].WT(2).V() = f.wedge(2).texCoord().v();
                        vcgMesh.face[fi].WT(2).N() = ti;
                    }
                }
            }

            // todo - custom components

            fi++;
        }
    }

    if constexpr (HasMaterials<MeshType>) {
        for (const Material& m : mesh.materials()) {
            vcgMesh.textures.push_back(m.baseColorTextureDescriptor().path());
        }
    }
}

} // namespace vcl::vc

#endif // VCL_VCG_EXPORT_H
