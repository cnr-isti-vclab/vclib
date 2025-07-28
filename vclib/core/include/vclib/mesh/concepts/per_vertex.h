/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MESH_CONCEPTS_PER_VERTEX_H
#define VCL_MESH_CONCEPTS_PER_VERTEX_H

#include "mesh_concept.h"

#include <vclib/mesh/containers/vertex_container.h>

#include <vclib/concepts.h>

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
    vert::HasAdjacentEdges<typename RemoveRef<MeshType>::VertexType>;

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
    vert::HasAdjacentFaces<typename RemoveRef<MeshType>::VertexType>;

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
    vert::HasAdjacentVertices<typename RemoveRef<MeshType>::VertexType>;

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
    HasVertices<MeshType> &&
    vert::HasColor<typename RemoveRef<MeshType>::VertexType>;

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
    HasVertices<MeshType> &&
    vert::HasMark<typename RemoveRef<MeshType>::VertexType>;

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
    vert::HasNormal<typename RemoveRef<MeshType>::VertexType>;

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
    vert::HasPrincipalCurvature<typename RemoveRef<MeshType>::VertexType>;

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
    vert::HasQuality<typename RemoveRef<MeshType>::VertexType>;

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
    vert::HasTexCoord<typename RemoveRef<MeshType>::VertexType>;

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
    vert::HasCustomComponents<typename RemoveRef<MeshType>::VertexType>;

} // namespace vcl

#endif // VCL_MESH_CONCEPTS_PER_VERTEX_H
