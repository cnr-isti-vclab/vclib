/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "mesh_requirements.h"

#include "../mesh.h"

namespace vcl {

/********************
 * is/has functions *
 ********************/

/**
 * @brief Checks *if a MeshType is composed of triangles* (N of Vertex References in
 * the Face Element == 3).
 *
 * If you need to check if an instance of a MeshType contains only triangles (also if it is a
 * polygon mesh), you should use the isTriangleMesh(m) function.
 *
 * Usage:
 *
 * @code{.cpp}
 * if constexpr(vcl::hasTriangles<MyMeshType>()) {
 *    // this portion of code is compiled only when MyMeshType has triangle faces, N == 3
 * }
 * @endcode
 *
 * @return `true` if the MeshType has the number of Vertex References in the Face Element = 3.
 */
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

/**
 * @brief Checks *at run time* if the mesh m is composed of triangles. If the size of the Face
 * Vertex References is 3, this check is immediate. If it is not 3, the functions checks for each
 * face if it has 3 vertices.
 *
 * Complexity: *O(n)*
 *
 * @param m: the mesh on which check if each face has 3 vertices.
 * @return `true` if every face of the mesh is composed of 3 vertices.
 */
template<typename MeshType>
bool isTriangleMesh(const MeshType& m)
{
	if constexpr (hasTriangles<MeshType>()) {
		return true;
	}
	else if constexpr (hasFaces<MeshType>()) {
		using F = typename MeshType::FaceType;
		for (const F& f : m.faces()) {
			if (f.vertexNumber() != 3)
				return false;
		}
		return true;
	}
	else {
		return false;
	}
}

/**
 * @brief Checks *if a MeshType is composed of quads* (N of Vertex References in
 * the Face Element == 4).
 *
 * If you need to check if an instance of a MeshType contains only quads (also if it is a
 * polygon mesh), you should use the isQuadMesh(m) function.
 *
 * Usage:
 *
 * @code{.cpp}
 * if constexpr(vcl::hasQuads<MyMeshType>()) {
 *    // this portion of code is compiled only when MyMeshType has quad faces, N == 4
 * }
 * @endcode
 *
 * @return `true` if the MeshType has the number of Vertex References in the Face Element = 4.
 */
template<typename MeshType>
bool constexpr hasQuads()
{
	if constexpr (hasFaces<MeshType>()) {
		using F = typename MeshType::FaceType;
		if constexpr (vcl::face::hasVertexReferences<F>()) {
			using R = typename F::VertexReferences;
			return R::VERTEX_NUMBER == 4;
		}
	}
	return false;
}

/**
 * @brief Checks *at run time* if the mesh is composed of quads. If the size of the Face Vertex
 * References is 4, this check is immediate. If it is not 4, the functions checks for each face if
 * it has 4 vertices.
 *
 * Complexity: *O(n)*
 *
 * @param m: the mesh on which check if each face has 4 vertices.
 * @return `true` if every face of the mesh is composed of 4 vertices.
 */
template<typename MeshType>
bool isQuadMesh(const MeshType& m)
{
	if constexpr (hasQuads<MeshType>()) {
		return true;
	}
	else if constexpr (hasFaces<MeshType>()) {
		using F = typename MeshType::FaceType;
		for (const F& f : m.faces()) {
			if (f.vertexNumber() != 4)
				return false;
		}
		return true;
	}
	else {
		return false;
	}
}

/**
 * @brief Checks *if a MeshType is composed of polygons* (N of Vertex References in
 * the Face Element < 0).
 *
 * Usage:
 *
 * @code{.cpp}
 * if constexpr(vcl::hasPolygons<MyMeshType>()) {
 *    // this portion of code is compiled only when MyMeshType has polygon faces, N < 0
 * }
 * @endcode
 *
 * @return `true` if the MeshType has the number of Vertex References in the Face Element < 0.
 */
template<typename MeshType>
bool constexpr hasPolygons()
{
	if constexpr (hasFaces<MeshType>()) {
		using F = typename MeshType::FaceType;
		if constexpr (vcl::face::hasVertexReferences<F>()) {
			using R = typename F::VertexReferences;
			return R::VERTEX_NUMBER < 0;
		}
	}
	return false;
}

/**
 * @brief Checks if a Mesh is compact, that is if it does not contains deleted elements.
 * @return `true` if `m` is compact, `false` otherwise.
 */
template<typename MeshType>
bool isCompact(const MeshType& m)
{
	bool c = true;
	if constexpr (HasVertices<MeshType>) {
		c = c && isVertexContainerCompact(m);
	}
	if constexpr (HasFaces<MeshType>) {
		c = c && isFaceContainerCompact(m);
	}
	if constexpr (HasEdges<MeshType>) {
		c = c && (m.edgeNumber() == m.edgeContainerSize());
	}
	if constexpr (HasHalfEdges<MeshType>) {
		c = c && (m.halfEdgeNumber() == m.halfEdgeContainerSize());
	}
	return c;
}

/*********************
 * require functions *
 *********************/

template<typename MeshType>
void requireTriangleMesh(const MeshType& m)
{
	if constexpr (!hasTriangles<MeshType>()) {
		requireFaces<MeshType>();
		using F = typename MeshType::FaceType;
		for (const F& f : m.faces()) {
			if (f.vertexNumber() != 3) {
				throw MissingTriangularRequirementException("Triangle Mesh Required.");
			}
		}
	}
}

template<typename MeshType>
void requireQuadMesh(const MeshType& m)
{
	if constexpr (!hasQuads<MeshType>()) {
		requireFaces<MeshType>();
		using F = typename MeshType::FaceType;
		for (const F& f : m.faces()) {
			if (f.vertexNumber() != 4) {
				throw MissingQuadRequirementException("Quad Mesh Required.");
			}
		}
	}
}

template <typename MeshType>
void requireCompactness(const MeshType& m)
{
	if (!isCompact(m))
		throw MissingCompactnessException("Mesh is not compact.");
}

} // namespace vcl
