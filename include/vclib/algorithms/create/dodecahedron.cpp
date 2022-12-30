/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#include "dodecahedron.h"

#include <vclib/algorithms/polygon.h>
#include <vclib/misc/internal/tmp_meshes.h>

namespace vcl {

/**
 * @brief Creates and returns a Polygon Mesh containing a Dodecahedron.
 *
 * The returned mesh will contain 12 pentagons.
 *
 * @return A Mesh containing a dodecahedron.
 */
template<PolygonMeshConcept MeshType>
MeshType createDodecahedron()
{
	using VertexType = typename MeshType::VertexType;
	using CoordType  = typename VertexType::CoordType;
	using ScalarType = typename CoordType::ScalarType;

	MeshType mesh;

	const ScalarType fi = (1 + std::sqrt(5)) / 2;
	const ScalarType s0 = 0;
	const ScalarType s1 = fi;
	const ScalarType s2 = std::pow(fi, 2);
	const ScalarType s3 = 1;

	mesh.addVertices(
		CoordType(-s1, -s1, s1),
		CoordType(s2, s3, s0),
		CoordType(s2, -s3, s0),
		CoordType(-s2, s3, s0),
		CoordType(-s2, -s3, s0),
		CoordType(s0, s2, s3),
		CoordType(s0, s2, -s3),
		CoordType(s3, s0, -s2),
		CoordType(-s3, s0, -s2),
		CoordType(s0, -s2, -s3),
		CoordType(s0, -s2, s3),
		CoordType(s3, s0, s2),
		CoordType(-s3, s0, s2),
		CoordType(s1, s1, -s1),
		CoordType(s1, s1, s1),
		CoordType(-s1, s1, -s1),
		CoordType(-s1, s1, s1),
		CoordType(s1, -s1, -s1),
		CoordType(s1, -s1, s1),
		CoordType(-s1, -s1, -s1));

	mesh.reserveFaces(12);
	mesh.addFace(14, 11, 18, 2, 1);
	mesh.addFace(2, 17, 7, 13, 1);
	mesh.addFace(15, 8, 19, 4, 3);
	mesh.addFace(4, 0, 12, 16, 3);
	mesh.addFace(16, 5, 6, 15, 3);
	mesh.addFace(13, 6, 5, 14, 1);
	mesh.addFace(18, 10, 9, 17, 2);
	mesh.addFace(19, 9, 10, 0, 4);
	mesh.addFace(17, 9, 19, 8, 7);
	mesh.addFace(13, 7, 8, 15, 6);
	mesh.addFace(16, 12, 11, 14, 5);
	mesh.addFace(18, 11, 12, 0, 10);

	return mesh;
}

/**
 * @brief Creates and returns a Triangle Mesh containing a triangulated Dodecahedron.
 *
 * If the mesh is composed of triangles, the the returned mesh is already triangulated.
 * If the mesh is polygonal, a mesh containing 12 pentagons will be returned.
 *
 * @return A Mesh containing a dodecahedron.
 */
template<TriangleMeshConcept MeshType>
MeshType createDodecahedron()
{
	internal::TMPSimplePolyMesh pmesh = createDodecahedron<internal::TMPSimplePolyMesh>();

	MeshType mesh;
	mesh.reserveVertices(pmesh.vertexNumber());
	for (const auto& v : pmesh.vertices()) {
		mesh.addVertex(v.coord());
	}

	for (const auto& f : pmesh.faces()) {
		std::vector<uint> ind = earCut(f);
		for (uint i = 0; i < ind.size(); i += 3) {
			mesh.addFace(
				pmesh.index(f.vertex(ind[i])),
				pmesh.index(f.vertex(ind[i + 1])),
				pmesh.index(f.vertex(ind[i + 2])));
		}
	}

	return mesh;
}

} // namespace vcl
