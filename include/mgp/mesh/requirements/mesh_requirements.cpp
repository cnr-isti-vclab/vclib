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

/**
 * @brief hasTriangles checks *statically* (compile time) if the mesh is composed of triangles
 * (N of Vertex References in the Face Element == 3).
 *
 * If the number of vertex references of the Face Element is dynamic (Polygonal Mesh), this function
 * will return false also if all the faces of your mesh have exactly 3 Vertex references.
 *
 * If you need to check if a Polygonal Mesh is composed only by triangles, you should use the
 * 'isTriangleMesh' function.
 *
 * @return true if the MeshType has the number of Vertex References in the Face Element = 3.
 */
template<typename MeshType>
bool constexpr hasTriangles()
{
	if constexpr (hasFaces<MeshType>()) {
		using F = typename MeshType::Face;
		using R = typename F::VertexReferences;
		return R::VERTEX_NUMBER == 3;
	}
	return false;
}

template<typename MeshType>
bool isTriangleMesh(const MeshType& m)
{
	if constexpr (hasTriangles<MeshType>()) {
		return true;
	}
	else {
		using F = typename MeshType::Face;
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
		using F = typename MeshType::Face;
		if constexpr (mgp::face::hasVertexReferences<F>()) {
			using R = typename F::VertexReferences;
			return R::VERTEX_NUMBER == 4;
		}
	}
	return false;
}

template<typename MeshType>
bool isQuadMesh(const MeshType& m)
{
	if constexpr (hasQuads<MeshType>()) {
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

/*********************
 * require functions *
 *********************/

template<typename MeshType>
void requireTriangleMesh(const MeshType& m)
{
	if constexpr (!hasTriangles<MeshType>()) {
		requireFaces<MeshType>();
		using F = typename MeshType::Face;
		for (const F& f : m.faceIterator()){
			if (f.vertexNumber() != 3){
				throw MissingTriangularRequirementException("Triangle Mesh Required");
			}
		}
	}
}

template<typename MeshType>
void requireQuadMesh(const MeshType& m)
{
	if constexpr (!hasQuads<MeshType>()) {
		requireFaces<MeshType>();
		using F = typename MeshType::Face;
		for (const F& f : m.faceIterator()){
			if (f.vertexNumber() != 4){
				throw MissingQuadRequirementException("Quad Mesh Required");
			}
		}
	}
}

template<typename MeshType>
void constexpr requireBoundingBox()
{
	static_assert(hasBoundingBox<MeshType>(), "Mesh has no bounding box.");
}

}
