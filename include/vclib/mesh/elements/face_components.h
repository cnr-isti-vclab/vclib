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

#include "../components/adjacent_edges.h"
#include "../components/adjacent_faces.h"
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
#include "../components/vertex_references.h"
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
using AdjacentEdges = comp::AdjacentEdges<EdgeType, -1, false>;

template<typename EdgeType, typename ParentFaceType>
using AdjacentPolygonEdges =
    comp::AdjacentEdges<EdgeType, -1, true, ParentFaceType>;

template<typename EdgeType, typename ParentFaceType>
using AdjacentTriangleEdges =
    comp::AdjacentEdges<EdgeType, 3, true, ParentFaceType>;

template<typename EdgeType, typename ParentFaceType>
using VerticalAdjacentEdges =
    comp::AdjacentEdges<EdgeType, -1, false, ParentFaceType, true>;

template<typename EdgeType, typename ParentFaceType>
using VerticalAdjacentPolygonEdges =
    comp::AdjacentEdges<EdgeType, -1, true, ParentFaceType, true>;

template<typename EdgeType, typename ParentFaceType>
using VerticalAdjacentTriangleEdges =
    comp::AdjacentEdges<EdgeType, 3, true, ParentFaceType, true>;

template<typename EdgeType, typename ParentFaceType>
using OptionalAdjacentEdges =
    comp::AdjacentEdges<EdgeType, -1, false, ParentFaceType, true, true>;

template<typename EdgeType, typename ParentFaceType>
using OptionalAdjacentPolygonEdges =
    comp::AdjacentEdges<EdgeType, -1, true, ParentFaceType, true, true>;

template<typename EdgeType, typename ParentFaceType>
using OptionalAdjacentTriangleEdges =
    comp::AdjacentEdges<EdgeType, 3, true, ParentFaceType, true, true>;

/* Port AdjacentFaces class into face namespace */
template<typename ParentFaceType>
using AdjacentPolygons =
    comp::AdjacentFaces<ParentFaceType, -1, true, ParentFaceType>;

template<typename ParentFaceType>
using AdjacentTriangles =
    comp::AdjacentFaces<ParentFaceType, 3, true, ParentFaceType>;

template<typename ParentFaceType>
using VerticalAdjacentPolygons =
    comp::AdjacentFaces<ParentFaceType, -1, true, ParentFaceType, true>;

template<typename ParentFaceType>
using VerticalAdjacentTriangles =
    comp::AdjacentFaces<ParentFaceType, 3, true, ParentFaceType, true>;

template<typename ParentFaceType>
using OptionalAdjacentPolygons = comp::
    AdjacentFaces<ParentFaceType, -1, true, ParentFaceType, true, true>;

template<typename ParentFaceType>
using OptionalAdjacentTriangles = comp::
    AdjacentFaces<ParentFaceType, 3, true, ParentFaceType, true, true>;

/* Port BitFlags class into face namespace */
using BitFlags = comp::BitFlags<>;

template<typename ParentFaceType>
using VerticalBitFlags = comp::BitFlags<ParentFaceType>;

/* Port Color class into face namespace */
using Color = comp::Color<>;

template<typename VertexType>
using VerticalColor = comp::Color<VertexType>;

template<typename ParentFaceType>
using OptionalColor = comp::Color<ParentFaceType, true>;

/* Port CustomComponents class into face namespace */
template<typename ParentFaceType>
using CustomComponents = comp::CustomComponents<ParentFaceType>;

/* Port Mark class into face namespace */
using Mark = comp::Mark<>;

template<typename ParentFaceType>
using VerticalMark = comp::Mark<ParentFaceType>;

template<typename ParentFaceType>
using OptionalMark = comp::Mark<ParentFaceType, true>;

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

template<typename ScalarType, int N, typename ParentFaceType>
using OptionalNormal = comp::Normal<Point<ScalarType, N>, ParentFaceType, true>;

template<typename ScalarType, typename ParentFaceType>
using OptionalNormal3 = comp::Normal3<ScalarType, ParentFaceType, true>;

