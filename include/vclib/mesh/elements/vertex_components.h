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

#ifndef VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_H
#define VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_H

#include "../components/adjacent_edges.h"
#include "../components/adjacent_faces.h"
#include "../components/adjacent_vertices.h"
#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/coordinate.h"
#include "../components/custom_components.h"
#include "../components/mark.h"
#include "../components/normal.h"
#include "../components/parent_mesh_pointer.h"
#include "../components/principal_curvature.h"
#include "../components/quality.h"
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

namespace vcl::vert {

/**
 * @addtogroup vert_components
 * @{
 */

/* Port AdjacentEdges class into vert namespace */
template<typename EdgeType, typename ParentVertexType>
using AdjacentEdges =
    comp::AdjacentEdges<EdgeType, -1, false, ParentVertexType>;

template<typename EdgeType, typename ParentVertexType>
using VerticalAdjacentEdges =
    comp::AdjacentEdges<EdgeType, -1, false, ParentVertexType, true>;

template<typename EdgeType, typename ParentVertexType>
using OptionalAdjacentEdges =
    comp::AdjacentEdges<EdgeType, -1, false, ParentVertexType, true, true>;

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
template<typename ParentVertexType>
using AdjacentVertices =
    comp::AdjacentVertices<ParentVertexType, ParentVertexType>;

template<typename ParentVertexType>
using VerticalAdjacentVertices =
    comp::AdjacentVertices<ParentVertexType, ParentVertexType, true>;

template<typename ParentVertexType>
using OptionalAdjacentVertices =
    comp::AdjacentVertices<ParentVertexType, ParentVertexType, true, true>;

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

/* Port Coordinate class into vert namespace */
template<typename ScalarType, int N>
using Coordinate = comp::Coordinate<Point<ScalarType, N>>;

template<typename ScalarType>
using Coordinate3 = comp::Coordinate3<ScalarType>;

using Coordinate3f = comp::Coordinate3f<>;
using Coordinate3d = comp::Coordinate3d<>;

template<typename ScalarType, int N, typename ParentVertexType>
using VerticalCoordinate =
    comp::Coordinate<Point<ScalarType, N>, ParentVertexType>;

template<typename ScalarType, typename ParentVertexType>
using VerticalCoordinate3 = comp::Coordinate3<ScalarType, ParentVertexType>;

template<typename ParentVertexType>
using VerticalCoordinate3f = comp::Coordinate3f<ParentVertexType>;

template<typename ParentVertexType>
using VerticalCoordinate3d = comp::Coordinate3d<ParentVertexType>;

/* Port CustomComponents class into vert namespace */
template<typename ParentVertexType>
using CustomComponents = comp::CustomComponents<ParentVertexType>;

/* Port Mark class into vert namespace */
using Mark = comp::Mark<>;

template<typename ParentVertexType>
using VerticalMark = comp::Mark<ParentVertexType>;

template<typename ParentVertexType>
using OptionalMark = comp::Mark<ParentVertexType, true>;

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

/** @} */ // end of edge_components group

} // namespace vcl::vert

#endif // VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_H
