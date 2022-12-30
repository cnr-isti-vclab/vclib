/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#include "clean.h"

#include <map>
#include <stack>
#include <vector>

#include <vclib/algorithms/sort.h>
#include <vclib/mesh_utils/mesh_pos.h>

namespace vcl {

namespace internal {

/* classe di confronto per l'algoritmo di eliminazione vertici duplicati*/
template<typename VertexPointer>
class VertPositionComparator
{
public:
	inline bool operator()(const VertexPointer& a, const VertexPointer& b)
	{
		return (a->coord() == b->coord()) ? (a < b) : (a->coord() < b->coord());
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
	using VertexType = typename MeshType::VertexType;

	std::vector<bool> referredVertices(m.vertexContainerSize(), false);
	if constexpr (vcl::HasFaces<MeshType>) {
		using FaceType = typename MeshType::FaceType;

		for (const FaceType& f : m.faces()) {
			for (const VertexType* v : f.vertices()) {
				referredVertices[m.index(v)] = true;
			}
		}
	}

	return referredVertices;
}

/**
 * @brief The SortedTriple class stores a sorted triplet of values that implement the operator <
 *
 * @todo this class should become SortedVector, in order to support sorting of Polygonal Faces
 */
template<typename FacePointer>
class SortedTriple
{
public:
	SortedTriple() {}
	SortedTriple(uint v0, uint v1, uint v2, FacePointer _fp)
	{
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
		fp   = _fp;
		std::sort(v, v + 3);
	}
	bool operator<(const SortedTriple& p) const
	{
		return (v[2] != p.v[2]) ? (v[2] < p.v[2]) :
			   (v[1] != p.v[1]) ? (v[1] < p.v[1]) :
                                  (v[0] < p.v[0]);
	}

	bool operator==(const SortedTriple& s) const
	{
		if ((v[0] == s.v[0]) && (v[1] == s.v[1]) && (v[2] == s.v[2]))
			return true;
		return false;
	}

