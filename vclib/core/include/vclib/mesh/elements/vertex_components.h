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

#ifndef VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_H
#define VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_H

#include "../components/adjacent_edges.h"
#include "../components/adjacent_faces.h"
#include "../components/adjacent_vertices.h"
#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/custom_components.h"
#include "../components/mark.h"
#include "../components/material_index.h"
#include "../components/normal.h"
#include "../components/parent_mesh_pointer.h"
#include "../components/position.h"
#include "../components/principal_curvature.h"
#include "../components/quality.h"
#include "../components/tangent.h"
#include "../components/tex_coord.h"

/**
 * @defgroup vert_components Vertex Components
 * @ingroup components
 *
 * @brief This group lists all the Components that may be used by the Vertex
 * element.
 *
 * All these components are listed inside the vcl::vert namespace.
 */

/**
 * @defgroup vert_concepts Vertex Concepts
 * @ingroup element_concepts
 * @ingroup vert_components
 *
 * @brief List of concepts for types related to the Vertex Element.
 */

namespace vcl::vert {

/**
 * @ingroup vert_concepts vert_components
 * @{
 */

/* Port concepts into the vert namespace */
template<typename T>
concept HasAdjacentEdges = comp::HasAdjacentEdges<T>;
template<typename T>
concept HasOptionalAdjacentEdges = comp::HasOptionalAdjacentEdges<T>;
template<typename T>
concept HasAdjacentFaces = comp::HasAdjacentFaces<T>;
template<typename T>
concept HasOptionalAdjacentFaces = comp::HasOptionalAdjacentFaces<T>;
template<typename T>
concept HasAdjacentVertices = comp::HasAdjacentVertices<T>;
template<typename T>
concept HasOptionalAdjacentVertices = comp::HasOptionalAdjacentVertices<T>;
template<typename T>
concept HasBitFlags = comp::HasBitFlags<T>;
template<typename T>
concept HasColor = comp::HasColor<T>;
template<typename T>
concept HasOptionalColor = comp::HasOptionalColor<T>;
template<typename T>
concept HasCustomComponents = comp::HasCustomComponents<T>;
template<typename T>
concept HasPosition = comp::HasPosition<T>;
template<typename T>
concept HasMark = comp::HasMark<T>;
template<typename T>
concept HasOptionalMark = comp::HasOptionalMark<T>;
template<typename T>
concept HasMaterialIndex = comp::HasMaterialIndex<T>;
template<typename T>
concept HasOptionalMaterialIndex = comp::HasOptionalMaterialIndex<T>;
template<typename T>
concept HasNormal = comp::HasNormal<T>;
template<typename T>
concept HasOptionalNormal = comp::HasOptionalNormal<T>;
template<typename T>
concept HasPrincipalCurvature = comp::HasPrincipalCurvature<T>;
template<typename T>
concept HasOptionalPrincipalCurvature = comp::HasOptionalPrincipalCurvature<T>;
template<typename T>
concept HasQuality = comp::HasQuality<T>;
template<typename T>
concept HasOptionalQuality = comp::HasOptionalQuality<T>;
template<typename T>
concept HasTangent = comp::HasTangent<T>;
template<typename T>
concept HasOptionalTangent = comp::HasOptionalTangent<T>;
template<typename T>
concept HasTexCoord = comp::HasTexCoord<T>;
template<typename T>
concept HasOptionalTexCoord = comp::HasOptionalTexCoord<T>;

/** @} */ // end of group

/**
 * @addtogroup vert_components
 * @{
 */

/* Port AdjacentEdges class into vert namespace */
template<bool INDEXED, typename EdgeType, typename ParentVertexType>
using AdjacentEdges =
    comp::AdjacentEdges<INDEXED, EdgeType, -1, false, ParentVertexType>;

template<bool INDEXED, typename EdgeType, typename ParentVertexType>
using VerticalAdjacentEdges =
    comp::AdjacentEdges<INDEXED, EdgeType, -1, false, ParentVertexType, true>;

template<bool INDEXED, typename EdgeType, typename ParentVertexType>
using OptionalAdjacentEdges = comp::
    AdjacentEdges<INDEXED, EdgeType, -1, false, ParentVertexType, true, true>;

template<typename EdgeType, typename ParentVertexType>
using AdjacentEdgeIndices =
    comp::AdjacentEdges<true, EdgeType, -1, false, ParentVertexType>;

template<typename EdgeType, typename ParentVertexType>
using VerticalAdjacentEdgeIndices =
    comp::AdjacentEdges<true, EdgeType, -1, false, ParentVertexType, true>;

template<typename EdgeType, typename ParentVertexType>
using OptionalAdjacentEdgeIndices = comp::
    AdjacentEdges<true, EdgeType, -1, false, ParentVertexType, true, true>;

template<typename EdgeType, typename ParentVertexType>
using AdjacentEdgePointers =
    comp::AdjacentEdges<false, EdgeType, -1, false, ParentVertexType>;

template<typename EdgeType, typename ParentVertexType>
using VerticalAdjacentEdgePointers =
    comp::AdjacentEdges<false, EdgeType, -1, false, ParentVertexType, true>;

template<typename EdgeType, typename ParentVertexType>
using OptionalAdjacentEdgePointers = comp::
    AdjacentEdges<false, EdgeType, -1, false, ParentVertexType, true, true>;

/* Port AdjacentFaces class into vert namespace */
template<bool INDEXED, typename FaceType, typename ParentVertexType>
using AdjacentFaces =
    comp::AdjacentFaces<INDEXED, FaceType, -1, false, ParentVertexType>;

template<bool INDEXED, typename FaceType, typename ParentVertexType>
using VerticalAdjacentFaces =
    comp::AdjacentFaces<INDEXED, FaceType, -1, false, ParentVertexType, true>;

template<bool INDEXED, typename FaceType, typename ParentVertexType>
using OptionalAdjacentFaces = comp::
    AdjacentFaces<INDEXED, FaceType, -1, false, ParentVertexType, true, true>;

template<typename FaceType, typename ParentVertexType>
using AdjacentFaceIndices =
    comp::AdjacentFaces<true, FaceType, -1, false, ParentVertexType>;

template<typename FaceType, typename ParentVertexType>
using VerticalAdjacentFaceIndices =
    comp::AdjacentFaces<true, FaceType, -1, false, ParentVertexType, true>;

template<typename FaceType, typename ParentVertexType>
using OptionalAdjacentFaceIndices = comp::
    AdjacentFaces<true, FaceType, -1, false, ParentVertexType, true, true>;

template<typename FaceType, typename ParentVertexType>
using AdjacentFacePointers =
    comp::AdjacentFaces<false, FaceType, -1, false, ParentVertexType>;

template<typename FaceType, typename ParentVertexType>
using VerticalAdjacentFacePointers =
    comp::AdjacentFaces<false, FaceType, -1, false, ParentVertexType, true>;

template<typename FaceType, typename ParentVertexType>
using OptionalAdjacentFacePointers = comp::
    AdjacentFaces<false, FaceType, -1, false, ParentVertexType, true, true>;

/* Port AdjacentVertices class into vert namespace */
template<bool INDEXED, typename ParentVertexType>
using AdjacentVertices =
    comp::AdjacentVertices<INDEXED, ParentVertexType, ParentVertexType>;

template<bool INDEXED, typename ParentVertexType>
using VerticalAdjacentVertices =
    comp::AdjacentVertices<INDEXED, ParentVertexType, ParentVertexType, true>;

template<bool INDEXED, typename ParentVertexType>
using OptionalAdjacentVertices = comp::
    AdjacentVertices<INDEXED, ParentVertexType, ParentVertexType, true, true>;

template<typename ParentVertexType>
using AdjacentVertexIndices =
    comp::AdjacentVertices<true, ParentVertexType, ParentVertexType>;

template<typename ParentVertexType>
using VerticalAdjacentVertexIndices =
    comp::AdjacentVertices<true, ParentVertexType, ParentVertexType, true>;

template<typename ParentVertexType>
using OptionalAdjacentVertexIndices = comp::
    AdjacentVertices<true, ParentVertexType, ParentVertexType, true, true>;

template<typename ParentVertexType>
using AdjacentVertexPointers =
    comp::AdjacentVertices<false, ParentVertexType, ParentVertexType>;

template<typename ParentVertexType>
using VerticalAdjacentVertexPointers =
    comp::AdjacentVertices<false, ParentVertexType, ParentVertexType, true>;

template<typename ParentVertexType>
using OptionalAdjacentVertexPointers = comp::
    AdjacentVertices<false, ParentVertexType, ParentVertexType, true, true>;

/* Port BitFlags class into vert namespace */
using BitFlags = comp::BitFlags<>;

template<typename ParentVertexType>
using VerticalBitFlags = comp::BitFlags<ParentVertexType>;

/* Port Color class into vert namespace */
using Color = comp::Color<>;

template<typename ParentVertexType>
using VerticalColor = comp::Color<ParentVertexType>;

template<typename ParentVertexType>
using OptionalColor = comp::Color<ParentVertexType, true>;

/* Port CustomComponents class into vert namespace */
template<typename ParentVertexType>
using CustomComponents = comp::CustomComponents<ParentVertexType>;

/* Port Mark class into vert namespace */
using Mark = comp::Mark<>;

template<typename ParentVertexType>
using VerticalMark = comp::Mark<ParentVertexType>;

template<typename ParentVertexType>
using OptionalMark = comp::Mark<ParentVertexType, true>;

/* Port MaterialIndex class into vert namespace */
using MaterialIndex = comp::MaterialIndex<>;

template<typename ParentVertexType>
using VerticalMaterialIndex = comp::MaterialIndex<ParentVertexType>;

template<typename ParentVertexType>
using OptionalMaterialIndex = comp::MaterialIndex<ParentVertexType, true>;

/* Port Normal classes into vert namespace */
template<typename ScalarType, int N>
using Normal = comp::Normal<Point<ScalarType, N>>;

template<typename ScalarType>
using Normal3 = comp::Normal3<ScalarType>;

using Normal3f = comp::Normal3f<>;
using Normal3d = comp::Normal3d<>;

template<typename ScalarType, int N, typename ParentVertexType>
using VerticalNormal = comp::Normal<Point<ScalarType, N>, ParentVertexType>;

template<typename ScalarType, typename ParentVertexType>
using VerticalNormal3 = comp::Normal3<ScalarType, ParentVertexType>;

template<typename ParentVertexType>
using VerticalNormal3f = comp::Normal3f<ParentVertexType>;

template<typename ParentVertexType>
using VerticalNormal3d = comp::Normal3d<ParentVertexType>;

template<typename ScalarType, int N, typename ParentVertexType>
using OptionalNormal =
    comp::Normal<Point<ScalarType, N>, ParentVertexType, true>;

template<typename ScalarType, typename ParentVertexType>
using OptionalNormal3 = comp::Normal3<ScalarType, ParentVertexType, true>;

template<typename ParentVertexType>
using OptionalNormal3f = comp::Normal3f<ParentVertexType, true>;

template<typename ParentVertexType>
using OptionalNormal3d = comp::Normal3d<ParentVertexType, true>;

/* Port ParentMeshPointer class into vert namespace */
template<typename MeshType>
using ParentMeshPointer = comp::ParentMeshPointer<MeshType>;

/* Port Position class into vert namespace */
template<typename ScalarType, int N>
using Position = comp::Position<Point<ScalarType, N>>;

template<typename ScalarType>
using Position3 = comp::Position3<ScalarType>;

using Position3f = comp::Position3f<>;
using Position3d = comp::Position3d<>;

template<typename ScalarType, int N, typename ParentVertexType>
using VerticalPosition = comp::Position<Point<ScalarType, N>, ParentVertexType>;

template<typename ScalarType, typename ParentVertexType>
using VerticalPosition3 = comp::Position3<ScalarType, ParentVertexType>;

template<typename ParentVertexType>
using VerticalPosition3f = comp::Position3f<ParentVertexType>;

template<typename ParentVertexType>
using VerticalPosition3d = comp::Position3d<ParentVertexType>;

/* Port PrincipalCurvature class into vert namespace */
template<typename ScalarType>
using PrincipalCurvature = comp::PrincipalCurvature<ScalarType>;

using PrincipalCurvaturef = comp::PrincipalCurvaturef<>;
using PrincipalCurvatured = comp::PrincipalCurvatured<>;

template<typename ScalarType, typename ParentVertexType>
using VerticalPrincipalCurvature =
    comp::PrincipalCurvature<ScalarType, ParentVertexType>;

template<typename ParentVertexType>
using VerticalPrincipalCurvaturef = comp::PrincipalCurvaturef<ParentVertexType>;
template<typename ParentVertexType>
using VerticalPrincipalCurvatured = comp::PrincipalCurvatured<ParentVertexType>;

template<typename ScalarType, typename ParentVertexType>
using OptionalPrincipalCurvature =
    comp::PrincipalCurvature<ScalarType, ParentVertexType, true>;

template<typename ParentVertexType>
using OptionalPrincipalCurvaturef =
    comp::PrincipalCurvaturef<ParentVertexType, true>;
template<typename ParentVertexType>
using OptionalPrincipalCurvatured =
    comp::PrincipalCurvatured<ParentVertexType, true>;

/* Port Quality class into vert namespace */
template<typename QualityType>
using Quality = comp::Quality<QualityType>;

using Qualityf = comp::Qualityf<>;
using Qualityd = comp::Qualityd<>;

template<typename QualityType, typename FaceType>
using VerticalQuality = comp::Quality<QualityType, FaceType>;

template<typename FaceType>
using VerticalQualityf = comp::Quality<float, FaceType>;

template<typename FaceType>
using VerticalQualityd = comp::Quality<double, FaceType>;

template<typename QualityType, typename ParentVertexType>
using OptionalQuality = comp::Quality<QualityType, ParentVertexType, true>;

template<typename ParentVertexType>
using OptionalQualityf = comp::Quality<float, ParentVertexType, true>;

template<typename ParentVertexType>
using OptionalQualityd = comp::Quality<double, ParentVertexType, true>;

/* Port Tangent classes into vert namespace */
template<typename ScalarType, int N>
using Tangent = comp::Tangent<Point<ScalarType, N>>;

template<typename ScalarType>
using Tangent3 = comp::Tangent3<ScalarType>;

using Tangent3f = comp::Tangent3f<>;
using Tangent3d = comp::Tangent3d<>;

template<typename ScalarType, int N, typename ParentVertexType>
using VerticalTangent = comp::Tangent<Point<ScalarType, N>, ParentVertexType>;

template<typename ScalarType, typename ParentVertexType>
using VerticalTangent3 = comp::Tangent3<ScalarType, ParentVertexType>;

template<typename ParentVertexType>
using VerticalTangent3f = comp::Tangent3f<ParentVertexType>;

template<typename ParentVertexType>
using VerticalTangent3d = comp::Tangent3d<ParentVertexType>;

template<typename ScalarType, int N, typename ParentVertexType>
using OptionalTangent =
    comp::Tangent<Point<ScalarType, N>, ParentVertexType, true>;

template<typename ScalarType, typename ParentVertexType>
using OptionalTangent3 = comp::Tangent3<ScalarType, ParentVertexType, true>;

template<typename ParentVertexType>
using OptionalTangent3f = comp::Tangent3f<ParentVertexType, true>;

template<typename ParentVertexType>
using OptionalTangent3d = comp::Tangent3d<ParentVertexType, true>;

/* Port TexCoord class into vert namespace */
template<typename ScalarType>
using TexCoord = comp::TexCoord<ScalarType>;

using TexCoordf = comp::TexCoordf<>;
using TexCoordd = comp::TexCoordd<>;

template<typename ScalarType, typename ParentVertexType>
using VerticalTexCoord = comp::TexCoord<ScalarType, ParentVertexType>;

template<typename ParentVertexType>
using VerticalTexCoordf = comp::TexCoord<float, ParentVertexType>;

template<typename ParentVertexType>
using VerticalTexCoordd = comp::TexCoord<double, ParentVertexType>;

template<typename ScalarType, typename ParentVertexType>
using OptionalTexCoord = comp::TexCoord<ScalarType, ParentVertexType, true>;

template<typename ParentVertexType>
using OptionalTexCoordf = comp::TexCoord<float, ParentVertexType, true>;

template<typename ParentVertexType>
using OptionalTexCoordd = comp::TexCoord<double, ParentVertexType, true>;

/** @} */ // end of vert_components group

} // namespace vcl::vert

#endif // VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_H
