/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include "containers/vertex_container.h"
#include "elements/vertex.h"

/**
 * @defgroup vertex_mesh_concepts VertexMesh Concepts
 * @ingroup mesh_concepts
 *
 * @brief List of concepts for types related to Meshes having Vertices.
 * They allow to check the presence of specific components for Vertices.
 */

namespace vcl {

/**
 * @brief Concept that checks if a Mesh has the per Vertex AdjacentEdges
 * component.
 *
 * Evaluates to true if AdjacentEdges is part of the Vertex element, whether it
 * is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup vertex_mesh_concepts
 */
template<typename MeshType>
concept HasPerVertexAdjacentEdges =
    HasVertices<MeshType> &&
    vcl::vert::HasAdjacentEdges<typename MeshType::VertexType>;

/**
 * @brief Concept that checks if a Mesh has the per Vertex AdjacentFaces
 * component.
 *
 * Evaluates to true if AdjacentFaces is part of the Vertex element, whether it
 * is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup vertex_mesh_concepts
 */
template<typename MeshType>
concept HasPerVertexAdjacentFaces =
    HasVertices<MeshType> &&
    vcl::vert::HasAdjacentFaces<typename MeshType::VertexType>;

/**
 * @brief Concept that checks if a Mesh has the per Vertex AdjacentVertices
 * component.
 *
 * Evaluates to true if AdjacentVertices is part of the Vertex element, whether
 * it is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup vertex_mesh_concepts
 */
template<typename MeshType>
concept HasPerVertexAdjacentVertices =
    HasVertices<MeshType> &&
    vcl::vert::HasAdjacentVertices<typename MeshType::VertexType>;

/**
 * @brief Concept that checks if a Mesh has the per Vertex Color component.
 *
 * Evaluates to true if Color is part of the Vertex element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup vertex_mesh_concepts
 */
template<typename MeshType>
concept HasPerVertexColor =
    HasVertices<MeshType> && vcl::vert::HasColor<typename MeshType::VertexType>;

/**
 * @brief Concept that checks if a Mesh has the per Vertex Mark component.
 *
 * Evaluates to true if Mark is part of the Vertex element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup vertex_mesh_concepts
 */
template<typename MeshType>
concept HasPerVertexMark =
    HasVertices<MeshType> && vcl::vert::HasMark<typename MeshType::VertexType>;

/**
 * @brief Concept that checks if a Mesh has the per Vertex Normal component.
 *
 * Evaluates to true if Normal is part of the Vertex element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup vertex_mesh_concepts
 */
template<typename MeshType>
concept HasPerVertexNormal =
    HasVertices<MeshType> &&
    vcl::vert::HasNormal<typename MeshType::VertexType>;

/**
 * @brief Concept that checks if a Mesh has the per Vertex PrincipalCurvature
 * component.
 *
 * Evaluates to true if PrincipalCurvature is part of the Vertex element,
 * whether it is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup vertex_mesh_concepts
 */
template<typename MeshType>
concept HasPerVertexPrincipalCurvature =
    HasVertices<MeshType> &&
    vcl::vert::HasPrincipalCurvature<typename MeshType::VertexType>;

/**
 * @brief Concept that checks if a Mesh has the per Vertex Quality component.
 *
 * Evaluates to true if Quality is part of the Vertex element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup vertex_mesh_concepts
 */
template<typename MeshType>
concept HasPerVertexQuality =
    HasVertices<MeshType> &&
    vcl::vert::HasQuality<typename MeshType::VertexType>;

/**
 * @brief Concept that checks if a Mesh has the per Vertex TexCoord component.
 *
 * Evaluates to true if TexCoord is part of the Vertex element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup vertex_mesh_concepts
 */
template<typename MeshType>
concept HasPerVertexTexCoord =
    HasVertices<MeshType> &&
    vcl::vert::HasTexCoord<typename MeshType::VertexType>;

/**
 * @brief Concept that checks if a Mesh has the per Vertex CustomComponents.
 *
 * Evaluates to true if CustomComponents is part of the Vertex element.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup vertex_mesh_concepts
 */
template<typename MeshType>
concept HasPerVertexCustomComponents =
    HasVertices<MeshType> &&
    vcl::vert::HasCustomComponents<typename MeshType::VertexType>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_PER_VERTEX_H