template<typename ParentFaceType>
using OptionalNormal3f = comp::Normal3f<ParentFaceType, true>;

template<typename ParentFaceType>
using OptionalNormal3d = comp::Normal3d<ParentFaceType, true>;

/* Port ParentMeshPointer class into vert namespace */
template<typename MeshType>
using ParentMeshPointer = comp::ParentMeshPointer<MeshType>;

/* Port PolygonBitFlags class into face namespace */
using PolygonBitFlags = comp::PolygonBitFlags<-1>;

template<typename ParentFaceType>
using VerticalPolygonBitFlags = comp::PolygonBitFlags<-1, ParentFaceType>;

/* Port PrincipalCurvature class into face namespace */
template<typename ParentFaceType>
using PrincipalCurvature = comp::PrincipalCurvature<ParentFaceType>;

using PrincipalCurvaturef = comp::PrincipalCurvaturef<>;
using PrincipalCurvatured = comp::PrincipalCurvatured<>;

template<typename ScalarType, typename ParentFaceType>
using VerticalPrincipalCurvature =
    comp::PrincipalCurvature<ScalarType, ParentFaceType>;

template<typename ParentFaceType>
using VerticalPrincipalCurvaturef = comp::PrincipalCurvaturef<ParentFaceType>;
template<typename ParentFaceType>
using VerticalPrincipalCurvatured = comp::PrincipalCurvatured<ParentFaceType>;

template<typename ScalarType, typename ParentFaceType>
using OptionalPrincipalCurvature =
    comp::PrincipalCurvature<ScalarType, ParentFaceType, true>;

template<typename ParentFaceType>
using OptionalPrincipalCurvaturef =
    comp::PrincipalCurvaturef<ParentFaceType, true>;
template<typename ParentFaceType>
using OptionalPrincipalCurvatured =
    comp::PrincipalCurvatured<ParentFaceType, true>;

/* Port Quality class into face namespace */
template<typename QualityType>
using Quality = comp::Quality<QualityType>;

using Qualityf = comp::Qualityf<>;
using Qualityd = comp::Qualityd<>;

template<typename QualityType, typename ParentFaceType>
using VerticalQuality = comp::Quality<QualityType, ParentFaceType>;

template<typename ParentFaceType>
using VerticalQualityf = comp::Quality<float, ParentFaceType>;

template<typename ParentFaceType>
using VerticalQualityd = comp::Quality<double, ParentFaceType>;

template<typename QualityType, typename ParentFaceType>
using OptionalQuality = comp::Quality<QualityType, ParentFaceType, true>;

template<typename ParentFaceType>
using OptionalQualityf = comp::Quality<float, ParentFaceType, true>;

template<typename ParentFaceType>
using OptionalQualityd = comp::Quality<double, ParentFaceType, true>;

/* Port TriangleBitFlags class into face namespace */
using TriangleBitFlags = comp::TriangleBitFlags<>;

template<typename ParentFaceType>
using VerticalTriangleBitFlags = comp::TriangleBitFlags<ParentFaceType>;

/* Port VertexReferences alias into face namespace */
template<bool INDEXED, typename Vertex, int N, typename FaceType>
using VertexReferences = comp::VertexReferences<INDEXED, Vertex, N, FaceType>;

template<bool INDEXED, typename Vertex, typename FaceType>
using TriangleVertexRefs = comp::VertexReferences<INDEXED, Vertex, 3, FaceType>;

template<bool INDEXED, typename Vertex, typename FaceType>
using PolygonVertexRefs = comp::VertexReferences<INDEXED, Vertex, -1, FaceType>;

template<typename Vertex, int N, typename FaceType>
using VertexIndices = comp::VertexReferences<true, Vertex, N, FaceType>;

template<typename Vertex, typename FaceType>
using TriangleVertexInds = comp::VertexReferences<true, Vertex, 3, FaceType>;

template<typename Vertex, typename FaceType>
using PolygonVertexInds = comp::VertexReferences<true, Vertex, -1, FaceType>;

