/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include <vclib/mesh/requirements.h>

namespace vcl {

namespace internal {

template<typename MeshType>
MeshType triangleDodecahedron()
{
	using VertexType     = typename MeshType::VertexType;
	using VertexIterator = typename MeshType::VertexIterator;
	using CoordType      = typename VertexType::CoordType;
	using ScalarType     = typename CoordType::ScalarType;
	using FaceType       = typename MeshType::FaceType;
	using FaceIterator   = typename MeshType::FaceIterator;

	MeshType mesh;

	// TODO: remove this and triangulate polygonDodecahedron

	const int nPenta  = 12;
	const int nPoints = 62;

	int penta[nPenta * 3 * 3] = {
		20, 11, 18, 18, 11, 8,  8,  11, 4,  13, 23, 4,  4,  23, 8,  8,  23, 16, 13, 4,  30, 30,
		4,  28, 28, 4,  11, 16, 34, 8,  8,  34, 18, 18, 34, 36, 11, 20, 28, 28, 20, 45, 45, 20,
		38, 13, 30, 23, 23, 30, 41, 41, 30, 47, 16, 23, 34, 34, 23, 50, 50, 23, 41, 20, 18, 38,
		38, 18, 52, 52, 18, 36, 30, 28, 47, 47, 28, 56, 56, 28, 45, 50, 60, 34, 34, 60, 36, 36,
		60, 52, 45, 38, 56, 56, 38, 60, 60, 38, 52, 50, 41, 60, 60, 41, 56, 56, 41, 47};
	const ScalarType p               = (1.0 + std::sqrt(5.0)) / 2.0;
	const ScalarType p2              = p * p;
	const ScalarType p3              = p * p * p;
	ScalarType       vv[nPoints * 3] = {
        0,   0,      2 * p2, p2,     0,   p3,      p,   p2,  p3,  0,       p,   p3,  -p,  p2,
        p3,  -p2,    0,      p3,     -p,  -p2,     p3,  0,   -p,  p3,      p,   -p2, p3,  p3,
        p,   p2,     p2,     p2,     p2,  0,       p3,  p2,  -p2, p2,      p2,  -p3, p,   p2,
        -p3, -p,     p2,     -p2,    -p2, p2,      0,   -p3, p2,  p2,      -p2, p2,  p3,  -p,
        p2,  p3,     0,      p,      p2,  p3,      p,   -p2, p3,  p,       -p3, 0,   p,   -p2,
        -p3, p,      p2,     -p3,    p,   2 * p2,  0,   0,   p3,  p2,      0,   p,   p3,  0,
        0,   2 * p2, 0,      -p,     p3,  0,       -p3, p2,  0,   -2 * p2, 0,   0,   -p3, -p2,
        0,   -p,     -p3,    0,      0,   -2 * p2, 0,   p,   -p3, 0,       p3,  -p2, 0,   p3,
        0,   -p,     p2,     p3,     -p,  -p2,     p3,  -p,  -p3, 0,       -p,  -p2, -p3, -p,
        p2,  -p3,    -p,     p3,     p,   -p2,     p2,  p2,  -p2, 0,       p3,  -p2, -p2, p2,
        -p2, -p3,    p,      -p2,    -p3, -p,      -p2, -p2, -p2, -p2,     0,   -p3, -p2, p2,
        -p2, -p2,    p3,     -p,     -p2, p2,      0,   -p3, p,   p2,      -p3, 0,   p,   -p3,
        -p,  p2,     -p3,    -p2,    0,   -p3,     -p,  -p2, -p3, 0,       -p,  -p3, p,   -p2,
        -p3, 0,      0,      -2 * p2};

	mesh.addVertices(32);
	mesh.addFaces(5 * 12); // five pentagons, each made by 5 tri

	int h, m = 0;

	bool used[nPoints] = {false};

	int        reindex[20 + 12 * 10];
	ScalarType xx, yy, zz, sx, sy, sz;

	int order[5] = {0, 1, 8, 6, 2};
	int added[12];

	VertexIterator vi = mesh.vertexBegin();
	for (uint i = 0; i < 12; i++) {
		sx = sy = sz = 0;
		for (int j = 0; j < 5; j++) {
			h  = penta[i * 9 + order[j]] - 1;
			xx = vv[h * 3];
			yy = vv[h * 3 + 1];
			zz = vv[h * 3 + 2];
			sx += xx;
			sy += yy;
			sz += zz;
			if (!used[h]) {
				vi->coord() = CoordType(xx, yy, zz);
				++vi;
				used[h]    = true;
				reindex[h] = m++;
			}
		}
		vi->coord() = CoordType(sx / 5.0, sy / 5.0, sz / 5.0);
		++vi;
		added[i] = m++;
	}

	std::vector<VertexType*> index(mesh.vertexNumber());

	uint j = 0;
	for (VertexType& v : mesh.vertices())
		index[j++] = &v;

	FaceIterator fi = mesh.faceBegin();

	for (uint i = 0; i < 12; i++) {
		for (j = 0; j < 5; j++) {
			fi->vertex(0) = index[added[i]];
			fi->vertex(1) = index[reindex[penta[i * 9 + order[j]] - 1]];
			fi->vertex(2) = index[reindex[penta[i * 9 + order[(j + 1) % 5]] - 1]];
			fi->setEdgeFaux(0);
			fi->setEdgeFaux(2);
			fi++;
		}
	}

	return mesh;
}

template<typename MeshType>
MeshType polygonDodecahedron()
{
	using VertexType     = typename MeshType::VertexType;
	using VertexIterator = typename MeshType::VertexIterator;
	using CoordType      = typename VertexType::CoordType;
	using ScalarType     = typename CoordType::ScalarType;

	MeshType mesh;

	const ScalarType s0 = 0;
	const ScalarType s1 = 0.57735;
	const ScalarType s2 = 0.934172;
	const ScalarType s3 = 0.356822;

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

} // namespace internal

/**
 * @brief Creates and returns a Mesh containing a Dodecahedron.
 *
 * If the mesh is composed of triangles, the the returned mesh is already triangulated.
 * If the mesh is polygonal, a mesh containing 12 pentagons will be returned.
 *
 * @return A Mesh containing a dodecahedron.
 */
template<typename MeshType>
MeshType createDodecahedron()
{
	if constexpr (hasTriangles<MeshType>()) {
		return internal::triangleDodecahedron<MeshType>();
	}
	else if constexpr (hasPolygons<MeshType>()) {
		return internal::polygonDodecahedron<MeshType>();
	}
	else {
		static_assert(true, "Mesh Type not supported for creating Dodecahedron.");
	}
}

} // namespace vcl
