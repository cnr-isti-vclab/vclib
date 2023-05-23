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

#ifndef VCL_CONCEPTS_MESH_H
#define VCL_CONCEPTS_MESH_H

#include "mesh/face_mesh_concept.h"
#include "mesh/per_mesh.h"
#include "mesh/per_vertex.h"

/**
 * @defgroup mesh_concepts Mesh Concepts
 * @ingroup lconcepts
 * @ingroup mesh
 *
 * @brief List of concepts for types related to the Mesh data structures of the library. They allow
 * to discriminate between different Mesh types, their elements and the element components.
 */

/**
 * @defgroup element_concepts Element Concepts
 * @ingroup mesh_concepts
 * @ingroup elements
 *
 * @brief List of concepts for types related to the Elements of a Mesh.
 */

namespace vcl {

template<typename T>
concept EdgeMeshConcept =
	MeshConcept<T> && mesh::HasEdgeContainer<T> &&
	requires(
		T o,
		const T& co,
		typename T::EdgeType e)
{
	typename T::EdgeType;
	typename T::EdgeContainer;

	{ co.index(e) } -> std::same_as<uint>;
	{ co.index(&e) } -> std::same_as<uint>;
	{ o.addEdge() } -> std::same_as<uint>;
	{ o.addEdges(uint()) } -> std::same_as<uint>;
	{ o.reserveEdges(uint()) } -> std::same_as<void>;
	{ o.compactEdges() } -> std::same_as<void>;
};

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_H
