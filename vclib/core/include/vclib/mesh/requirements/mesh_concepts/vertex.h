// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_VERTEX_H
#define VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_VERTEX_H

#include "base.h"

namespace vcl {

/* =========================================================================
 * Vertex Component Concepts
 * ========================================================================= */

/* Normals */
template<typename M>
concept HasVertexNormal =
    HasElementComponent<M, ElemId::VERTEX, CompId::NORMAL>;

template<typename M>
concept HasVertexOptionalNormal =
    HasElementOptionalComponent<M, ElemId::VERTEX, CompId::NORMAL>;

/* Colors */
template<typename M>
concept HasVertexColor =
    HasElementComponent<M, ElemId::VERTEX, CompId::COLOR>;

template<typename M>
concept HasVertexOptionalColor =
    HasElementOptionalComponent<M, ElemId::VERTEX, CompId::COLOR>;

/* TexCoords */
template<typename M>
concept HasVertexTexCoord =
    HasElementComponent<M, ElemId::VERTEX, CompId::TEX_COORD>;

template<typename M>
concept HasVertexOptionalTexCoord =
    HasElementOptionalComponent<M, ElemId::VERTEX, CompId::TEX_COORD>;

/* Adjacencies */
template<typename M>
concept HasVertexAdjacentFaces =
    HasElementAdjacentFaces<M, ElemId::VERTEX>;

template<typename M>
concept HasVertexOptionalAdjacentFaces =
    HasElementOptionalAdjacentFaces<M, ElemId::VERTEX>;

template<typename M>
concept HasVertexAdjacentEdges =
    HasElementAdjacentEdges<M, ElemId::VERTEX>;

template<typename M>
concept HasVertexOptionalAdjacentEdges =
    HasElementOptionalAdjacentEdges<M, ElemId::VERTEX>;

template<typename M>
concept HasVertexAdjacentVertices =
    HasElementAdjacentVertices<M, ElemId::VERTEX>;

template<typename M>
concept HasVertexOptionalAdjacentVertices =
    HasElementOptionalAdjacentVertices<M, ElemId::VERTEX>;

/* Quality */
template<typename M>
concept HasVertexQuality =
    HasElementComponent<M, ElemId::VERTEX, CompId::QUALITY>;

template<typename M>
concept HasVertexOptionalQuality =
    HasElementOptionalComponent<M, ElemId::VERTEX, CompId::QUALITY>;

/* PrincipalCurvature */
template<typename M>
concept HasVertexPrincipalCurvature =
    HasElementComponent<M, ElemId::VERTEX, CompId::PRINCIPAL_CURVATURE>;

template<typename M>
concept HasVertexOptionalPrincipalCurvature =
    HasElementOptionalComponent<M, ElemId::VERTEX, CompId::PRINCIPAL_CURVATURE>;

/* Tangent */
template<typename M>
concept HasVertexTangent =
    HasElementComponent<M, ElemId::VERTEX, CompId::TANGENT>;

template<typename M>
concept HasVertexOptionalTangent =
    HasElementOptionalComponent<M, ElemId::VERTEX, CompId::TANGENT>;

/* MaterialIndex */
template<typename M>
concept HasVertexMaterialIndex =
    HasElementComponent<M, ElemId::VERTEX, CompId::MATERIAL_INDEX>;

template<typename M>
concept HasVertexOptionalMaterialIndex =
    HasElementOptionalComponent<M, ElemId::VERTEX, CompId::MATERIAL_INDEX>;

/* Custom Components */
template<typename M>
concept HasVertexCustomComponents =
    HasElementCustomComponents<M, ElemId::VERTEX>;

} // namespace vcl

#endif // VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_VERTEX_H
