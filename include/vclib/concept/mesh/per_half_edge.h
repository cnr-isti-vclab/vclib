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

#ifndef VCL_CONCEPT_MESH_PER_HALF_EDGE_H
#define VCL_CONCEPT_MESH_PER_HALF_EDGE_H

#include "container.h"
#include "element/half_edge.h"

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

#endif // VCL_CONCEPT_MESH_PER_HALF_EDGE_H
