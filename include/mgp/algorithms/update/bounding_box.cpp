/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "bounding_box.h"

#include <mgp/mesh/requirements.h>

namespace mgp {

/**
 * @brief updateBoundingBox updates the bounding box of the mesh.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *   - BoundingBox
 *
 * @param m
 */
template<typename MeshType>
void updateBoundingBox(MeshType& m)
{
	mgp::requireVertices<MeshType>();
	mgp::requireBoundingBox<MeshType>();

	using VertexType = typename MeshType::Vertex;

	m.boundingBox().setNull();
	for (const VertexType& v : m.vertexIterator()) {
		m.boundingBox().add(v.coord());
	}
}

} // namespace mgp