	uint v[3];
	FacePointer  fp;
};

template<FaceMeshConcept MeshType>
std::vector<bool> nonManifoldVerticesVectorBool(const MeshType& m)
	requires vcl::HasPerFaceAdjacentFaces<MeshType>
{
	//vcl::requirePerFaceAdjacentFaces(m);

	using FaceType = typename MeshType::FaceType;

	std::vector<bool> nonManifoldVertices(m.vertexContainerSize(), false);

	std::vector<uint> TD(m.vertexContainerSize(), 0);
	std::vector<bool> nonManifoldInc(m.vertexContainerSize(), false);
	// First Loop, count how many faces are incident on a vertex and store it in TD,
	// and flag how many vertices are incident on non manifold edges.
	for (const FaceType& f : m.faces()) {
		for (uint i = 0; i < f.vertexNumber(); ++i) {
			TD[m.index(f.vertex(i))]++;
			if (!isManifoldOnEdge(f, i)) {
				nonManifoldInc[m.index(f.vertex(i))] = true;
				nonManifoldInc[m.index(f.vertexMod(i+1))] = true;
			}
		}
	}

	std::vector<bool> visited(m.vertexContainerSize(), false);
	for (const FaceType& f : m.faces()) {
		for (uint i = 0; i < f.vertexNumber(); ++i){
			if (!visited[m.index(f.vertex(i))]) {
				visited[m.index(f.vertex(i))] = true;
				MeshPos pos(&f, i);
				uint starSize = pos.numberOfAdjacentFacesToV();
				if (starSize != TD[m.index(f.vertex(i))])
					nonManifoldVertices[m.index(f.vertex(i))] = true;
				}
		}
	}

	return nonManifoldVertices;
}

template<FaceMeshConcept MeshType>
uint numberEdges(const MeshType& m, uint& numBoundaryEdges, uint& numNonManifoldEdges)
{
	std::vector<ConstMeshEdgeUtil<MeshType>> edgeVec = fillAndSortMeshEdgeUtilVector(m);

	uint numEdges = 0;
	numBoundaryEdges = 0;
	numNonManifoldEdges = 0;

	size_t f_on_cur_edge =1;
	for(size_t i=0;i<edgeVec.size();++i) {
		if(( (i+1) == edgeVec.size()) ||  !(edgeVec[i] == edgeVec[i+1])) {
			++numEdges;
			if(f_on_cur_edge==1)
				++numBoundaryEdges;
			if(f_on_cur_edge>2)
				++numNonManifoldEdges;
			f_on_cur_edge=1;
		}
		else {
			++f_on_cur_edge;
		}
	}
	return numEdges;
}

} // namespace internal

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
template<MeshConcept MeshType>
uint numberUnreferencedVertices(const MeshType& m)
{
	std::vector<bool> referredVertices = internal::unreferencedVerticesVectorBool(m);

	uint nV = std::count(VCL_PARALLEL referredVertices.begin(), referredVertices.end(), false);

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
template<MeshConcept MeshType>
uint removeUnreferencedVertices(MeshType& m)
{
	using VertexType = typename MeshType::VertexType;

	std::vector<bool> referredVertices = internal::unreferencedVerticesVectorBool(m);

	// deleted vertices are automatically jumped
	uint n = 0;
	for (const VertexType& v : m.vertices()) {
		if (!referredVertices[m.index(v)]) {
			m.deleteVertex(m.index(v));
			++n;
		}
	}

	return n;
}

/**
 * @brief removeDuplicateVertices marks as deleted the duplicate vertices of the mesh, by looking
 * only at their spatial positions.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *   - Faces
 *
 * @param m
 * @return
 */
template<FaceMeshConcept MeshType>
uint removeDuplicatedVertices(MeshType& m)
{
	using VertexType    = typename MeshType::VertexType;
	using FaceType      = typename MeshType::FaceType;
	using VertexPointer = typename MeshType::VertexType*;

	if (m.vertexNumber() == 0)
		return 0;

	std::map<VertexPointer, VertexPointer> deletedVertsMap;

	uint deleted = 0;

	std::vector<VertexPointer> perm(m.vertexNumber());

	// put all the vertices into a vector
	uint k = 0;
	for (VertexType& v : m.vertices())
		perm[k++] = &v;

	// sort the vector based on the verts position
	std::sort(VCL_PARALLEL perm.begin(), perm.end(), internal::VertPositionComparator<VertexPointer>());

	uint i = 0;

	// will compare the i-th position with the next ones while they are equal to the i-th
	while (i < perm.size() - 1) {
		uint j = i + 1;
		while (j < perm.size() && perm[i]->coord() == perm[j]->coord()) {
			// j will be deleted
			deletedVertsMap[perm[j]] = perm[i]; // map j into i
			m.deleteVertex(m.index(perm[j]));
			j++;
			deleted++;
		}
		// here perm[i] != perm[j]
		// need to check perm[j] with the next, therefore set i for the next iteration
		i = j;
	}

	for (FaceType& f : m.faces()) {
		for (VertexPointer& v : f.vertices()) {
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

/**
 * @brief removeDuplicatedFaces removes all duplicate faces of the mesh by looking only at their
 * vertex reference. So it should be called after unification of vertices. Note that it does not
 * update any topology relation that could be affected by this like the VF or FF relation. The
 * reason this function is usually performed BEFORE building any topology information.
 *
 * @todo make this general for Polygonal Meshes
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *   - Faces
 *
 * @param m
 * @return
 */
template<TriangleMeshConcept MeshType> // TODO: remove this and adjust the function for polymeshes
uint removeDuplicatedFaces(MeshType& m)
{
	using FaceType = typename MeshType::FaceType;

	std::vector<internal::SortedTriple<FaceType*>> fvec;
	for (FaceType& f : m.faces()) {
		fvec.push_back(internal::SortedTriple(m.index(f.vertex(0)), m.index(f.vertex(1)), m.index(f.vertex(2)), &f));
	}
	std::sort(VCL_PARALLEL fvec.begin(), fvec.end());
	uint total = 0;
	for (uint i = 0; i < fvec.size() - 1; ++i) {
		if (fvec[i] == fvec[i + 1]) {
			total++;
			m.deleteFace(fvec[i].fp);
		}
	}
	return total;
}

/**
 * @brief removeDegeneratedVertices removes all the vertices that have coords with invalid
 * floating point values (NaN or inf). If the mesh has faces, and if the flag deleteAlsoFaces is
 * true, all the faces incident on deleted vertices are also deleted.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 * Optional:
 * - Mesh:
 *   - Faces
 *
 * @param m
 * @param deleteAlsoFaces
 * @return
 */
template<MeshConcept MeshType>
uint removeDegeneratedVertices(MeshType& m, bool deleteAlsoFaces)
{
	using VertexType = typename MeshType::VertexType;

	int count_vd = 0;

	for (VertexType& v : m.vertices()) {
		if (v.coord().isDegenerate()) {
			count_vd++;
			m.deleteVertex(&v);
		}
	}

	if constexpr (HasFaces<MeshType>) {
		using FaceType = typename MeshType::FaceType;
		if (deleteAlsoFaces) {
			for (FaceType& f : m.faces()) {
				bool deg = false;
				for (VertexType* v : f.vertices()) {
					if (v->isDeleted()) {
						deg = true;
					}
				}
				if (deg) {
					m.deleteFace(&f);
				}
			}
		}
	}

	return count_vd;
}

/**
 * @brief Removes degenerate faces are faces that are Topologically degenerate, i.e. have two or
 * more vertex reference that link the same vertex. All degenerate faces are zero area faces BUT not
 * all zero area faces are degenerate (e.g. a face with three differente vertex references, but two
 * of them have the same coordinates). Therefore, if you want to remove also these kind of faces you
 * should call `removeDuplicatedVertices(m)` first.
 * This function does not adjust topology.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *   - Faces
 *
 * @param m
 * @return
 */
template<FaceMeshConcept MeshType>
uint removeDegenerateFaces(MeshType& m)
{
	uint count = 0;
	using FaceType = typename MeshType::FaceType;

	for (FaceType& f : m.faces()){
		bool deg = false; // flag to check if a face is degenerate
		for (uint i = 0; i < f.vertexNumber() && !deg; ++i){
			if (f.vertex(i) == f.vertexMod(i+1)){
				deg = true;
				m.deleteFace(m.index(f));
				count++;
			}
		}
	}
	return count;
}

template<typename FaceType>
bool isManifoldOnEdge(const FaceType& f, uint edge)
{
	if (comp::isAdjacentFacesEnabledOn(f)) {
		if (f.adjFace(edge) == nullptr) {
			return true;
		}
		else {
			return f.adjFace(edge)->indexOfAdjFace(&f) >= 0;
		}
	}
	else {
		throw vcl::MissingComponentException("Face has no Adjacent Faces component.");
	}
}


template<FaceMeshConcept MeshType>
uint numberNonManifoldVertices(const MeshType& m)
{
	std::vector<bool> nonManifoldVertices = internal::nonManifoldVerticesVectorBool(m);
	return std::count(nonManifoldVertices.begin(), nonManifoldVertices.end(), true);
}

/**
 * @brief Returns `true` if the Mesh is water tight, that is if closed and manifold.
 *
 * Performs a very simple test of water tightness: checks that there are no boundary and no
 * non-manifold edges, assuming that it is orientable.
 * It could be debated if a closed non orientable surface is watertight or not. This function does
 * not take into account orientability.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *   - Faces
 *
 * @param[in] m: the mesh to check whether is water tight or not.
 * @return `true` if the mesh is water tight, `false` otherwise.
 */
template<FaceMeshConcept MeshType>
bool isWaterTight(const MeshType& m)
{
	uint numEdgeBorder, numNonManifoldEdges;
	internal::numberEdges(m, numEdgeBorder, numNonManifoldEdges);
	return numEdgeBorder == 0 && numNonManifoldEdges == 0;
}

/**
 * @brief Returns the number of holes of the mesh, that is the number of closed loops of border
 * edges.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *   - Faces:
 *     - AdjacentFaces
 *
 * @param[in] m: Mesh on which count the number of holes.
 * @return The number of holes of the mesh.
 */
template<FaceMeshConcept MeshType>
uint numberHoles(const MeshType& m)
	requires vcl::HasPerFaceAdjacentFaces<MeshType>
{
	vcl::requirePerFaceAdjacentFaces(m);

	using VertexType = typename MeshType::VertexType;
	using FaceType = typename MeshType::FaceType;

	uint loopNum=0;

	std::vector<bool> visitedFaces(m.faceContainerSize(), false);
	for(const FaceType& f : m.faces()) {
		uint e = 0;
		for(const VertexType* v : f.vertices()) {
			if(!visitedFaces[m.index(f)] && f.adjFace(e) == nullptr) {
				MeshPos<FaceType> startPos(&f,e);
				MeshPos<FaceType> curPos=startPos;
				do {
					curPos.nextEdgeOnBorderAdjacentToV();
					curPos.flipVertex();
					visitedFaces[m.index(curPos.face())] = true;
				}
				while(curPos!=startPos);
				++loopNum;
			}
			++e;
		}
	}
	return loopNum;
}

/**
 * @brief Computes the connected components of a Mesh based on its topology. This function returns
 * a vector of sets, each one of these sets represents a connected component and contains the face
 * indices of the mesh that compose it.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - AdjacentFaces
 *
 * @param[in] m: Mesh on which compute the connected components.
 * @return A vector of sets representing the connected components of the mesh.
 */
template <FaceMeshConcept MeshType>
std::vector<std::set<uint>> connectedComponents(const MeshType& m)
	requires vcl::HasPerFaceAdjacentFaces<MeshType>
{
	vcl::requirePerFaceAdjacentFaces(m);

	using FaceType = typename MeshType::FaceType;

	std::vector<std::set<uint>> cc;

	std::vector<bool> visitedFaces(m.faceContainerSize(), false);

	std::stack<const FaceType*> sf;
	for (const FaceType& f : m.faces()){
		if (!visitedFaces[m.index(f)]) { // first time I see this face
			visitedFaces[m.index(f)] = true;

			// new connected component
			cc.push_back(std::set<uint>());
			std::set<uint>& ccf = cc[cc.size()-1];
			ccf.insert(m.index(f));

			// while the stack is empty, visit the adjacent faces of the top face of the stack
			sf.push(&f);
			while(!sf.empty()){
				const FaceType* fpt = sf.top();
				sf.pop(); // remove the top face and add it to the connected component
				ccf.insert(m.index(fpt));

				// add the adjacent faces of the current visited in the stack
				for (uint j = 0; j < fpt->vertexNumber(); ++j){
					const FaceType* adjf = fpt->adjFace(j);
					// if there is an adj face and it has not been visited
					if (adjf != nullptr && !visitedFaces[m.index(adjf)]){
						sf.push(adjf);
						visitedFaces[m.index(adjf)] = true;
					}
				}
			}
		}
	}
	return cc;
}

template <FaceMeshConcept MeshType>
uint numberConnectedComponents(const MeshType& m)
{
	return connectedComponents(m).size();
}

} // namespace vcl
