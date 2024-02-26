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

#ifndef VCL_CONCEPTS_MESH_PER_EDGE_H
#define VCL_CONCEPTS_MESH_PER_EDGE_H

#include "containers/edge_container.h"
#include "elements/edge.h"

/**
 * @defgroup edge_mesh_concepts EdgeMesh Concepts
 * @ingroup mesh_concepts
 *
 * @brief List of concepts for types related to Meshes having Edges.
 * They allow to check the presence of specific components for Edges.
 */

namespace vcl {

/**
 * @brief Concept that checks if a Mesh has the per Edge AdjacentEdges
 * component.
 *
 * Evaluates to true if AdjacentEdges is part of the Edge element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup edge_mesh_concepts
 */
template<typename MeshType>
concept HasPerEdgeAdjacentEdges =
    HasEdges<MeshType> &&
    vcl::edge::HasAdjacentEdges<typename MeshType::EdgeType>;

/**
 * @brief Concept that checks if a Mesh has the per Edge AdjacentFaces
 * component.
 *
 * Evaluates to true if AdjacentFaces is part of the Edge element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup edge_mesh_concepts
 */
template<typename MeshType>
concept HasPerEdgeAdjacentFaces =
    HasEdges<MeshType> &&
    vcl::edge::HasAdjacentFaces<typename MeshType::EdgeType>;

/**
 * @brief Concept that checks if a Mesh has the per Edge BitFlags component.
 *
 * Evaluates to true if BitFlags is part of the Edge element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup edge_mesh_concepts
 */
template<typename MeshType>
concept HasPerEdgeBitFlags =
    HasEdges<MeshType> && vcl::edge::HasBitFlags<typename MeshType::EdgeType>;

/**
 * @brief Concept that checks if a Mesh has the per Edge Color component.
 *
 * Evaluates to true if Color is part of the Edge element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup edge_mesh_concepts
 */
template<typename MeshType>
concept HasPerEdgeColor =
    HasEdges<MeshType> && vcl::edge::HasColor<typename MeshType::EdgeType>;

/**
 * @brief Concept that checks if a Mesh has the per Edge CustomComponents
 * component.
 *
 * Evaluates to true if CustomComponents is part of the Edge element, whether it
 * is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup edge_mesh_concepts
 */
template<typename MeshType>
concept HasPerEdgeCustomComponents =
    HasEdges<MeshType> &&
    vcl::edge::HasCustomComponents<typename MeshType::EdgeType>;

/**
 * @brief Concept that checks if a Mesh has the per Edge Mark component.
 *
 * Evaluates to true if Mark is part of the Edge element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup edge_mesh_concepts
 */
template<typename MeshType>
concept HasPerEdgeMark =
    HasEdges<MeshType> && vcl::edge::HasMark<typename MeshType::EdgeType>;

/**
 * @brief Concept that checks if a Mesh has the per Edge Quality component.
 *
 * Evaluates to true if Quality is part of the Edge element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup edge_mesh_concepts
 */
template<typename MeshType>
concept HasPerEdgeQuality =
    HasEdges<MeshType> && vcl::edge::HasQuality<typename MeshType::EdgeType>;

/**
 * @brief Concept that checks if a Mesh has the per Edge VertexPointers
 * component.
 *
 * Evaluates to true if VertexPointers is part of the Edge element, whether it
 * is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup edge_mesh_concepts
 */
template<typename MeshType>
concept HasPerEdgeVertexPointers =
    HasEdges<MeshType> &&
    vcl::edge::HasVertexPointers<typename MeshType::EdgeType>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_PER_EDGE_H
