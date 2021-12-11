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

#include "tetrahedron.h"

#include <vclib/mesh/requirements.h>

namespace vcl {

/**
 * @brief createTetrahedron creates a simple tetrahedron mesh with the following point coordinates:
 *
 * ( 1,  1,  1),
 * (-1,  1, -1),
 * (-1, -1,  1),
 * ( 1, -1, -1)
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *   - Faces
 *
 * @return a Mesh containing a Tetrahedron.
 */
template<typename MeshType>
MeshType createTetrahedron()
{
	vcl::requireVertices<MeshType>();
	vcl::requireFaces<MeshType>();

	using CoordType = typename MeshType::Vertex::CoordType;
	return createTetrahedron<MeshType>(
		CoordType(1, 1, 1), CoordType(-1, 1, -1), CoordType(-1, -1, 1), CoordType(1, -1, -1));
}

/**
 * @brief createTetrahedron createTetrahedron creates a simple tetrahedron mesh with the given
 * points. The function assumes that the points p0, p1 and p2 are in counterclockwise order, and
 * does not perform any sanity check about the validity of the points.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *   - Faces
 *
 * @param p0
 * @param p1
 * @param p2
 * @param p3
 * @return a Mesh containing a Tetrahedron.
 */
template<typename MeshType, typename CoordType>
MeshType createTetrahedron(
	const CoordType& p0,
	const CoordType& p1,
	const CoordType& p2,
	const CoordType& p3)
{
	vcl::requireVertices<MeshType>();
	vcl::requireFaces<MeshType>();

	MeshType m;

	m.addVertices(p0, p1, p2, p3);

	m.reserveFaces(4);
	m.addFace(0, 1, 2);
	m.addFace(0, 2, 3);
	m.addFace(0, 3, 1);
	m.addFace(3, 2, 1);

	return m;
}

} // namespace vcl
