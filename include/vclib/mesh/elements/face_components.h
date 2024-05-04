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

#ifndef VCL_MESH_ELEMENTS_FACE_COMPONENTS_H
#define VCL_MESH_ELEMENTS_FACE_COMPONENTS_H

#include "../components/adjacent_edge_pointers.h"
#include "../components/adjacent_face_pointers.h"
#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/custom_components.h"
#include "../components/mark.h"
#include "../components/normal.h"
#include "../components/parent_mesh_pointer.h"
#include "../components/polygon_bit_flags.h"
#include "../components/principal_curvature.h"
#include "../components/quality.h"
#include "../components/triangle_bit_flags.h"
#include "../components/vertex_indices.h"
#include "../components/vertex_references.h"
#include "../components/vertex_pointers.h"
#include "../components/wedge_colors.h"
#include "../components/wedge_tex_coords.h"

/**
 * @defgroup face_components Face Components
 * @ingroup components
 *
 * @brief This group lists all the Components that may be used by the Face
 * element.
 *
 * All these components are listed inside the vcl::face namespace.
 */

namespace vcl::face {

/**
 * @addtogroup face_components
 * @{
 */

/* Port AdjacentEdges class into face namespace */
template<typename EdgeType>
using AdjacentEdgePointers = comp::AdjacentEdgePointers<EdgeType, -1, false>;

template<typename EdgeType, typename FaceType>
using AdjacentPolygonEdgePointers =
    comp::AdjacentEdgePointers<EdgeType, -1, true, FaceType>;

template<typename EdgeType, typename FaceType>
using AdjacentTriangleEdgePointers =
    comp::AdjacentEdgePointers<EdgeType, 3, true, FaceType>;

template<typename EdgeType, typename FaceType>
using VerticalAdjacentEdgePointers =
    comp::AdjacentEdgePointers<EdgeType, -1, false, FaceType, true>;

template<typename EdgeType, typename FaceType>
using VerticalAdjacentPolygonEdgePointers =
    comp::AdjacentEdgePointers<EdgeType, -1, true, FaceType, true>;

template<typename EdgeType, typename FaceType>
using VerticalAdjacentTriangleEdgePointers =
    comp::AdjacentEdgePointers<EdgeType, 3, true, FaceType, true>;

template<typename EdgeType, typename FaceType>
using OptionalAdjacentEdgePointers =
    comp::AdjacentEdgePointers<EdgeType, -1, false, FaceType, true, true>;

template<typename EdgeType, typename FaceType>
using OptionalAdjacentPolygonEdgePointers =
    comp::AdjacentEdgePointers<EdgeType, -1, true, FaceType, true, true>;

template<typename EdgeType, typename FaceType>
using OptionalAdjacentTriangleEdgePointers =
    comp::AdjacentEdgePointers<EdgeType, 3, true, FaceType, true, true>;

/* Port AdjacentFaces class into face namespace */
template<typename FaceType>
using AdjacentPolygonPointers =
    comp::AdjacentFacePointers<FaceType, -1, true, FaceType>;

template<typename FaceType>
using AdjacentTrianglePointers =
    comp::AdjacentFacePointers<FaceType, 3, true, FaceType>;

template<typename FaceType>
using VerticalAdjacentPolygonPointers =
    comp::AdjacentFacePointers<FaceType, -1, true, FaceType, true>;

template<typename FaceType>
using VerticalAdjacentTrianglePointers =
    comp::AdjacentFacePointers<FaceType, 3, true, FaceType, true>;

template<typename FaceType>
using OptionalAdjacentPolygonPointers =
    comp::AdjacentFacePointers<FaceType, -1, true, FaceType, true, true>;

template<typename FaceType>
using OptionalAdjacentTrianglePointers =
    comp::AdjacentFacePointers<FaceType, 3, true, FaceType, true, true>;

/* Port BitFlags class into face namespace */
using BitFlags = comp::BitFlags<>;

template<typename FaceType>
using VerticalBitFlags = comp::BitFlags<FaceType>;

/* Port Color class into face namespace */
using Color = comp::Color<>;

template<typename VertexType>
using VerticalColor = comp::Color<VertexType>;

template<typename FaceType>
using OptionalColor = comp::Color<FaceType, true>;

/* Port CustomComponents class into face namespace */
template<typename FaceType>
using CustomComponents = comp::CustomComponents<FaceType>;

/* Port Mark class into face namespace */
using Mark = comp::Mark<>;

template<typename FaceType>
using VerticalMark = comp::Mark<FaceType>;

template<typename FaceType>
using OptionalMark = comp::Mark<FaceType, true>;

/* Port Normal classes into face namespace */
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

template<typename ScalarType, int N, typename FaceType>
using OptionalNormal = comp::Normal<Point<ScalarType, N>, FaceType, true>;

template<typename ScalarType, typename FaceType>
using OptionalNormal3 = comp::Normal3<ScalarType, FaceType, true>;

template<typename FaceType>
using OptionalNormal3f = comp::Normal3f<FaceType, true>;

template<typename FaceType>
using OptionalNormal3d = comp::Normal3d<FaceType, true>;

/* Port ParentMeshPointer class into vert namespace */
template<typename MeshType>
using ParentMeshPointer = comp::ParentMeshPointer<MeshType>;

/* Port PolygonBitFlags class into face namespace */
using PolygonBitFlags = comp::PolygonBitFlags<-1>;

template<typename FaceType>
using VerticalPolygonBitFlags = comp::PolygonBitFlags<-1, FaceType>;

/* Port PrincipalCurvature class into face namespace */
template<typename FaceType>
using PrincipalCurvature = comp::PrincipalCurvature<FaceType>;

using PrincipalCurvaturef = comp::PrincipalCurvaturef<>;
using PrincipalCurvatured = comp::PrincipalCurvatured<>;

template<typename ScalarType, typename FaceType>
using VerticalPrincipalCurvature =
    comp::PrincipalCurvature<ScalarType, FaceType>;

template<typename FaceType>
using VerticalPrincipalCurvaturef = comp::PrincipalCurvaturef<FaceType>;
template<typename FaceType>
using VerticalPrincipalCurvatured = comp::PrincipalCurvatured<FaceType>;

template<typename ScalarType, typename FaceType>
using OptionalPrincipalCurvature =
    comp::PrincipalCurvature<ScalarType, FaceType, true>;

template<typename FaceType>
using OptionalPrincipalCurvaturef = comp::PrincipalCurvaturef<FaceType, true>;
template<typename FaceType>
using OptionalPrincipalCurvatured = comp::PrincipalCurvatured<FaceType, true>;

/* Port Quality class into face namespace */
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

template<typename QualityType, typename FaceType>
using OptionalQuality = comp::Quality<QualityType, FaceType, true>;

template<typename FaceType>
using OptionalQualityf = comp::Quality<float, FaceType, true>;

template<typename FaceType>
using OptionalQualityd = comp::Quality<double, FaceType, true>;

/* Port TriangleBitFlags class into face namespace */
using TriangleBitFlags = comp::TriangleBitFlags<>;

template<typename FaceType>
using VerticalTriangleBitFlags = comp::TriangleBitFlags<FaceType>;

/* Port VertexIndices class into face namespace */
template<typename Vertex, int N, typename FaceType>
using VertexIndices = comp::VertexIndices<Vertex, N, FaceType>;

template<typename Vertex, typename FaceType>
using TriangleVertexInds = comp::VertexIndices<Vertex, 3, FaceType>;

template<typename Vertex, typename FaceType>
using PolygonVertexInds = comp::VertexIndices<Vertex, -1, FaceType>;

/* Port VertexReferences alias into face namespace */
template<bool INDEXED, typename Vertex, int N, typename FaceType>
using VertexReferences = comp::VertexReferences<INDEXED, Vertex, N, FaceType>;

template<bool INDEXED, typename Vertex, typename FaceType>
using TriangleVertexRefs = comp::VertexReferences<INDEXED, Vertex, 3, FaceType>;

template<bool INDEXED, typename Vertex, typename FaceType>
using PolygonVertexRefs = comp::VertexReferences<INDEXED, Vertex, -1, FaceType>;

/* Port VertexPointers class into face namespace */
template<typename Vertex, int N, typename FaceType>
using VertexPointers = comp::VertexPointers<Vertex, N, FaceType>;

template<typename Vertex, typename FaceType>
using TriangleVertexPtrs = comp::VertexPointers<Vertex, 3, FaceType>;

template<typename Vertex, typename FaceType>
using PolygonVertexPtrs = comp::VertexPointers<Vertex, -1, FaceType>;

/* Port WedgeColors class into face namespace */
using PolygonWedgeColors = comp::WedgeColors<-1>;

using TriangleWedgeColors = comp::WedgeColors<3>;

template<typename FaceType>
using VerticalPolygonWedgeColors = comp::WedgeColors<-1, FaceType>;

template<typename FaceType>
using VerticalTriangleWedgeColors = comp::WedgeColors<3, FaceType>;

template<typename FaceType>
using OptionalPolygonWedgeColors = comp::WedgeColors<-1, FaceType, true>;

template<typename FaceType>
using OptionalTriangleWedgeColors = comp::WedgeColors<3, FaceType, true>;

/* Port WedgeTexCoords class into face namespace */
template<typename ScalarType>
using PolygonWedgeTexCoords = comp::WedgeTexCoords<ScalarType, -1>;

template<typename ScalarType>
using TriangleWedgeTexCoords = comp::WedgeTexCoords<ScalarType, 3>;

using PolygonWedgeTexCoordsf  = PolygonWedgeTexCoords<float>;
using PolygonWedgeTexCoordsd  = PolygonWedgeTexCoords<double>;
using TriangleWedgeTexCoordsf = TriangleWedgeTexCoords<float>;
using TriangleWedgeTexCoordsd = TriangleWedgeTexCoords<double>;

template<typename ScalarType, typename FaceType>
using VerticalPolygonWedgeTexCoords =
    comp::WedgeTexCoords<ScalarType, -1, FaceType, true>;

template<typename ScalarType, typename FaceType>
using VerticalTriangleWedgeTexCoords =
    comp::WedgeTexCoords<ScalarType, 3, FaceType, true>;

template<typename FaceType>
using VerticalPolygonWedgeTexCoordsf =
    comp::WedgeTexCoords<float, -1, FaceType>;
template<typename FaceType>
using VerticalPolygonWedgeTexCoordsd =
    comp::WedgeTexCoords<double, -1, FaceType>;
template<typename FaceType>
using VerticalTriangleWedgeTexCoordsf =
    comp::WedgeTexCoords<float, 3, FaceType>;
template<typename FaceType>
using VerticalTriangleWedgeTexCoordsd =
    comp::WedgeTexCoords<double, 3, FaceType>;

template<typename ScalarType, typename FaceType>
using OptionalPolygonWedgeTexCoords =
    comp::WedgeTexCoords<ScalarType, -1, FaceType, true>;

template<typename ScalarType, typename FaceType>
using OptionalTriangleWedgeTexCoords =
    comp::WedgeTexCoords<ScalarType, 3, FaceType, true>;

template<typename FaceType>
using OptionalPolygonWedgeTexCoordsf =
    comp::WedgeTexCoords<float, -1, FaceType, true>;
template<typename FaceType>
using OptionalPolygonWedgeTexCoordsd =
    comp::WedgeTexCoords<double, -1, FaceType, true>;
template<typename FaceType>
using OptionalTriangleWedgeTexCoordsf =
    comp::WedgeTexCoords<float, 3, FaceType, true>;
template<typename FaceType>
using OptionalTriangleWedgeTexCoordsd =
    comp::WedgeTexCoords<double, 3, FaceType, true>;

/** @} */ // end of edge_components group

} // namespace vcl::face

#endif // VCL_MESH_ELEMENTS_FACE_COMPONENTS_H
