/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
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

#ifndef VCL_MESH_ELEMENTS_FACE_COMPONENTS_H
#define VCL_MESH_ELEMENTS_FACE_COMPONENTS_H

#include "../components/adjacent_edges.h"
#include "../components/adjacent_faces.h"
#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/custom_components.h"
#include "../components/face_half_edge_reference.h"
#include "../components/mark.h"
#include "../components/normal.h"
#include "../components/parent_mesh_pointer.h"
#include "../components/polygon_bit_flags.h"
#include "../components/principal_curvature.h"
#include "../components/scalar.h"
#include "../components/triangle_bit_flags.h"
#include "../components/vertex_references.h"
#include "../components/wedge_colors.h"
#include "../components/wedge_tex_coords.h"

namespace vcl::face {

/* Port AdjacentEdges class into face namespace */
template<typename EdgeType>
using AdjacentPolygonEdges = comp::AdjacentEdges<EdgeType, -1>;

template<typename EdgeType>
using AdjacentTriangleEdges = comp::AdjacentEdges<EdgeType, 3>;

template<typename EdgeType, typename FaceType>
using VerticalAdjacentPolygonEdges = comp::AdjacentEdges<EdgeType, -1, FaceType>;

template<typename EdgeType, typename FaceType>
using VerticalAdjacentTriangleEdges = comp::AdjacentEdges<EdgeType, 3, FaceType>;

template<typename EdgeType, typename FaceType>
using OptionalAdjacentPolygonEdges = comp::AdjacentEdges<EdgeType, -1, FaceType, true>;

template<typename EdgeType, typename FaceType>
using OptionalAdjacentTriangleEdges = comp::AdjacentEdges<EdgeType, 3, FaceType, true>;

/* Port AdjacentFaces class into face namespace */
template<typename FaceType>
using AdjacentPolygons = comp::AdjacentFaces<FaceType, -1>;

template<typename FaceType>
using AdjacentTriangles = comp::AdjacentFaces<FaceType, 3>;

template<typename FaceType>
using VerticalAdjacentPolygons = comp::AdjacentFaces<FaceType, -1, FaceType>;

template<typename FaceType>
using VerticalAdjacentTriangles = comp::AdjacentFaces<FaceType, 3, FaceType>;

template<typename FaceType>
using OptionalAdjacentPolygons = comp::AdjacentFaces<FaceType, -1, FaceType, true>;

template<typename FaceType>
using OptionalAdjacentTriangles = comp::AdjacentFaces<FaceType, 3, FaceType, true>;

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

/* Port Half Edge reference into face namespace*/
template<typename HalfEdge>
using HalfEdgeReference = comp::FaceHalfEdgeReference<HalfEdge>;

/* Port Mark class into face namespace */
using Mark = comp::Mark<>;

template<typename FaceType>
using VerticalMark = comp::Mark<FaceType>;

template<typename FaceType>
using OptionalMark = comp::Mark<FaceType, true>;

/* Port Normal classes into face namespace */
template<typename ScalarType, int N>
using Normal = comp::Normal<ScalarType, N>;

template<typename ScalarType>
using Normal3 = comp::Normal3<ScalarType>;

using Normal3f = comp::Normal3f<>;
using Normal3d = comp::Normal3d<>;

template<typename ScalarType, int N, typename VertexType>
using VerticalNormal = comp::Normal<ScalarType, N, VertexType>;

template<typename ScalarType, typename VertexType>
using VerticalNormal3 = comp::Normal3<ScalarType, VertexType>;

template<typename VertexType>
using VerticalNormal3f = comp::Normal3f<VertexType>;

template<typename VertexType>
using VerticalNormal3d = comp::Normal3d<VertexType>;

template<typename ScalarType, int N, typename FaceType>
using OptionalNormal = comp::Normal<ScalarType, N, FaceType, true>;

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
using PolygonBitFlags = comp::PolygonBitFlags<>;

template<typename FaceType>
using VerticalPolygonBitFlags = comp::PolygonBitFlags<FaceType>;

/* Port PrincipalCurvature class into face namespace */
template<typename FaceType>
using PrincipalCurvature = comp::PrincipalCurvature<FaceType>;

using PrincipalCurvaturef = comp::PrincipalCurvaturef<>;
using PrincipalCurvatured = comp::PrincipalCurvatured<>;

template<typename ScalarType, typename FaceType>
using VerticalPrincipalCurvature = comp::PrincipalCurvature<ScalarType, FaceType>;

template<typename FaceType>
using VerticalPrincipalCurvaturef = comp::PrincipalCurvaturef<FaceType>;
template<typename FaceType>
using VerticalPrincipalCurvatured = comp::PrincipalCurvatured<FaceType>;

template<typename ScalarType, typename FaceType>
using OptionalPrincipalCurvature = comp::PrincipalCurvature<ScalarType, FaceType, true>;

template<typename FaceType>
using OptionalPrincipalCurvaturef = comp::PrincipalCurvaturef<FaceType, true>;
template<typename FaceType>
using OptionalPrincipalCurvatured = comp::PrincipalCurvatured<FaceType, true>;

/* Port Scalar class into face namespace */
template<typename ScalarType>
using Scalar = comp::Scalar<ScalarType>;

using Scalarf = comp::Scalarf<>;
using Scalard = comp::Scalard<>;

template<typename ScalarType, typename FaceType>
using VerticalScalar = comp::Scalar<ScalarType, FaceType>;

template<typename FaceType>
using VerticalScalarf = comp::Scalar<float, FaceType>;

template<typename FaceType>
using VerticalScalard = comp::Scalar<double, FaceType>;

template<typename ScalarType, typename FaceType>
using OptionalScalar = comp::Scalar<ScalarType, FaceType, true>;

template<typename FaceType>
using OptionalScalarf = comp::Scalar<float, FaceType, true>;

template<typename FaceType>
using OptionalScalard = comp::Scalar<double, FaceType, true>;

/* Port TriangleBitFlags class into face namespace */
using TriangleBitFlags = comp::TriangleBitFlags<>;

template<typename FaceType>
using VerticalTriangleBitFlags = comp::TriangleBitFlags<FaceType>;

/* Port VertexReferences class into face namespace */
template<typename Vertex, int N>
using VertexReferences = comp::VertexReferences<Vertex, N>;

template<typename Vertex>
using TriangleVertexRefs = comp::VertexReferences<Vertex, 3>;

template<typename Vertex>
using PolygonVertexRefs = comp::VertexReferences<Vertex, -1>;

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

using PolygonWedgeTexCoordsf = PolygonWedgeTexCoords<float>;
using PolygonWedgeTexCoordsd = PolygonWedgeTexCoords<double>;
using TriangleWedgeTexCoordsf = TriangleWedgeTexCoords<float>;
using TriangleWedgeTexCoordsd = TriangleWedgeTexCoords<double>;

template<typename ScalarType, typename FaceType>
using VerticalPolygonWedgeTexCoords = comp::WedgeTexCoords<ScalarType, -1, FaceType, true>;

template<typename ScalarType, typename FaceType>
using VerticalTriangleWedgeTexCoords = comp::WedgeTexCoords<ScalarType, 3, FaceType, true>;

template<typename FaceType>
using VerticalPolygonWedgeTexCoordsf = comp::WedgeTexCoords<float, -1, FaceType>;
template<typename FaceType>
using VerticalPolygonWedgeTexCoordsd = comp::WedgeTexCoords<double, -1, FaceType>;
template<typename FaceType>
using VerticalTriangleWedgeTexCoordsf = comp::WedgeTexCoords<float, 3, FaceType>;
template<typename FaceType>
using VerticalTriangleWedgeTexCoordsd = comp::WedgeTexCoords<double, 3, FaceType>;

template<typename ScalarType, typename FaceType>
using OptionalPolygonWedgeTexCoords = comp::WedgeTexCoords<ScalarType, -1, FaceType, true>;

template<typename ScalarType, typename FaceType>
using OptionalTriangleWedgeTexCoords = comp::WedgeTexCoords<ScalarType, 3, FaceType, true>;

template<typename FaceType>
using OptionalPolygonWedgeTexCoordsf = comp::WedgeTexCoords<float, -1, FaceType, true>;
template<typename FaceType>
using OptionalPolygonWedgeTexCoordsd = comp::WedgeTexCoords<double, -1, FaceType, true>;
template<typename FaceType>
using OptionalTriangleWedgeTexCoordsf = comp::WedgeTexCoords<float, 3, FaceType, true>;
template<typename FaceType>
using OptionalTriangleWedgeTexCoordsd = comp::WedgeTexCoords<double, 3, FaceType, true>;

} // namespace vcl::face

#endif // VCL_MESH_ELEMENTS_FACE_COMPONENTS_H
