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

#include "../components/adjacent_edge_pointers.h"
#include "../components/adjacent_face_pointers.h"
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
template<typename EdgeType, typename VertexType>
using AdjacentEdgePointers =
    comp::AdjacentEdgePointers<EdgeType, -1, false, VertexType>;

template<typename EdgeType, typename VertexType>
using VerticalAdjacentEdgePointers =
    comp::AdjacentEdgePointers<EdgeType, -1, false, VertexType, true>;

template<typename EdgeType, typename VertexType>
using OptionalAdjacentEdgePointers =
    comp::AdjacentEdgePointers<EdgeType, -1, false, VertexType, true, true>;

/* Port AdjacentFaces class into vert namespace */
template<typename FaceType, typename VertexType>
using AdjacentFacePointers =
    comp::AdjacentFacePointers<FaceType, -1, false, VertexType>;

template<typename FaceType, typename VertexType>
using VerticalAdjacentFacePointers =
    comp::AdjacentFacePointers<FaceType, -1, false, VertexType, true>;

template<typename FaceType, typename VertexType>
using OptionalAdjacentFacePointers =
    comp::AdjacentFacePointers<FaceType, -1, false, VertexType, true, true>;

/* Port AdjacentVertices class into vert namespace */
template<typename VertexType>
using AdjacentVertices = comp::AdjacentVertices<VertexType, VertexType>;

template<typename VertexType>
using VerticalAdjacentVertices =
    comp::AdjacentVertices<VertexType, VertexType, true>;

template<typename VertexType>
using OptionalAdjacentVertices =
    comp::AdjacentVertices<VertexType, VertexType, true, true>;

/* Port BitFlags class into vert namespace */
using BitFlags = comp::BitFlags<>;

template<typename VertexType>
using VerticalBitFlags = comp::BitFlags<VertexType>;

/* Port Color class into vert namespace */
using Color = comp::Color<>;

template<typename VertexType>
using VerticalColor = comp::Color<VertexType>;

template<typename VertexType>
using OptionalColor = comp::Color<VertexType, true>;

/* Port Coordinate class into vert namespace */
template<typename ScalarType, int N>
using Coordinate = comp::Coordinate<Point<ScalarType, N>>;

template<typename ScalarType>
using Coordinate3 = comp::Coordinate3<ScalarType>;

using Coordinate3f = comp::Coordinate3f<>;
using Coordinate3d = comp::Coordinate3d<>;

template<typename ScalarType, int N, typename VertexType>
using VerticalCoordinate = comp::Coordinate<Point<ScalarType, N>, VertexType>;

template<typename ScalarType, typename VertexType>
using VerticalCoordinate3 = comp::Coordinate3<ScalarType, VertexType>;

template<typename VertexType>
using VerticalCoordinate3f = comp::Coordinate3f<VertexType>;

template<typename VertexType>
using VerticalCoordinate3d = comp::Coordinate3d<VertexType>;

/* Port CustomComponents class into vert namespace */
template<typename VertexType>
using CustomComponents = comp::CustomComponents<VertexType>;

/* Port Mark class into vert namespace */
using Mark = comp::Mark<>;

template<typename VertexType>
using VerticalMark = comp::Mark<VertexType>;

template<typename VertexType>
using OptionalMark = comp::Mark<VertexType, true>;

/* Port Normal classes into vert namespace */
template<typename ScalarType, int N>
using Normal = comp::Normal<Point<ScalarType, N>>;

template<typename ScalarType>
using Normal3 = comp::Normal3<ScalarType>;

using Normal3f = comp::Normal3f<>;
using Normal3d = comp::Normal3d<>;

template<typename ScalarType, int N, typename VertexType>
using VerticalNormal = comp::Normal<Point<ScalarType, N>, VertexType>;

template<typename ScalarType, typename VertexType>
using VerticalNormal3 = comp::Normal3<ScalarType, VertexType>;

template<typename VertexType>
using VerticalNormal3f = comp::Normal3f<VertexType>;

template<typename VertexType>
using VerticalNormal3d = comp::Normal3d<VertexType>;

template<typename ScalarType, int N, typename VertexType>
using OptionalNormal = comp::Normal<Point<ScalarType, N>, VertexType, true>;

template<typename ScalarType, typename VertexType>
using OptionalNormal3 = comp::Normal3<ScalarType, VertexType, true>;

template<typename VertexType>
using OptionalNormal3f = comp::Normal3f<VertexType, true>;

template<typename VertexType>
using OptionalNormal3d = comp::Normal3d<VertexType, true>;

/* Port ParentMeshPointer class into vert namespace */
template<typename MeshType>
using ParentMeshPointer = comp::ParentMeshPointer<MeshType>;

/* Port PrincipalCurvature class into vert namespace */
template<typename ScalarType>
using PrincipalCurvature = comp::PrincipalCurvature<ScalarType>;

using PrincipalCurvaturef = comp::PrincipalCurvaturef<>;
using PrincipalCurvatured = comp::PrincipalCurvatured<>;

template<typename ScalarType, typename VertexType>
using VerticalPrincipalCurvature =
    comp::PrincipalCurvature<ScalarType, VertexType>;

template<typename VertexType>
using VerticalPrincipalCurvaturef = comp::PrincipalCurvaturef<VertexType>;
template<typename VertexType>
using VerticalPrincipalCurvatured = comp::PrincipalCurvatured<VertexType>;

template<typename ScalarType, typename VertexType>
using OptionalPrincipalCurvature =
    comp::PrincipalCurvature<ScalarType, VertexType, true>;

template<typename VertexType>
using OptionalPrincipalCurvaturef = comp::PrincipalCurvaturef<VertexType, true>;
template<typename VertexType>
using OptionalPrincipalCurvatured = comp::PrincipalCurvatured<VertexType, true>;

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

template<typename QualityType, typename VertexType>
using OptionalQuality = comp::Quality<QualityType, VertexType, true>;

template<typename VertexType>
using OptionalQualityf = comp::Quality<float, VertexType, true>;

template<typename VertexType>
using OptionalQualityd = comp::Quality<double, VertexType, true>;

/* Port TexCoord class into vert namespace */
template<typename ScalarType>
using TexCoord = comp::TexCoord<ScalarType>;

using TexCoordf = comp::TexCoordf<>;
using TexCoordd = comp::TexCoordd<>;

template<typename ScalarType, typename VertexType>
using VerticalTexCoord = comp::TexCoord<ScalarType, VertexType>;

template<typename VertexType>
using VerticalTexCoordf = comp::TexCoord<float, VertexType>;

template<typename VertexType>
using VerticalTexCoordd = comp::TexCoord<double, VertexType>;

template<typename ScalarType, typename VertexType>
using OptionalTexCoord = comp::TexCoord<ScalarType, VertexType, true>;

template<typename VertexType>
using OptionalTexCoordf = comp::TexCoord<float, VertexType, true>;

template<typename VertexType>
using OptionalTexCoordd = comp::TexCoord<double, VertexType, true>;

/** @} */ // end of edge_components group

} // namespace vcl::vert

#endif // VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_H
