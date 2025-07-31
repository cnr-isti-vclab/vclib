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

#ifndef VCL_MESH_REQUIREMENTS_VERTEX_REQUIREMENTS_H
#define VCL_MESH_REQUIREMENTS_VERTEX_REQUIREMENTS_H

#include "../containers/vertex_container.h"
#include "element_requirements.h"

#include <vclib/exceptions.h>

/**
 * @defgroup vertex_requirements Vertex Requirements
 * @ingroup mesh_requirements
 *
 * @brief List functions that allows to check, enable and assert the
 * availability of components in the Vertex Elements of a Mesh.
 */

/**
 * @defgroup vertex_mesh_concepts VertexMesh Concepts
 * @ingroup vertex_requirements
 *
 * @brief List of concepts for types related to Meshes having Vertices.
 * They allow to check the presence of specific components for Vertices.
 */

namespace vcl {

/************
 * concepts *
 ************/

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

/*************************
 * is/enableIf functions *
 *************************/

/**
 * @brief Returns `true` if the given mesh has its Vertex Container compact.
 *
 * @tparam MeshType: type of the mesh. It must satisfy the MeshConcept.
 * @param[in] m: input mesh on which test if the its Vertex Container is
 * compact.
 * @return `true` if the Vertex Container of the mesh is compact.
 *
 * @ingroup vertex_requirements
 */
template<MeshConcept MeshType>
bool isVertexContainerCompact(const MeshType& m)
{
    return isElementContainerCompact<ElemId::VERTEX>(m);
}

template<MeshConcept MeshType>
bool isPerVertexAdjacentFacesAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<
        ElemId::VERTEX,
        CompId::ADJACENT_FACES>(m);
}

template<MeshConcept MeshType>
bool enableIfPerVertexAdjacentFacesOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<
        ElemId::VERTEX,
        CompId::ADJACENT_FACES>(m);
}

template<MeshConcept MeshType>
bool isPerVertexAdjacentVerticesAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<
        ElemId::VERTEX,
        CompId::ADJACENT_VERTICES>(m);
}

template<MeshConcept MeshType>
bool enableIfPerVertexAdjacentVerticesOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<
        ElemId::VERTEX,
        CompId::ADJACENT_VERTICES>(m);
}

template<MeshConcept MeshType>
bool isPerVertexColorAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<ElemId::VERTEX, CompId::COLOR>(m);
}

template<MeshConcept MeshType>
bool enableIfPerVertexColorOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<ElemId::VERTEX, CompId::COLOR>(
        m);
}

template<MeshConcept MeshType>
bool isPerVertexMarkAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<ElemId::VERTEX, CompId::MARK>(m);
}

template<MeshConcept MeshType>
bool enableIfPerVertexMarkOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<ElemId::VERTEX, CompId::MARK>(m);
}

template<MeshConcept MeshType>
bool isPerVertexNormalAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>(m);
}

template<MeshConcept MeshType>
bool enableIfPerVertexNormalOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<ElemId::VERTEX, CompId::NORMAL>(
        m);
}

template<MeshConcept MeshType>
bool isPerVertexPrincipalCurvatureAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<
        ElemId::VERTEX,
        CompId::PRINCIPAL_CURVATURE>(m);
}

template<MeshConcept MeshType>
bool enableIfPerVertexPrincipalCurvatureOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<
        ElemId::VERTEX,
        CompId::PRINCIPAL_CURVATURE>(m);
}

template<MeshConcept MeshType>
bool isPerVertexQualityAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<ElemId::VERTEX, CompId::QUALITY>(m);
}

template<MeshConcept MeshType>
bool enableIfPerVertexQualityOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<ElemId::VERTEX, CompId::QUALITY>(
        m);
}

template<MeshConcept MeshType>
bool isPerVertexTexCoordAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<ElemId::VERTEX, CompId::TEX_COORD>(m);
}

template<MeshConcept MeshType>
bool enableIfPerVertexTexCoordOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<
        ElemId::VERTEX,
        CompId::TEX_COORD>(m);
}

/*********************
 * require functions *
 *********************/

template<MeshConcept MeshType>
void requireVertexContainerCompactness(const MeshType& m)
{
    requireElementContainerCompactness<ElemId::VERTEX>(m);
}

template<typename MeshType>
void requirePerVertexAdjacentFaces(const MeshType& m)
    requires HasPerVertexAdjacentFaces<MeshType>
{
    requirePerElementComponent<ElemId::VERTEX, CompId::ADJACENT_FACES>(m);
}

template<typename MeshType>
void requirePerVertexAdjacentVertices(const MeshType& m)
    requires HasPerVertexAdjacentVertices<MeshType>
{
    requirePerElementComponent<ElemId::VERTEX, CompId::ADJACENT_VERTICES>(m);
}

template<typename MeshType>
void requirePerVertexColor(const MeshType& m)
    requires HasPerVertexColor<MeshType>
{
    requirePerElementComponent<ElemId::VERTEX, CompId::COLOR>(m);
}

template<typename MeshType>
void requirePerVertexMark(const MeshType& m) requires HasPerVertexMark<MeshType>
{
    requirePerElementComponent<ElemId::VERTEX, CompId::MARK>(m);
}

template<typename MeshType>
void requirePerVertexNormal(const MeshType& m)
    requires HasPerVertexNormal<MeshType>
{
    requirePerElementComponent<ElemId::VERTEX, CompId::NORMAL>(m);
}

template<typename MeshType>
void requirePerVertexPrincipalCurvature(const MeshType& m)
    requires HasPerVertexPrincipalCurvature<MeshType>
{
    requirePerElementComponent<ElemId::VERTEX, CompId::PRINCIPAL_CURVATURE>(m);
}

template<typename MeshType>
void requirePerVertexQuality(const MeshType& m)
    requires HasPerVertexQuality<MeshType>
{
    requirePerElementComponent<ElemId::VERTEX, CompId::QUALITY>(m);
}

template<typename MeshType>
void requirePerVertexTexCoord(const MeshType& m)
    requires HasPerVertexTexCoord<MeshType>
{
    requirePerElementComponent<ElemId::VERTEX, CompId::TEX_COORD>(m);
}

} // namespace vcl

#endif // VCL_MESH_REQUIREMENTS_VERTEX_REQUIREMENTS_H
