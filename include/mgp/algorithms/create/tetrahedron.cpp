/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

/**
 * This is a refactoring of VCGLib: https://github.com/cnr-isti-vclab/vcglib/
 * Copyright(C): Visual Computing Lab
 * ISTI - Italian National Research Council
 */

#include "tetrahedron.h"

#include <mgp/mesh/requirements.h>

namespace mgp {

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
	mgp::requireVertices<MeshType>();
	mgp::requireFaces<MeshType>();

	using CoordType = typename MeshType::Vertex::CoordinateType;
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
	mgp::requireVertices<MeshType>();
	mgp::requireFaces<MeshType>();

	using FaceType = typename MeshType::Face;

	MeshType m;

	m.addVertices(4);
	m.addFaces(4);

	// if the mesh is polygonal, need first to resize the faces
	if constexpr (FaceType::VERTEX_NUMBER < 0) {
		for (int i = 0; i < 4; ++i)
			m.face(i).resizeVertices(3);
	}

	m.vertex(0).coordinate() = p0;
	m.vertex(1).coordinate() = p1;
	m.vertex(2).coordinate() = p2;
	m.vertex(3).coordinate() = p3;

	m.face(0).v(0) = &m.vertex(0);
	m.face(0).v(1) = &m.vertex(1);
	m.face(0).v(2) = &m.vertex(2);

	m.face(1).v(0) = &m.vertex(0);
	m.face(1).v(1) = &m.vertex(2);
	m.face(1).v(2) = &m.vertex(3);

	m.face(2).v(0) = &m.vertex(0);
	m.face(2).v(1) = &m.vertex(3);
	m.face(2).v(2) = &m.vertex(1);

	m.face(3).v(0) = &m.vertex(3);
	m.face(3).v(1) = &m.vertex(2);
	m.face(3).v(2) = &m.vertex(1);

	return m;
}

} // namespace mgp
