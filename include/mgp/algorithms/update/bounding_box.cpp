/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "bounding_box.h"

#include <mgp/mesh/requirements.h>

namespace mgp {

template<typename MeshType>
void updateBoundingBox(MeshType& m)
{
	using VertexType = typename MeshType::Vertex;

	mgp::requireBoundingBox(m);
	if constexpr (hasVertices(m)) {
		m.boundingBox().setNull();
		for (const VertexType& v : m.vertexIterator()) {
			m.boundingBox().add(v.coordinate());
		}
	}
}

} // namespace mgp
