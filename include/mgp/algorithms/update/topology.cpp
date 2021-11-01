/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
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
	mgp::requireFaces(m);

	using VertexType = typename MeshType::Vertex;
	using FaceType   = typename MeshType::Face;

	for (VertexType& v : m.vertexIterator()) {
		v.clearFaces();
	}

	for (FaceType& f : m.faceIterator()) {
		for (VertexType* v : f.vertexIterator()){
			v->pushFace(&f);
		}
	}
}

}

