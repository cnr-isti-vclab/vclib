/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "../components/horizontal/adjacent_edges.h"
#include "../components/horizontal/adjacent_faces.h"
#include "../components/horizontal/adjacent_vertices.h"
#include "../components/horizontal/bit_flags.h"
#include "../components/horizontal/color.h"
#include "../components/horizontal/coordinate.h"
#include "../components/horizontal/mark.h"
#include "../components/horizontal/normal.h"
#include "../components/horizontal/principal_curvature.h"
#include "../components/horizontal/scalar.h"
#include "../components/horizontal/vertex_half_edge_reference.h"
#include "../components/horizontal/tex_coord.h"

namespace vcl::vert {

/* Port AdjacentEdges class into vert namespace */
template<typename Edge>
using AdjacentEdges = comp::AdjacentEdges<Edge, -1>;

/* Port AdjacentFaces class into vert namespace */
template<typename FaceType>
using AdjacentFaces = comp::AdjacentFaces<FaceType, -1>;

/* Port AdjacentVertices class into vert namespace */
template<typename VertexType>
using AdjacentVertices = comp::AdjacentVertices<VertexType>;

/* Port BitFlags class into vert namespace */
using BitFlags = comp::BitFlags;

/* Port Color class into vert namespace */
using Color = comp::Color;

/* Port Coordinate class into vert namespace */
template<typename ScalarType, int N>
using Coordinate = comp::Coordinate<ScalarType, N>;

template<typename ScalarType>
using Coordinate3 = comp::Coordinate3<ScalarType>;

using Coordinate3f = comp::Coordinate3f;
using Coordinate3d = comp::Coordinate3d;

/* Port Half Edge reference into vert namespace*/
template<typename HalfEdge>
using HalfEdgeReference = comp::VertexHalfEdgeReference<HalfEdge>;

template<typename VertexType>
bool constexpr hasHalfEdgeReference()
{
	return comp::hasVertexHalfEdgeReference<VertexType>();
}

/* Port Mark class into vert namespace */
using Mark = comp::Mark;

template<typename VertexType>
bool constexpr hasMark()
{
	return comp::hasMark<VertexType>();
}

/* Port Normal classes into vert namespace */
template<typename ScalarType, int N>
using Normal = comp::Normal<ScalarType, N>;

template<typename ScalarType>
using Normal3 = comp::Normal3<ScalarType>;

using Normal3f = comp::Normal3f;
using Normal3d = comp::Normal3d;

template<typename VertexType>
bool constexpr hasNormal()
{
	return comp::hasNormal<VertexType>();
}

/* Port PrincipalCurvature class into face namespace */
template<typename ScalarType>
using PrincipalCurvature = comp::PrincipalCurvature<ScalarType>;

using PrincipalCurvaturef = comp::PrincipalCurvaturef;
using PrincipalCurvatured = comp::PrincipalCurvatured;

template<typename VertexType>
bool constexpr hasPrincipalCurvature()
{
	return comp::hasPrincipalCurvature<VertexType>();
}

/* Port Scalar class into vert namespace */
template<typename ScalarType>
using Scalar = comp::Scalar<ScalarType>;

using Scalarf = comp::Scalarf;
using Scalard = comp::Scalard;

template<typename VertexType>
bool constexpr hasScalar()
{
	return comp::hasScalar<VertexType>();
}

/* Port TexCoord class into vert namespace */
template<typename ScalarType>
using TexCoord = comp::TexCoord<ScalarType>;

template<typename VertexType>
bool constexpr hasTexCoord()
{
	return comp::hasTexCoord<VertexType>();
}

using TexCoordf = comp::TexCoordf;
using TexCoordd = comp::TexCoordd;

} // namespace vcl::vert

#endif // VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_H
