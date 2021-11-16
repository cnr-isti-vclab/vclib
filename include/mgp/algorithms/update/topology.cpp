/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

/**
 * This is a refactoring of VCGLib: https://github.com/cnr-isti-vclab/vcglib/
 * Copyright(C): Visual Computing Lab
 * ISTI - Italian National Research Council
 */

#include "topology.h"

#include <mgp/mesh/requirements.h>

namespace mgp {

/**
 * @brief updatePerVertexAdjacentFaces
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - AdjacentFaces
 *   - Faces
 *
 * @param m
 */
template<typename MeshType>
void updatePerVertexAdjacentFaces(MeshType& m)
{
	mgp::requirePerVertexAdjacentFaces(m);
	mgp::requireFaces<MeshType>();

	using VertexType = typename MeshType::Vertex;
	using FaceType   = typename MeshType::Face;

	for (VertexType& v : m.vertices()) {
		v.clearAdjFaces();
	}

	for (FaceType& f : m.faces()) {
		for (VertexType* v : f.vertices()){
			v->pushAdjFace(&f);
		}
	}
}

}

