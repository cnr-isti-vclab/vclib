/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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
		for (const F& f : m.faces()) {
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
		if constexpr (vcl::face::hasVertexReferences<F>()) {
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
		for (const F& f : m.faces()) {
			if (f.vertexNumber() != 4)
				return false;
		}
		return true;
	}
}

template<typename MeshType>
bool constexpr hasBoundingBox()
{
	return vcl::mesh::hasBoundingBox<MeshType>();
}

template<typename MeshType>
bool constexpr hasMark()
{
	return vcl::mesh::hasMark<MeshType>();
}

template<typename MeshType>
bool constexpr hasTransformMatrix()
{
	return vcl::mesh::hasTransformMatrix<MeshType>();
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
		using F = typename MeshType::Face;
		for (const F& f : m.faces()) {
			if (f.vertexNumber() != 4) {
				throw MissingQuadRequirementException("Quad Mesh Required.");
			}
		}
	}
}

template<typename MeshType>
void constexpr requireBoundingBox()
{
	static_assert(hasBoundingBox<MeshType>(), "Mesh has no bounding box.");
}

template<typename MeshType>
void constexpr requireMark()
{
	static_assert(hasMark<MeshType>(), "Mesh has no mark.");
}

template<typename MeshType>
void constexpr requireTransformMatrix()
{
	static_assert(hasTransformMatrix<MeshType>(), "Mesh has no transform matrix.");
}

} // namespace vcl
