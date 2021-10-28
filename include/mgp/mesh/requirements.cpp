/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "requirements.h"

#include "container/face_container.h"
#include "container/vertex_container.h"

namespace mgp {

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
		assert(m.isVertexNormalEnabled());
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
