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

#include "mesh_requirements.h"

#include "../mesh.h"

namespace vcl {

/********************
 * is/has functions *
 ********************/

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
template<MeshConcept MeshType>
bool isTriangleMesh(const MeshType& m)
{
	if constexpr (HasTriangles<MeshType>) {
		return true;
	}
	else if constexpr (HasFaces<MeshType>) {
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
 * @brief Checks *at run time* if the mesh is composed of quads. If the size of the Face Vertex
 * References is 4, this check is immediate. If it is not 4, the functions checks for each face if
 * it has 4 vertices.
 *
 * Complexity: *O(n)*
 *
 * @param m: the mesh on which check if each face has 4 vertices.
 * @return `true` if every face of the mesh is composed of 4 vertices.
 */
template<MeshConcept MeshType>
bool isQuadMesh(const MeshType& m)
{
	if constexpr (HasQuads<MeshType>) {
		return true;
	}
	else if constexpr (HasFaces<MeshType>) {
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
 * @brief Checks if a Mesh is compact, that is if it does not contains deleted elements.
 * @return `true` if `m` is compact, `false` otherwise.
 */
template<MeshConcept MeshType>
bool isCompact(const MeshType& m)
{
	bool c = true;
	c = c && isVertexContainerCompact(m);
	if constexpr (HasFaces<MeshType>) {
		c = c && isFaceContainerCompact(m);
	}
	if constexpr (HasEdges<MeshType>) {
		c = c && (m.edgeNumber() == m.edgeContainerSize());
	}
	if constexpr (HasHalfEdges<MeshType>) {
		c = c && isHalfEdgeContainerCompact(m);
	}
	return c;
}

/*********************
 * require functions *
 *********************/

template<FaceMeshConcept MeshType>
void requireTriangleMesh(const MeshType& m)
{
	using F = typename MeshType::FaceType;
	for (const F& f : m.faces()) {
		if (f.vertexNumber() != 3) {
			throw MissingTriangularRequirementException("Triangle Mesh Required.");
		}
	}
}

template<FaceMeshConcept MeshType>
void requireQuadMesh(const MeshType& m)
{
	using F = typename MeshType::FaceType;
	for (const F& f : m.faces()) {
		if (f.vertexNumber() != 4) {
			throw MissingQuadRequirementException("Quad Mesh Required.");
		}
	}
}

template <MeshConcept MeshType>
void requireCompactness(const MeshType& m)
{
	if (!isCompact(m))
		throw MissingCompactnessException("Mesh is not compact.");
}

} // namespace vcl
