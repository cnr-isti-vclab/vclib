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
	mgp::requireVertices<MeshType>();
	mgp::requireFaces<MeshType>();

	using FaceType = typename MeshType::Face;

	MeshType m;

	// if the mesh is polygonal, need first to resize the faces
	if constexpr (FaceType::VERTEX_NUMBER < 0) {
		for (int i = 0; i < 4; ++i)
			m.face(i).resizeVertices(3);
	}

	m.addVertices(p0, p1, p2, p3);

	m.reserveFaces(4);
	m.addFace(&m.vertex(0), &m.vertex(1), &m.vertex(2));
	m.addFace(&m.vertex(0), &m.vertex(2), &m.vertex(3));
	m.addFace(&m.vertex(0), &m.vertex(3), &m.vertex(1));
	m.addFace(&m.vertex(3), &m.vertex(2), &m.vertex(1));

	return m;
}

} // namespace mgp
