/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "mesh_requirements.h"

namespace mgp {

// has functions

template<typename MeshType>
bool constexpr isTriangleMesh()
{
	// todo when have lists
	//if constexpr (hasPerFaceVertexReferencesArray<MeshType>()) {
	using F = typename MeshType::FaceType;
	using R = typename F::VertexReferences;
	return R::FACE_SIZE == 3;
	//}
}

template<typename MeshType>
bool constexpr isTriangleMesh(const MeshType&)
{
	return isTriangleMesh<MeshType>();
}

template<typename MeshType>
bool constexpr isQuadMesh()
{
	// todo when have lists
	//if constexpr (hasPerFaceVertexReferencesArray<MeshType>()) {
	using F = typename MeshType::FaceType;
	using R = typename F::VertexReferences;
	return R::FACE_SIZE == 4;
	//}
}

template<typename MeshType>
bool constexpr isQuadMesh(const MeshType&)
{
	return isQuadMesh<MeshType>();
}

// require functions

template<typename MeshType>
void constexpr requireTriangleMesh()
{
	static_assert(isTriangleMesh<MeshType>(), "Mesh is not a Triangle Mesh.");
}

template<typename MeshType>
void constexpr requireTriangleMesh(const MeshType&)
{
	requireTriangleMesh<MeshType>();
}

template<typename MeshType>
void constexpr requireQuadMesh()
{
	static_assert(isQuadMesh<MeshType>(), "Mesh is not a Quad Mesh.");
}

template<typename MeshType>
void constexpr requireQuadMesh(const MeshType&)
{
	requireQuadMesh<MeshType>();
}

}
