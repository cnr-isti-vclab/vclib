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

#include "import.h"

namespace vcl {

template<MeshConcept MeshType, typename VCGMeshType>
MeshType meshFromVCGMesh(const VCGMeshType& vcgMesh)
{
	MeshType mesh;

	importMeshFromVCGMesh(mesh, vcgMesh);

	return mesh;
}

template<MeshConcept MeshType, typename VCGMeshType>
void importMeshFromVCGMesh(MeshType& mesh, const VCGMeshType& vcgMesh)
{
	using CoordType = MeshType::VertexType::CoordType;

	mesh.reserveVertices(vcgMesh.VN());

	// vertices
	for (uint i = 0; i < vcgMesh.vert.size(); i++) {
		if (!vcgMesh.vert[i].IsD()) {
			uint vi = mesh.addVertex(CoordType(
				vcgMesh.vert[i].P()[0],
				vcgMesh.vert[i].P()[1],
				vcgMesh.vert[i].P()[2]));

			// flags
			mesh.vertex(vi).importFromVCGFlags(vcgMesh.vert[i].Flags());

			// normal
			if constexpr (HasPerVertexNormal<MeshType>) {
				using NormalType = MeshType::VertexType::NormalType;
				if (isPerVertexNormalAvailable(mesh)) {
					if (vcg::tri::HasPerVertexNormal(vcgMesh)) {
						mesh.vertex(vi).normal() = NormalType(
							vcgMesh.vert[i].N()[0],
							vcgMesh.vert[i].N()[1],
							vcgMesh.vert[i].N()[2]);
					}
				}
			}

			// color
			if constexpr (HasPerVertexColor<MeshType>) {
				if (isPerVertexColorAvailable(mesh)) {
					if (vcg::tri::HasPerVertexColor(vcgMesh)) {
						mesh.vertex(vi).color() = Color(
							vcgMesh.vert[i].C()[0],
							vcgMesh.vert[i].C()[1],
							vcgMesh.vert[i].C()[2],
							vcgMesh.vert[i].C()[3]);
					}
				}
			}

			// quality
			if constexpr (HasPerVertexQuality<MeshType>) {
				if (isPerVertexQualityAvailable(mesh)) {
					if (vcg::tri::HasPerVertexQuality(vcgMesh)) {
						mesh.vertex(vi).quality() = vcgMesh.vert[i].Q();
					}
				}
			}

			// texcoord
			if constexpr (HasPerVertexTexCoord<MeshType>) {
				using TexCoordType = MeshType::VertexType::TexCoordType;
				if (isPerVertexTexCoordAvailable(mesh)) {
					if (vcg::tri::HasPerVertexTexCoord(vcgMesh)) {
						mesh.vertex(vi).texCoord() = TexCoordType(
							vcgMesh.vert[i].T().U(),
							vcgMesh.vert[i].T().V());
					}
				}
			}
		}
	}

	// faces
	if constexpr (HasFaces<MeshType>) {
		using FaceType = MeshType::FaceType;

		for (uint i = 0; i < vcgMesh.face.size(); ++i) {
			if (!vcgMesh.face[i].IsD()) {
				uint fi = mesh.addFace();
				if constexpr (FaceType::VERTEX_NUMBER < 0) {
					mesh.face(fi).resizeVertices(3);
				}
				for (uint j = 0; j < 3; ++j) {
					uint vi = vcg::tri::Index(vcgMesh, vcgMesh.face[i].V(j));
					mesh.face(fi).vertex(j) = &mesh.vertex(vi);
				}

				// flags
				mesh.face(fi).importFromVCGFlags(vcgMesh.face[i].Flags());

				// normal
				if constexpr (HasPerFaceNormal<MeshType>) {
					using NormalType = MeshType::FaceType::NormalType;
					if (isPerFaceNormalAvailable(mesh)) {
						if (vcg::tri::HasPerFaceNormal(vcgMesh)) {
							mesh.face(fi).normal() = NormalType(
								vcgMesh.face[i].N()[0],
								vcgMesh.face[i].N()[1],
								vcgMesh.face[i].N()[2]);
						}
					}
				}

				// color
				if constexpr (HasPerFaceColor<MeshType>) {
					if (isPerFaceColorAvailable(mesh)) {
						if (vcg::tri::HasPerFaceColor(vcgMesh)) {
							mesh.face(fi).color() = Color(
								vcgMesh.face[i].C()[0],
								vcgMesh.face[i].C()[1],
								vcgMesh.face[i].C()[2],
								vcgMesh.face[i].C()[3]);
						}
					}
				}

				// quality
				if constexpr (HasPerFaceQuality<MeshType>) {
					if (isPerFaceQualityAvailable(mesh)) {
						if (vcg::tri::HasPerFaceQuality(vcgMesh)) {
							mesh.face(fi).quality() = vcgMesh.face[i].Q();
						}
					}
				}

				// wedge texcoords
				if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
					using WTType = MeshType::FaceType::WedgeTexCoordType;
					if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
						if (vcg::tri::HasPerWedgeTexCoord(vcgMesh)) {
							mesh.face(fi).textureIndex() =
								vcgMesh.face[i].WT(0).N();
							for (uint j = 0; j < 3; ++j) {
								mesh.face(fi).wedgeTexCoord(j) = WTType(
									vcgMesh.face[i].WT(j).U(),
									vcgMesh.face[i].WT(j).V());
							}
						}
					}
				}
			}
		}
	}
}

} // namespace vcl
