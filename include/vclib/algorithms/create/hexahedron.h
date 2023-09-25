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

#ifndef VCL_ALGORITHMS_CREATE_HEXAHEDRON_H
#define VCL_ALGORITHMS_CREATE_HEXAHEDRON_H

#include <vclib/mesh/requirements.h>

namespace vcl {

/****************
 * Declarations *
 ****************/

template<FaceMeshConcept MeshType>
MeshType createHexahedron();

template<FaceMeshConcept MeshType, Point3Concept CoordType>
MeshType createHexahedron(const CoordType& min, const CoordType& max);

template<FaceMeshConcept MeshType, Point3Concept CoordType>
MeshType createCube(const CoordType& min, double edgeLength);

/***************
 * Definitions *
 ***************/

namespace internal {

template<FaceMeshConcept MeshType>
void fillHexahedronTriangles(MeshType& m)
{
	m.reserveFaces(12);
	m.addFace(0, 2, 1);
	m.addFace(3, 1, 2);
	m.addFace(0, 4, 2);
	m.addFace(6, 2, 4);
	m.addFace(0, 1, 4);
	m.addFace(5, 4, 1);
	m.addFace(7, 6, 5);
	m.addFace(4, 5, 6);
	m.addFace(7, 3, 6);
	m.addFace(2, 6, 3);
	m.addFace(7, 5, 3);
	m.addFace(1, 3, 5);
}

template<FaceMeshConcept MeshType>
void fillHexahedronQuads(MeshType& m)
{
	m.reserveFaces(6);
	m.addFace(2, 3, 1, 0);
	m.addFace(4, 6, 2, 0);
	m.addFace(1, 5, 4, 0);
	m.addFace(6, 4, 5, 7);
	m.addFace(3, 2, 6, 7);
	m.addFace(5, 1, 3, 7);
}

} // namespace internal

/**
 * @brief Creates and returns a hexahedron having as extremes the points ``(-1,
 * -1, -1)`` and ``(1, 1, 1)``.
 *
 * If the mesh is composed of triangles, the the returned mesh is already
 * triangulated. If the mesh is composed of quads or polygons, a mesh containing
 * 6 quads will be returned.
 *
 * @return A Mesh containing a ``[(-1, -1, -1), (1, 1, 1)]``Hexahedron .
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType>
MeshType createHexahedron()
{
	using CoordType = MeshType::Vertex::CoordType;

	return createHexahedron<MeshType>(
		CoordType(-1, -1, -1), CoordType(1, 1, 1));
}

/**
 * @brief Creates and returns a Hexahedron having as extremes points min and max
 * given as arguments.
 *
 * If the mesh is composed of triangles, the the returned mesh is already
 * triangulated. If the mesh is composed of quads or polygons, a mesh containing
 * 6 quads will be returned.
 *
 * @return A Mesh containing a ``[min, max]``Hexahedron.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType, Point3Concept CoordType>
MeshType createHexahedron(const CoordType& min, const CoordType& max)
{
	MeshType m;

		   // fill vertices...
	m.addVertices(
		CoordType(min(0), min(1), min(2)),
		CoordType(max(0), min(1), min(2)),
		CoordType(min(0), max(1), min(2)),
		CoordType(max(0), max(1), min(2)),
		CoordType(min(0), min(1), max(2)),
		CoordType(max(0), min(1), max(2)),
		CoordType(min(0), max(1), max(2)),
		CoordType(max(0), max(1), max(2)));

		   // fill faces
	if constexpr (HasTriangles<MeshType>) {
		internal::fillHexahedronTriangles(m);
	}
	else {
		internal::fillHexahedronQuads(m);
	}
	return m;
}

/**
 * @brief Creates and returns a Cube having ``min`` as minimum extreme and the
 * given edge length.
 *
 * If the mesh is composed of triangles, the the returned mesh is already
 * triangulated. If the mesh is composed of quads or polygons, a mesh containing
 * 6 quads will be returned.
 *
 * @return A Mesh containing a ``[min, min+edgeLength]`` Cube.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType, Point3Concept CoordType>
MeshType createCube(const CoordType& min, double edgeLength)
{
	return createHexahedron<MeshType>(min, CoordType(min + edgeLength));
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CREATE_HEXAHEDRON_H
