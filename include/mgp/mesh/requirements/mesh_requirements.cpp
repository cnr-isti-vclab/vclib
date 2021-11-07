/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "mesh_requirements.h"

#include "../mesh.h"

namespace mgp {

/********************
 * is/has functions *
 ********************/

template<typename MeshType>
bool constexpr hasTriangles()
{
	if constexpr (hasFaces<MeshType>()) {
		using F = typename MeshType::FaceType;
		using R = typename F::VertexReferences;
		return R::VERTEX_NUMBER == 3;
	}
	return false;
}

template<typename MeshType>
bool constexpr hasTriangles(const MeshType&)
{
	return hasTriangles<MeshType>();
}

template<typename MeshType>
bool isTriangleMesh(const MeshType& m)
{
	if constexpr (hasTriangles(m)) {
		return true;
	}
	else {
		using F = typename MeshType::FaceType;
		for (const F& f : m.faceIterator()){
			if (f.vertexNumber() != 3)
				return false;
		}
		return true;
	}
}

template<typename MeshType>
bool constexpr hasQuads()
{
	if constexpr (hasFaces<MeshType>()) {
		using F = typename MeshType::FaceType;
		if constexpr (mgp::face::hasVertexReferences<F>()) {
			using R = typename F::VertexReferences;
			return R::VERTEX_NUMBER == 4;
		}
	}
	return false;
}

template<typename MeshType>
bool constexpr hasQuads(const MeshType&)
{
	return hasQuads<MeshType>();
}

template<typename MeshType>
bool isQuadMesh(const MeshType& m)
{
	if constexpr (hasQuads(m)) {
		return true;
	}
	else {
		using F = typename MeshType::FaceType;
		for (const F& f : m.faceIterator()){
			if (f.vertexNumber() != 4)
				return false;
		}
		return true;
	}
}

template<typename MeshType>
bool constexpr hasBoundingBox()
{
	return mgp::mesh::hasBoundingBox<MeshType>();
}

template<typename MeshType>
bool constexpr hasBoundingBox(const MeshType&)
{
	return hasBoundingBox<MeshType>();
}

/*********************
 * require functions *
 *********************/

template<typename MeshType>
void constexpr requireTriangleMesh()
{
	static_assert(hasTriangles<MeshType>(), "Mesh is not a Triangle Mesh.");
}

template<typename MeshType>
void constexpr requireTriangleMesh(const MeshType&)
{
	requireTriangleMesh<MeshType>();
}

template<typename MeshType>
void constexpr requireQuadMesh()
{
	static_assert(hasQuads<MeshType>(), "Mesh is not a Quad Mesh.");
}

template<typename MeshType>
void constexpr requireQuadMesh(const MeshType&)
{
	requireQuadMesh<MeshType>();
}

template<typename MeshType>
void constexpr requireBoundingBox()
{
	static_assert(hasBoundingBox<MeshType>(), "Mesh has no bounding box.");
}

template<typename MeshType>
void constexpr requireBoundingBox(const MeshType&)
{
	requireBoundingBox<MeshType>();
}

}
