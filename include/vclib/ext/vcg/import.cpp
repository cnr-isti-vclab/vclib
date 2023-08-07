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
	using CoordType = typename MeshType::VertexType::CoordType;

	mesh.reserveVertices(vcgMesh.VN());

	// copy vertices
	for (uint i = 0; i < vcgMesh.vert.size(); i++) {
		if (!vcgMesh.vert[i].IsD()) {
			uint vi = mesh.addVertex(CoordType(
				vcgMesh.vert[i].P()[0],
				vcgMesh.vert[i].P()[1],
				vcgMesh.vert[i].P()[2]));

			if constexpr (HasPerVertexNormal<MeshType>) {
				using NormalType = typename MeshType::VertexType::NormalType;
				if (isPerVertexNormalAvailable(mesh)) {
					if (vcg::tri::HasPerVertexNormal(vcgMesh)) {
						mesh.vertex(vi).normal() = NormalType(
							vcgMesh.vert[i].N()[0],
							vcgMesh.vert[i].N()[1],
							vcgMesh.vert[i].N()[2]);
					}
				}
			}
		}
	}
}

} // namespace vcl
