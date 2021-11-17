/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

/**
 * This is a refactoring of VCGLib: https://github.com/cnr-isti-vclab/vcglib/
 * Copyright(C): Visual Computing Lab
 * ISTI - Italian National Research Council
 */

#include "clean.h"

#include <map>
#include <vector>

#include <mgp/mesh/requirements.h>

namespace mgp {

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
	using VertexType = typename MeshType::Vertex;

	std::vector<bool> referredVertices(m.vertexContainerSize(), false);
	if constexpr (mgp::hasFaces<MeshType>()) {
		using FaceType = typename MeshType::Face;

		for (const FaceType& f : m.faces()) {
			for (const VertexType* v : f.vertices()) {
				referredVertices[v->id()] = true;
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
	SortedTriple(unsigned int v0, unsigned int v1, unsigned int v2, FacePointer _fp)
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

	unsigned int v[3];
	FacePointer  fp;
};
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
template<typename MeshType>
unsigned int numberUnreferencedVertices(const MeshType& m)
{
	mgp::requireVertices<MeshType>();

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
	mgp::requireVertices<MeshType>();

	using VertexType = typename MeshType::Vertex;

	std::vector<bool> referredVertices = internal::unreferencedVerticesVectorBool(m);

	// deleted vertices are automatically jumped
	unsigned int n = 0;
	for (const VertexType& v : m.vertices()) {
		if (!referredVertices[v.id()]) {
			m.deleteVertex(v.id());
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
template<typename MeshType>
unsigned int removeDuplicatedVertices(MeshType& m)
{
	mgp::requireVertices<MeshType>();
	mgp::requireFaces<MeshType>();

	using VertexType    = typename MeshType::Vertex;
	using FaceType      = typename MeshType::Face;
	using VertexPointer = typename MeshType::Vertex*;

	if (m.vertexNumber() == 0)
		return 0;

	std::map<VertexPointer, VertexPointer> deletedVertsMap;

	unsigned int deleted = 0;

	std::vector<VertexPointer> perm(m.vertexNumber());

	// put all the vertices into a vector
	unsigned int k = 0;
	for (VertexType& v : m.vertices())
		perm[k++] = &v;

	// sort the vector based on the verts position
	std::sort(perm.begin(), perm.end(), internal::VertPositionComparator<VertexPointer>());

	unsigned int i = 0;

	// will compare the i-th position with the next ones while they are equal to the i-th
	while (i < perm.size() - 1) {
		unsigned int j = i + 1;
		while (j < perm.size() && perm[i]->coord() == perm[j]->coord()) {
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
template<typename MeshType>
unsigned int removeDuplicatedFaces(MeshType& m)
{
	mgp::requireVertices<MeshType>();
	mgp::requireTriangleMesh(m); // TODO: remove this and adjust the function for polymeshes

	using FaceType = typename MeshType::Face;

	std::vector<internal::SortedTriple<FaceType*>> fvec;
	for (FaceType& f : m.faces()) {
		fvec.push_back(internal::SortedTriple(f.vertex(0)->id(), f.vertex(1)->id(), f.vertex(2)->id(), &f));
	}
	std::sort(fvec.begin(), fvec.end());
	unsigned int total = 0;
	for (unsigned int i = 0; i < fvec.size() - 1; ++i) {
		if (fvec[i] == fvec[i + 1]) {
			total++;
			m.deleteFace(fvec[i].fp->id());
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
template<typename MeshType>
unsigned int removeDegeneratedVertices(MeshType& m, bool deleteAlsoFaces)
{
	using VertexType = typename MeshType::Vertex;

	int count_vd = 0;

	for (VertexType& v : m.vertices()) {
		if (v.coord().isDegenerate()) {
			count_vd++;
			m.deleteVertex(v.id());
		}
	}

	if constexpr (hasFaces(m)) {
		using FaceType = typename MeshType::Face;
		if (deleteAlsoFaces) {
			for (FaceType& f : m.faces()) {
				bool deg = false;
				for (VertexType* v : f.vertices()) {
					if (v->isDeleted()) {
						deg = true;
					}
				}
				if (deg) {
					m.deleteFace(f.id());
				}
			}
		}
	}

	return count_vd;
}

} // namespace mgp
