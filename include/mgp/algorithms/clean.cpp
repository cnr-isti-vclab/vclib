/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "clean.h"

#include <map>
#include <vector>

#include <mgp/mesh/requirements.h>

namespace mgp {

namespace internal {

/* classe di confronto per l'algoritmo di eliminazione vertici duplicati*/
template <typename VertexPointer>
class VertPositionComparator{
public:
	inline bool operator()(const VertexPointer& a, const VertexPointer& b)
	{
		return (a->coordinate() == b->coordinate()) ? (a<b) : (a->coordinate() < b->coordinate());
	}
};

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

template<typename MeshType>
/**
 * @brief removeDuplicateVertices marks as deleted the duplicate vertices of the mesh, by looking
 * only at their spatial positions.
 *
 *  * Requirements:
 * - Mesh:
 *   - Vertices
 *     - Coordinate
 *   - Faces
 *
 * @param m
 * @return
 */
unsigned int removeDuplicatedVertices(MeshType& m)
{
	using VertexType    = typename MeshType::Vertex;
	using FaceType      = typename MeshType::Face;
	using VertexPointer = typename MeshType::Vertex*;

	if (m.vertexNumber() == 0)
		return 0;

	std::map<VertexPointer, VertexPointer> deletedVertsMap;

	unsigned int deleted  = 0;

	std::vector<VertexPointer> perm(m.vertexNumber());

	// put all the vertices into a vector
	unsigned int k = 0;
	for (VertexType& v : m.vertexIterator())
		perm[k++] = &v;

	// sort the vector based on the verts position
	std::sort(perm.begin(), perm.end(), internal::VertPositionComparator<VertexPointer>());

	unsigned int i = 0;

	// will compare the i-th position with the next ones while they are equal to the i-th
	while(i < perm.size()-1){
		unsigned int j = i+1;
		while (j < perm.size() && perm[i]->coordinate() == perm[j]->coordinate()) {
			// j will be deleted
			deletedVertsMap[perm[j]] = perm[i]; // map j into i
			m.deleteVertex(perm[j]->id());
			j++;
			deleted++;
		}
		// here perm[i] != perm[j]
		// need to check perm[j] with the next, therefore set i for the next iteration
		i = j;
	}

	for (FaceType& f : m.faceIterator()) {
		for (VertexPointer& v : f.vertexIterator()) {
			// if v is in the map of the deleted vertices, need to change its reference
			if (deletedVertsMap.find(v) != deletedVertsMap.end()) {
				v = deletedVertsMap[v];
			}
		}
	}

//	for(EdgeIterator ei = m.edge.begin(); ei!=m.edge.end(); ++ei)
//		if( !(*ei).IsD() )
//			for(k = 0; k < 2; ++k)
//				if( mp.find( (typename MeshType::VertexPointer)(*ei).V(k) ) != mp.end() )
//				{
//					(*ei).V(k) = &*mp[ (*ei).V(k) ];
//				}

//	for (TetraIterator ti = m.tetra.begin(); ti != m.tetra.end(); ++ti)
//		if (!(*ti).IsD())
//			for (k = 0; k < 4; ++k)
//				if (mp.find((typename MeshType::VertexPointer)(*ti).V(k)) != mp.end())
//					(*ti).V(k) = &*mp[ (*ti).V(k) ];

//	if(RemoveDegenerateFlag) RemoveDegenerateFace(m);
//	if(RemoveDegenerateFlag && m.en>0) {
//		RemoveDegenerateEdge(m);
//		RemoveDuplicateEdge(m);
//	}
	return deleted;
}

}
