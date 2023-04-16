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

#ifndef VCL_CONCEPTS_MESH_PER_VERTEX_H
#define VCL_CONCEPTS_MESH_PER_VERTEX_H

#include "containers.h"
#include "elements/vertex.h"

namespace vcl {

template<typename MeshType>
concept HasPerVertexAdjacentEdges =
	HasVertices<MeshType> && vcl::vert::HasAdjacentEdges<typename MeshType::VertexType>;

template<typename MeshType>
concept HasPerVertexAdjacentFaces =
	HasVertices<MeshType> && vcl::vert::HasAdjacentFaces<typename MeshType::VertexType>;

template<typename MeshType>
concept HasPerVertexAdjacentVertices =
	HasVertices<MeshType> && vcl::vert::HasAdjacentVertices<typename MeshType::VertexType>;

template<typename MeshType>
concept HasPerVertexColor =
	HasVertices<MeshType> && vcl::vert::HasColor<typename MeshType::VertexType>;

template<typename MeshType>
concept HasPerVertexHalfEdgeReference =
	HasVertices<MeshType> && vcl::vert::HasHalfEdgeReference<typename MeshType::VertexType>;

template<typename MeshType>
concept HasPerVertexMark =
	HasVertices<MeshType> && vcl::vert::HasMark<typename MeshType::VertexType>;

template<typename MeshType>
concept HasPerVertexNormal =
	HasVertices<MeshType> && vcl::vert::HasNormal<typename MeshType::VertexType>;

template<typename MeshType>
concept HasPerVertexPrincipalCurvature =
	HasVertices<MeshType> && vcl::vert::HasPrincipalCurvature<typename MeshType::VertexType>;

template<typename MeshType>
concept HasPerVertexScalar =
	HasVertices<MeshType> && vcl::vert::HasScalar<typename MeshType::VertexType>;

template<typename MeshType>
concept HasPerVertexTexCoord =
	HasVertices<MeshType> && vcl::vert::HasTexCoord<typename MeshType::VertexType>;

template<typename MeshType>
concept HasPerVertexCustomComponents =
	HasVertices<MeshType> && vcl::vert::HasCustomComponents<typename MeshType::VertexType>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_PER_VERTEX_H
