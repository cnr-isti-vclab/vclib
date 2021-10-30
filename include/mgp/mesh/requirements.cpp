/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "requirements.h"

#include <mgp/exception/mesh_exception.h>

#include "container/face_container.h"
#include "container/vertex_container.h"

namespace mgp {

// has functions

template<typename MeshType>
bool constexpr hasVertices()
{
	return mgp::mesh::hasVertices<MeshType>();
}

template<typename MeshType>
bool constexpr hasVertices(const MeshType&)
{
	return hasVertices<MeshType>();
}

template<typename MeshType>
bool constexpr hasFaces()
{
	return mgp::mesh::hasFaces<MeshType>();
}

template<typename MeshType>
bool constexpr hasFaces(const MeshType&)
{
	return hasFaces<MeshType>();
}

// require functions

template<typename MeshType>
void constexpr requireMeshVertices()
{
	static_assert(hasVertices<MeshType>(), "Mesh does not have a Vertex Container.");
}

template<typename MeshType>
void constexpr requireMeshVertices(const MeshType&)
{
	requireMeshVertices<MeshType>();
}

template<typename MeshType>
void constexpr requireMeshFaces()
{
	static_assert(hasFaces<MeshType>(), "Mesh does not have a Face Container.");
}

template<typename MeshType>
void constexpr requireMeshFaces(const MeshType&)
{
	requireMeshFaces<MeshType>();
}

template<typename MeshType>
void constexpr requireTriangleMesh()
{
	requirePerFaceVertexReferences<MeshType>();
	using F = typename MeshType::FaceType;
	using R = typename F::VertexRefsArray;
	static_assert(R::FACE_SIZE == 3, "Mesh is not a Triangle Mesh.");
}

template<typename MeshType>
void constexpr requireTriangleMesh(const MeshType&)
{
	requireTriangleMesh<MeshType>();
}

template<typename MeshType>
void constexpr requireQuadMesh()
{
	requirePerFaceVertexReferences<MeshType>();
	using F = typename MeshType::FaceType;
	using R = typename F::VertexRefsArray;
	static_assert(R::FACE_SIZE == 4, "Mesh is not a Quad Mesh.");
}

template<typename MeshType>
void constexpr requireQuadMesh(const MeshType&)
{
	requireQuadMesh<MeshType>();
}

template<typename MeshType>
void constexpr requirePerVertexCoordinates()
{
	static_assert(
		mgp::vert::hasCoordinate<typename MeshType::VertexType>(),
		"Mesh has no vertex coordinates.");
}

template<typename MeshType>
void constexpr requirePerVertexCoordinates(const MeshType&)
{
	requirePerVertexCoordinates<MeshType>();
}

template<typename MeshType>
void requirePerVertexNormals(const MeshType& m)
{
	static_assert (
		mgp::vert::hasNormal<typename MeshType::VertexType>() ||
			mgp::vert::hasOptionalNormal<typename MeshType::VertexType>(),
		"Mesh has no vertex normals.");
	if constexpr(mgp::vert::hasOptionalNormal<typename MeshType::VertexType>()) {
		if (!m.isVertexNormalEnabled())
			throw mgp::MissingComponentException("Vertex normals not enabled.");
	}
}

template<typename MeshType>
void constexpr requirePerFaceVertexReferences()
{
	static_assert(
		mgp::face::hasVertexRefsArray<typename MeshType::FaceType>(),
		"Mesh has no vertex references.");
}

template<typename MeshType>
void constexpr requirePerFaceVertexReferences(const MeshType&)
{
	requirePerFaceVertexReferences<MeshType>();
}

} // namespace mgp
