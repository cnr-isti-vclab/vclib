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
#include "../components/scalar.h"
#include "../components/vertex_half_edge_reference.h"
#include "../components/tex_coord.h"

namespace vcl::vert {

/* Port AdjacentEdges class into vert namespace */
template<typename Edge>
using AdjacentEdges = comp::AdjacentEdges<Edge, -1>;

template<typename EdgeType, typename VertexType>
using OptionalAdjacentEdges = comp::AdjacentEdges<EdgeType, -1, VertexType, true>;

/* Port AdjacentFaces class into vert namespace */
template<typename FaceType>
using AdjacentFaces = comp::AdjacentFaces<FaceType, -1>;

template<typename FaceType, typename VertexType>
using OptionalAdjacentFaces = comp::AdjacentFaces<FaceType, -1, VertexType, true>;

/* Port AdjacentVertices class into vert namespace */
template<typename VertexType>
using AdjacentVertices = comp::AdjacentVertices<VertexType>;

template<typename VertexType>
using OptionalAdjacentVertices = comp::AdjacentVertices<VertexType, VertexType, true>;

/* Port BitFlags class into vert namespace */
using BitFlags = comp::BitFlags<>;

/* Port Color class into vert namespace */
using Color = comp::Color<>;

template<typename VertexType>
using VerticalColor = comp::Color<VertexType>; 

template<typename VertexType>
using OptionalColor = comp::Color<VertexType, true>;

/* Port Coordinate class into vert namespace */
template<typename ScalarType, int N>
using Coordinate = comp::Coordinate<ScalarType, N>;

template<typename ScalarType>
using Coordinate3 = comp::Coordinate3<ScalarType>;

using Coordinate3f = comp::Coordinate3f<>;
using Coordinate3d = comp::Coordinate3d<>;

template<typename ScalarType, int N, typename VertexType>
using VerticalCoordinate = comp::Coordinate<ScalarType, N, VertexType>;

template<typename ScalarType, typename VertexType>
using VerticalCoordinate3 = comp::Coordinate3<ScalarType, VertexType>;

template<typename VertexType>
using VerticalCoordinate3f = comp::Coordinate3f<VertexType>;

template<typename VertexType>
using VerticalCoordinate3d = comp::Coordinate3d<VertexType>;

/* Port CustomComponents class into vert namespace */
template<typename VertexType>
using CustomComponents = comp::CustomComponents<VertexType>;

/* Port Half Edge reference into vert namespace*/
template<typename HalfEdge>
using HalfEdgeReference = comp::VertexHalfEdgeReference<HalfEdge>;

/* Port Mark class into vert namespace */
using Mark = comp::Mark<>;

template<typename VertexType>
using OptionalMark = comp::Mark<VertexType, true>;

/* Port Normal classes into vert namespace */
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

template<typename ScalarType, int N, typename VertexType>
using OptionalNormal = comp::Normal<ScalarType, N, VertexType, true>;

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
using OptionalPrincipalCurvature = comp::PrincipalCurvature<ScalarType, VertexType, true>;

template<typename VertexType>
using OptionalPrincipalCurvaturef = comp::PrincipalCurvaturef<VertexType, true>;
template<typename VertexType>
using OptionalPrincipalCurvatured = comp::PrincipalCurvatured<VertexType, true>;

/* Port Scalar class into vert namespace */
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

template<typename ScalarType, typename VertexType>
using OptionalScalar = comp::Scalar<ScalarType, VertexType, true>;

template<typename VertexType>
using OptionalScalarf = comp::Scalar<float, VertexType, true>;

template<typename VertexType>
using OptionalScalard = comp::Scalar<double, VertexType, true>;

/* Port TexCoord class into vert namespace */
template<typename ScalarType>
using TexCoord = comp::TexCoord<ScalarType>;

using TexCoordf = comp::TexCoordf<>;
using TexCoordd = comp::TexCoordd<>;

template<typename ScalarType, typename VertexType>
using OptionalTexCoord = comp::TexCoord<ScalarType, VertexType, true>;

template<typename VertexType>
using OptionalTexCoordf = comp::TexCoord<float, VertexType, true>;

template<typename VertexType>
using OptionalTexCoordd = comp::TexCoord<double, VertexType, true>;

} // namespace vcl::vert

#endif // VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_H
