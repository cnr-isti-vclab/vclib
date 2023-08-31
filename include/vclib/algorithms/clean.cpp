/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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
#include "vclib/concepts/mesh/containers.h"

#include <map>
#include <stack>
#include <vector>

#include <vclib/algorithms/polygon/topology.h>
#include <vclib/algorithms/sort.h>
#include <vclib/mesh/utils/mesh_pos.h>

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

template<typename Cont, typename MeshType>
void setReferencedVerticesOnVector(const MeshType& m, std::vector<bool>& refs, uint& nRefs)
{
	// check if the Cont container of the Mesh has vertex pointers
	if constexpr (comp::HasVertexPointers<typename Cont::ElementType>) {
		// if there are still some vertices non-referenced
		if (nRefs < m.vertexNumber()) {
			constexpr uint ELEM_ID = Cont::ElementType::ELEMENT_ID;
			// for eache element of the Cont container
			for (const typename Cont::ElementType& el : m.template elements<ELEM_ID>()) {
				// for each vertex pointer of the element
				for (const typename Cont::ElementType::VertexType* v : el.vertices()) {
					if (! refs[m.index(v)]) {
						// set the vertex as referenced
						refs[m.index(v)] = true;
						nRefs++;
					}
				}
			}
		}
	}
}


template<typename MeshType, typename... Cont>
void setReferencedVerticesOnVector(const MeshType& m, std::vector<bool>& refs, uint& nRefs, TypeWrapper<Cont...>)
{
	// call the setReferencedVerticesOnVector function for each container of the mesh
	(setReferencedVerticesOnVector<Cont>(m, refs, nRefs), ...);
}

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
std::vector<bool> unreferencedVerticesVectorBool(const MeshType& m, uint& nUnref)
{
	using VertexType = typename MeshType::VertexType;

	uint nRefs = 0;
	std::vector<bool> referredVertices(m.vertexContainerSize(), false);

	setReferencedVerticesOnVector(m, referredVertices, nRefs, typename MeshType::Containers());
	nUnref = m.vertexNumber() - nRefs;

	return referredVertices;
}

/**
 * @brief The SortedIndexContainer class stores a sorted container of indices of type IndexType,
 * plus a Sentinel value.
 *
 * The size of the container can be specified at compile time, or left unspecified with the -1 value
 * (in this case, the container is dynamically allocated).
 *
 * The container provides the operator < and ==, that allow to sort and compare two containers.
 * Two containers with same indices but different Sentinel values are considered equal.
 */
template<typename IndexType, typename SentinelType, int N>
class SortedIndexContainer
{
public:
	SortedIndexContainer() {}

	template<Range RangeType>
	SortedIndexContainer(SentinelType s, RangeType rng) : s(s), v(rng)
	{
		std::sort(v.begin(), v.end());
	}

	bool operator<(const SortedIndexContainer& s) const
	{
		if constexpr (N >= 0) {
			for (uint i = 0; i < N; ++i) {
				if (v[i] != s.v[i])
					return v[i] < s.v[i];
			}
			return false;
		}
		else {
			for (uint i = 0; i < v.size() && i < s.v.size(); ++i) {
				if (v[i] != s.v[i])
					return v[i] < s.v[i];
			}
			return v.size() < s.v.size();
		}
	}

	bool operator==(const SortedIndexContainer& s) const
	{
		if constexpr (N >= 0) {
			for (uint i = 0; i < N; ++i) {
				if (v[i] != s.v[i])
					return false;
			}
			return true;
		}
		else {
			if (v.size() != s.v.size())
				return false;
			for (uint i = 0; i < v.size(); ++i) {
				if (v[i] != s.v[i])
					return false;
			}
			return true;
		}
	}

	SentinelType sentinel() const { return s; }

private:
	vcl::Vector<IndexType, N> v;
	SentinelType s;
};