template<typename Vertex, int N, typename FaceType>
using VertexPointers = comp::VertexReferences<false, Vertex, N, FaceType>;

template<typename Vertex, typename FaceType>
using TriangleVertexPtrs = comp::VertexReferences<false, Vertex, 3, FaceType>;

template<typename Vertex, typename FaceType>
using PolygonVertexPtrs = comp::VertexReferences<false, Vertex, -1, FaceType>;

/* Port WedgeColors class into face namespace */
using PolygonWedgeColors = comp::WedgeColors<-1>;

using TriangleWedgeColors = comp::WedgeColors<3>;

template<typename ParentFaceType>
using VerticalPolygonWedgeColors = comp::WedgeColors<-1, ParentFaceType>;

template<typename ParentFaceType>
using VerticalTriangleWedgeColors = comp::WedgeColors<3, ParentFaceType>;

template<typename ParentFaceType>
using OptionalPolygonWedgeColors = comp::WedgeColors<-1, ParentFaceType, true>;

template<typename ParentFaceType>
using OptionalTriangleWedgeColors = comp::WedgeColors<3, ParentFaceType, true>;

/* Port WedgeTexCoords class into face namespace */
template<typename ScalarType>
using PolygonWedgeTexCoords = comp::WedgeTexCoords<ScalarType, -1>;

template<typename ScalarType>
using TriangleWedgeTexCoords = comp::WedgeTexCoords<ScalarType, 3>;

using PolygonWedgeTexCoordsf  = PolygonWedgeTexCoords<float>;
using PolygonWedgeTexCoordsd  = PolygonWedgeTexCoords<double>;
using TriangleWedgeTexCoordsf = TriangleWedgeTexCoords<float>;
using TriangleWedgeTexCoordsd = TriangleWedgeTexCoords<double>;

template<typename ScalarType, typename ParentFaceType>
using VerticalPolygonWedgeTexCoords =
    comp::WedgeTexCoords<ScalarType, -1, ParentFaceType, true>;

template<typename ScalarType, typename ParentFaceType>
using VerticalTriangleWedgeTexCoords =
    comp::WedgeTexCoords<ScalarType, 3, ParentFaceType, true>;

template<typename ParentFaceType>
using VerticalPolygonWedgeTexCoordsf =
    comp::WedgeTexCoords<float, -1, ParentFaceType>;
template<typename ParentFaceType>
using VerticalPolygonWedgeTexCoordsd =
    comp::WedgeTexCoords<double, -1, ParentFaceType>;
template<typename ParentFaceType>
using VerticalTriangleWedgeTexCoordsf =
    comp::WedgeTexCoords<float, 3, ParentFaceType>;
template<typename ParentFaceType>
using VerticalTriangleWedgeTexCoordsd =
    comp::WedgeTexCoords<double, 3, ParentFaceType>;

template<typename ScalarType, typename ParentFaceType>
using OptionalPolygonWedgeTexCoords =
    comp::WedgeTexCoords<ScalarType, -1, ParentFaceType, true>;

template<typename ScalarType, typename ParentFaceType>
using OptionalTriangleWedgeTexCoords =
    comp::WedgeTexCoords<ScalarType, 3, ParentFaceType, true>;

template<typename ParentFaceType>
using OptionalPolygonWedgeTexCoordsf =
    comp::WedgeTexCoords<float, -1, ParentFaceType, true>;
template<typename ParentFaceType>
using OptionalPolygonWedgeTexCoordsd =
    comp::WedgeTexCoords<double, -1, ParentFaceType, true>;
template<typename ParentFaceType>
using OptionalTriangleWedgeTexCoordsf =
    comp::WedgeTexCoords<float, 3, ParentFaceType, true>;
template<typename ParentFaceType>
using OptionalTriangleWedgeTexCoordsd =
    comp::WedgeTexCoords<double, 3, ParentFaceType, true>;

/** @} */ // end of edge_components group

} // namespace vcl::face

#endif // VCL_MESH_ELEMENTS_FACE_COMPONENTS_H
