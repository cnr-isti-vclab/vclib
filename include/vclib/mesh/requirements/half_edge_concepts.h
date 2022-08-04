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

#ifndef VCL_MESH_HALF_EDGE_CONCEPTS_H
#define VCL_MESH_HALF_EDGE_CONCEPTS_H

#include "element_concepts.h"
#include "../containers/half_edge_container.h"

namespace vcl {

template<typename MeshType>
concept HasPerHalfEdgeColor =
	HasHalfEdges<MeshType> && vcl::hedge::HasColor<typename MeshType::HalfEdgeType>;

template<typename MeshType>
concept HasPerHalfCustomComponents =
	HasHalfEdges<MeshType> && vcl::hedge::HasCustomComponents<typename MeshType::HalfEdgeType>;

template<typename MeshType>
concept HasPerHalfEdgeMark =
	HasHalfEdges<MeshType> && vcl::hedge::HasMark<typename MeshType::HalfEdgeType>;

template<typename MeshType>
concept HasPerHalfEdgeScalar =
	HasHalfEdges<MeshType> && vcl::hedge::HasScalar<typename MeshType::HalfEdgeType>;


template<typename MeshType>
concept HasPerHalfEdgeTexCoord =
	HasHalfEdges<MeshType> && vcl::hedge::HasTexCoord<typename MeshType::HalfEdgeType>;

} // namespace vcl

#endif // VCL_MESH_HALF_EDGE_CONCEPTS_H