template<FaceMeshConcept MeshType>
std::vector<bool> nonManifoldVerticesVectorBool(const MeshType& m)
	requires vcl::HasPerFaceAdjacentFaces<MeshType>
{
	vcl::requirePerFaceAdjacentFaces(m);

	using FaceType = typename MeshType::FaceType;

	std::vector<bool> nonManifoldVertices(m.vertexContainerSize(), false);

	std::vector<uint> TD(m.vertexContainerSize(), 0);
	std::vector<bool> nonManifoldInc(m.vertexContainerSize(), false);
	// First Loop, count how many faces are incident on a vertex and store it in TD,
	// and flag how many vertices are incident on non manifold edges.
	for (const FaceType& f : m.faces()) {
		for (uint i = 0; i < f.vertexNumber(); ++i) {
			TD[m.index(f.vertex(i))]++;
			if (!isFaceManifoldOnEdge(f, i)) {
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
 * @brief Returns the number of non-deleted unreferenced vertices of the mesh.
 *
 * This function calculates the number of vertices that are not referenced by any of the
 * elements of the mesh, and which have not been marked as deleted.
 *
 * @tparam MeshType: the type of the input Mesh. It must satisfy the MeshConcept.
 *
 * @param[in] m: The input mesh for which to calculate the number of unreferenced vertices.
 * @return The number of non-deleted unreferenced vertices in the mesh.
 *
 * @ingroup clean
 */
template<MeshConcept MeshType>
uint numberUnreferencedVertices(const MeshType& m)
{
	uint nV = 0;
	// Generate a vector of boolean flags indicating whether each vertex is referenced by any of the
	// mesh's elements.
	std::vector<bool> referredVertices = internal::unreferencedVerticesVectorBool(m, nV);

	return nV;
}

/**
 * @brief Marks as deleted all the non-deleted unreferenced vertices of the mesh.
 *
 * This function marks as deleted all vertices in the input mesh that are not referenced by any of
 * the mesh's elements, and which have not already been marked as deleted.
 *
 * @tparam MeshType The type of the input Mesh. It must satisfy the MeshConcept.
 *
 * @param[in,out] m: The input mesh for which to remove the unreferenced vertices. This mesh will be
 * modified in place, with all unreferenced vertices being marked as deleted.
 *
 * @return The number of non-deleted vertices that were marked as deleted.
 *
 * @ingroup clean
 */
template<MeshConcept MeshType>
uint removeUnreferencedVertices(MeshType& m)
{
	using VertexType = typename MeshType::VertexType;

	// Generate a vector of boolean flags indicating whether each vertex is referenced by any of the
	// mesh's elements.

	uint n = 0;
	std::vector<bool> referredVertices = internal::unreferencedVerticesVectorBool(m, n);

	// need to mark as deleted vertices only if the number of unreferenced is less than vn
	if (n < m.vertexNumber()) {
		// will store on this vector only the indices of the referenced vertices
		std::vector<uint> refVertIndices(m.vertexContainerSize(), UINT_NULL);
		// Iterate over all vertices in the mesh, and mark any unreferenced vertex as deleted.
		for (const VertexType& v : m.vertices()) {
			if (!referredVertices[m.index(v)]) {
				m.deleteVertex(m.index(v));
			}
			else {
				refVertIndices[m.index(v)] = m.index(v);
			}
		}

		// update the vertex indices of the mesh, setting to null the indices of the
		// unreferenced vertices (it may happen on adjacent vertices of some container).
		m.updateVertexIndices(refVertIndices);
	}

	return n;
}

/**
 * @brief Marks as deleted the duplicate vertices of the mesh, by looking only at their spatial
 * positions.
 *
 * This function marks as deleted all vertices in the input mesh that have the same spatial position
 * as another vertex in the mesh. The comparison of vertex positions is based on the `coord()`
 * function of the vertex type, which must return a 3D point representing the vertex coordinates.
 *
 * @tparam MeshType The type of the input Mesh. It must satisfy the MeshConcept.
 *
 * @param[in,out] m: The input mesh for which to remove duplicate vertices. This mesh will be
 * modified in place, with all duplicate vertices being marked as deleted.
 * @return The number of duplicated vertices that were marked as deleted.
 *
 * @ingroup clean
 */
template<MeshConcept MeshType>
uint removeDuplicatedVertices(MeshType& m)
{
	using VertexType    = typename MeshType::VertexType;
	using VertexPointer = typename MeshType::VertexType*;

	if (m.vertexNumber() == 0)
		return 0;

	// a map that will be used to keep track of deleted vertices and their corresponding pointers.
	std::vector<uint> newVertexIndices(m.vertexNumber());
	std::iota(newVertexIndices.begin(), newVertexIndices.end(), 0); // assigning each vertex index to itself.

	uint deleted = 0;

	std::vector<VertexPointer> perm(m.vertexNumber());

	// put all the vertices into a vector for sorting.
	uint k = 0;
	for (VertexType& v : m.vertices())
		perm[k++] = &v;

	// sort the vector based on the vertices' spatial positions.
	std::sort(
		std::execution::par_unseq,
		perm.begin(),
		perm.end(),
		internal::VertPositionComparator<VertexPointer>());

	uint i = 0;

	// compare the i-th position with the next ones while they are equal to the i-th.
	while (i < perm.size() - 1) {
		uint j = i + 1;
		while (j < perm.size() && perm[i]->coord() == perm[j]->coord()) {
			// j will be deleted, so we map its pointer to the i-th vertex's pointer.
			newVertexIndices[m.index(perm[j])] = m.index(perm[i]); // map j into i
			m.deleteVertex(m.index(perm[j]));
			j++;
			deleted++;
		}
		// here perm[i] != perm[j], so we need to check perm[j] with the next vertex.
		i = j;
	}

	// update the vertex pointers to point to the correct vertices, in every container of the mesh
	m.updateVertexIndices(newVertexIndices);

	// todo:
	// - add a flag that removes degenerate elements after
	return deleted;
}

/**
 * @brief Removes all duplicate faces of the mesh by looking only at their vertex references.
 *
 * This function removes all faces in the input mesh that have the same vertex references as another
 * face in the mesh. The comparison of face vertex references is based on the indices of the face
 * vertices, so it assumes that the mesh's vertices have already been unified.
 *
 * @note This function currently only works for triangle meshes. It should be made more general to
 * work for polygonal meshes as well.
 *
 * @note This function does not update any topology relation that could be affected by the removal
 * of duplicate faces, such as the VF or FF relation. Therefore, it is usually performed before
 * building any topology information.
 *
 * @tparam MeshType: The type of the input Mesh. It must satisfy the TriangleMeshConcept.
 *
 * @param[in,out] m: The input mesh for which to remove duplicate faces. This mesh will be modified
 * in place, with all duplicate faces being marked as deleted.
 * @return The number of duplicated faces that were marked as deleted.
 *
 * @ingroup clean
 */
template<FaceMeshConcept MeshType>
uint removeDuplicatedFaces(MeshType& m)
{
	using VertexType = typename MeshType::VertexType;
	using FaceType = typename MeshType::FaceType;

	// create a vector of sorted tuples of indices, where each tuple represents a face's vertices and a
	// pointer to the face.
	std::vector<internal::SortedIndexContainer<VertexType*, FaceType*, FaceType::VERTEX_NUMBER>> fvec;

	for (FaceType& f : m.faces()) {
		fvec.emplace_back(&f, f.vertices());
	}

	// sort the vector based on the face vertex indices.
	std::sort(std::execution::par_unseq, fvec.begin(), fvec.end());
	uint total = 0;

	// iterate over the sorted vector, and mark any duplicate faces as deleted.
	for (uint i = 0; i < fvec.size() - 1; ++i) {
		if (fvec[i] == fvec[i + 1]) {
			total++;
			m.deleteFace(fvec[i].sentinel());
		}
	}
	return total;
}

/**
 * @brief Removes all vertices that have coordinates with invalid floating point values (NaN or
 * inf).
 *
 * This function removes all vertices in the input mesh that have coordinates with invalid floating
 * point values, such as NaN or inf. If the input mesh has faces, and if the flag `deleteAlsoFaces`
 * is set to true, all faces incident on deleted vertices are also deleted.
 *
 * @tparam MeshType: The type of the input Mesh. It must satisfy the MeshConcept.
 *
 * @param[in,out] m: The input mesh for which to remove degenerated vertices. This mesh will be
 * modified in place, with all degenerated vertices being marked as deleted.
 *
 * @param[in] deleteAlsoFaces: If true, all faces incident on deleted vertices will also be deleted.
 *
 * @return The number of degenerated vertices that were marked as deleted.
 *
 * @ingroup clean
 */
template<MeshConcept MeshType>
uint removeDegeneratedVertices(MeshType& m, bool deleteAlsoFaces)
{
	using VertexType = typename MeshType::VertexType;

	int count_vd = 0;

	// iterate over all vertices in the mesh, and mark any with invalid floating point values as
	// deleted.
	for (VertexType& v : m.vertices()) {
		if (v.coord().isDegenerate()) {
			count_vd++;
			m.deleteVertex(&v);
		}
	}

	// If the mesh has faces and the `deleteAlsoFaces` flag is true, delete all faces incident on
	// deleted vertices.
	if constexpr (HasFaces<MeshType>) {
		using FaceType = typename MeshType::FaceType;
		if (deleteAlsoFaces) {
			for (FaceType& f : m.faces()) {
				bool deg = false;
				for (VertexType* v : f.vertices()) {
					if (v->deleted()) {
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
 * @brief Removes all degenerate faces from the input mesh.
 *
 * This function removes all faces in the input mesh that are topologically degenerate, meaning that
 * they have two or more vertex references that link the same vertex. All degenerate faces are zero
 * area faces, but not all zero area faces are degenerate (for example, a face with three different
 * vertex references, but two of them have the same coordinates). Therefore, if you also want to
 * remove these kinds of faces, you should call `removeDuplicatedVertices(m)` first. This function
 * does not adjust topology.
 *
 * @tparam MeshType The type of the input Mesh. It must satisfy the FaceMeshConcept.
 *
 * @param[in,out] m: The input mesh for which to remove degenerate faces. This mesh will be modified
 * in place, with all degenerate faces being marked as deleted.
 *
 * @return The number of degenerate faces that were marked as deleted.
 *
 * @ingroup clean
 */
template<FaceMeshConcept MeshType>
uint removeDegenerateFaces(MeshType& m)
{
	uint count = 0;
	using FaceType = typename MeshType::FaceType;

	// iterate over all faces in the mesh, and mark any that are degenerate as deleted.
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

/**
 * @brief Counts the number of non-manifold vertices in the input mesh.
 *
 * This function counts the number of vertices in the input mesh that are non-manifold, meaning that
 * they are connected to more than two faces. A non-manifold vertex is one that belongs to two or
 * more different edges that are not part of the same face.
 *
 * @tparam MeshType The type of the input Mesh. It must satisfy the FaceMeshConcept.
 *
 * @param[in] m: The input mesh for which to count the number of non-manifold vertices. This mesh
 * will not be modified by the function.
 *
 * @return The number of non-manifold vertices in the input mesh.
 *
 * @ingroup clean
 */
template<FaceMeshConcept MeshType>
uint numberNonManifoldVertices(const MeshType& m)
{
	std::vector<bool> nonManifoldVertices = internal::nonManifoldVerticesVectorBool(m);
	return std::count(nonManifoldVertices.begin(), nonManifoldVertices.end(), true);
}

/**
 * @brief Determines whether the input mesh is water tight.
 *
 * This function performs a simple test of water tightness on the input mesh, checking that there
 * are no boundary and no non-manifold edges, assuming that the mesh is orientable. It could be
 * debated whether a closed non-orientable surface is water tight or not, but this function does not
 * take orientability into account.
 *
 * @tparam MeshType: The type of the input Mesh. It must satisfy the FaceMeshConcept.
 *
 * @param[in] m The input mesh to check for water tightness.
 *
 * @return `true` if the input mesh is water tight (i.e., closed and manifold), `false` otherwise.
 *
 * @ingroup clean
 */
template<FaceMeshConcept MeshType>
bool isWaterTight(const MeshType& m)
{
	uint numEdgeBorder, numNonManifoldEdges;
	internal::numberEdges(m, numEdgeBorder, numNonManifoldEdges);
	return numEdgeBorder == 0 && numNonManifoldEdges == 0;
}

/**
 * @brief Counts the number of holes in the input mesh.
 *
 * This function counts the number of holes in the input mesh, where a hole is defined as a closed
 * loop of border edges. The function uses a depth-first search algorithm to traverse the mesh and
 * find all the holes. The function requires the input MeshType to have per-face adjacent faces, and
 * uses the `vcl::requirePerFaceAdjacentFaces` function to enforce this requirement.
 *
 * @tparam MeshType: The type of the input Mesh. It must satisfy the FaceMeshConcept and have
 * per-face adjacent faces.
 *
 * @param[in] m: The input mesh for which to count the number of holes.
 *
 * @return The number of holes in the input mesh.
 *
 * @ingroup clean
 */
template<FaceMeshConcept MeshType>
uint numberHoles(const MeshType& m)
	requires vcl::HasPerFaceAdjacentFaces<MeshType>
{
	vcl::requirePerFaceAdjacentFaces(m);

	using VertexType = typename MeshType::VertexType;
	using FaceType = typename MeshType::FaceType;

	uint loopNum=0;

	// create a vector of bools to keep track of visited faces.
	std::vector<bool> visitedFaces(m.faceContainerSize(), false);

	// Traverse the mesh using a depth-first search algorithm to find all the holes.
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
 * @brief Computes the connected components of the input mesh based on its topology.
 *
 * This function computes the connected components of the input mesh based on its topology, and
 * returns a vector of sets, where each set represents a connected component and contains the face
 * indices of the mesh that compose it. The function uses a depth-first search algorithm to traverse
 * the mesh and find the connected components. The function requires the input MeshType to have
 * per-face adjacent faces, and uses the `vcl::requirePerFaceAdjacentFaces` function to enforce this
 * requirement.
 *
 * @tparam MeshType: The type of the input Mesh. It must satisfy the FaceMeshConcept and have
 * per-face adjacent faces.
 *
 * @param[in] m: The input mesh for which to compute the connected components.
 * @return A vector of sets representing the connected components of the input mesh. Each set
 * contains the face indices of the mesh that compose a connected component.
 *
 * @ingroup clean
 */
template <FaceMeshConcept MeshType>
std::vector<std::set<uint>> connectedComponents(const MeshType& m)
	requires vcl::HasPerFaceAdjacentFaces<MeshType>
{
	vcl::requirePerFaceAdjacentFaces(m);

	using FaceType = typename MeshType::FaceType;

	std::vector<std::set<uint>> cc;

	// create a vector of bools to keep track of visited faces.
	std::vector<bool> visitedFaces(m.faceContainerSize(), false);

	// create a stack to hold the faces that need to be visited during the depth-first search.
	std::stack<const FaceType*> sf;

	// traverse the mesh using a depth-first search algorithm to find the connected components.
	for (const FaceType& f : m.faces()){
		if (!visitedFaces[m.index(f)]) { // first time I see this face
			visitedFaces[m.index(f)] = true;

			// new connected component
			cc.emplace_back();
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

/**
 * @brief Computes the number of connected components of the input mesh based on its topology.
 *
 * This function computes the number of connected components of the input mesh based on its
 * topology, and returns the result as an unsigned integer. The function simply calls the
 * `connectedComponents` function to compute the connected components and then returns the size of
 * the resulting vector.
 *
 * @tparam MeshType The type of the input Mesh. It must satisfy the FaceMeshConcept and have
 * per-face adjacent faces.
 *
 * @param[in] m: The input mesh for which to compute the number of connected components.
 * @return The number of connected components of the input mesh.
 *
 * @ingroup clean
 */
template <FaceMeshConcept MeshType>
uint numberConnectedComponents(const MeshType& m)
{
	return connectedComponents(m).size();
}

} // namespace vcl
