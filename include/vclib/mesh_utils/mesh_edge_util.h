#ifndef VCL_MESH_UTILS_MESH_EDGE_UTIL_H
#define VCL_MESH_UTILS_MESH_EDGE_UTIL_H

#include <vclib/mesh/requirements.h>
#include <vclib/misc/types.h>

namespace vcl {

/*
 * Utility class that stores, for an edge of the mesh:
 * - the index of the edge in the face
 * - the pointer of the face
 * - the pointers of the vertices of the edge
 *
 * edges can be sorted using this class
 */
template<FaceMeshConcept MeshType>
class MeshEdgeUtil
{
public:
	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;

	VertexType* v[2]; // Pointer to the two (ordered) vertices of the edge
	FaceType*   f;    // Pointer to the face of the edge
	int         e;    // Index of the edge inside the face

	MeshEdgeUtil() : v {nullptr, nullptr}, f(nullptr), e(-1) {}
	MeshEdgeUtil(FaceType& pf, uint ne)
	{
		v[0] = pf.vertex(ne);
		v[1] = pf.vertexMod(ne + 1);
		assert(v[0] != v[1]);

		if (v[0] > v[1])
			std::swap(v[0], v[1]);
		f = &pf;
		e = ne;
	}

	bool operator<(const MeshEdgeUtil& pe) const
	{
		if (v[0] < pe.v[0])
			return true;
		else if (v[0] > pe.v[0])
			return false;
		else
			return v[1] < pe.v[1];
	}

	bool operator==(const MeshEdgeUtil& pe) const { return v[0] == pe.v[0] && v[1] == pe.v[1]; }
	bool operator!=(const MeshEdgeUtil& pe) const { return v[0] != pe.v[0] || v[1] != pe.v[1]; }
};

template<FaceMeshConcept MeshType>
class ConstMeshEdgeUtil
{
public:
	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;

	const VertexType* v[2]; // Pointer to the two (ordered) vertices of the edge
	const FaceType*   f;    // Pointer to the face of the edge
	int               e;    // Index of the edge inside the face

	ConstMeshEdgeUtil() : v {nullptr, nullptr}, f(nullptr), e(-1) {}
	ConstMeshEdgeUtil(const FaceType& pf, uint ne)
	{
		v[0] = pf.vertex(ne);
		v[1] = pf.vertexMod(ne + 1);
		assert(v[0] != v[1]);

		if (v[0] > v[1])
			std::swap(v[0], v[1]);
		f = &pf;
		e = ne;
	}

	bool operator<(const ConstMeshEdgeUtil& pe) const
	{
		if (v[0] < pe.v[0])
			return true;
		else if (v[0] > pe.v[0])
			return false;
		else
			return v[1] < pe.v[1];
	}

	bool operator==(const ConstMeshEdgeUtil& pe) const { return v[0] == pe.v[0] && v[1] == pe.v[1]; }
	bool operator!=(const ConstMeshEdgeUtil& pe) const { return v[0] != pe.v[0] || v[1] != pe.v[1]; }
};

} // namespace vcl

#endif // VCL_MESH_UTILS_MESH_EDGE_UTIL_H
