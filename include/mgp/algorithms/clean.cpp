/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "clean.h"

#include <vector>

#include <mgp/mesh/requirements.h>

namespace mgp {

/**
 * @brief unreferencedVerticesVectorBool returns a vector of boolean telling, for each vertex of the
 * Mesh m, if it is referenced by any other Element of the Mesh.
 *
 * The size of the vector will be == to the vertexContainerSize of m, and all the deleted vertices
 * are marked as unreferenced by default.
 *
 * @param m
 * @return
 */
namespace internal {
template<typename MeshType>
std::vector<bool> unreferencedVerticesVectorBool(const MeshType& m)
{
	using VertexType = typename MeshType::Vertex;

	std::vector<bool> referredVertices(m.vertexContainerSize(), false);
	if constexpr (mgp::hasFaces(m)){
		using FaceType   = typename MeshType::Face;

		for (const FaceType& f : m.faceIterator()){
			for (const VertexType* v : f.vertexIterator()){
				referredVertices[v->id()] = true;
			}
		}
	}

	return referredVertices;
}

}

/**
 * @brief numberUnreferencedVertices returns the number of non-deleted unreferenced vertices of
 * the mesh, which is the number of vertices that is not referenced by none of the Elements of the
 * Mesh.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *
 * @param m
 * @return the number of unreferenced vertices.
 */
template<typename MeshType>
unsigned int numberUnreferencedVertices(const MeshType& m)
{
	mgp::requireVertices(m);
	std::vector<bool> referredVertices = internal::unreferencedVerticesVectorBool(m);

	unsigned int nV = std::count(referredVertices.begin(), referredVertices.end(), false);

	nV -= m.deletedVertexNumber();

	return nV;
}

/**
 * @brief removeUnreferencedVertices marks as deleted all the non-deleted unreferenced vertices of
 * the mesh, which is the number of vertices that is not referenced by none of the Elements of the
 * Mesh.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *
 * @param m
 * @return the number of removed vertices.
 */
template<typename MeshType>
unsigned int removeUnreferencedVertices(MeshType& m)
{
	mgp::requireVertices(m);

	using VertexType = typename MeshType::Vertex;

	std::vector<bool> referredVertices = internal::unreferencedVerticesVectorBool(m);

	// deleted vertices are automatically jumped
	unsigned int n = 0;
	for (const VertexType& v : m.vertexIterator()) {
		if (!referredVertices[v.id()]){
			m.deleteVertex(v.id());
			++n;
		}
	}

	return n;
}

}
