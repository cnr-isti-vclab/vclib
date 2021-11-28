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

#include "../components/adjacent_edges.h"
#include "../components/adjacent_faces.h"
#include "../components/adjacent_vertices.h"
#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/coordinate.h"
#include "../components/mark.h"
#include "../components/mutable_bit_flags.h"
#include "../components/normal.h"
#include "../components/principal_curvature.h"
#include "../components/scalar.h"
#include "../components/tex_coord.h"
#include "../components/vertex_references.h"

namespace vcl::vert {

/* Port AdjacentEdges class into vert namespace */
template<typename Edge>
using AdjacentEdges = comp::AdjacentEdges<Edge, -1>;

template<typename VertexType>
bool constexpr hasAdjacentEdges()
{
	return comp::hasAdjacentEdges<VertexType>();
}

/* Port AdjacentFaces class into vert namespace */
template<typename FaceType>
using AdjacentFaces = comp::AdjacentFaces<FaceType, -1>;

template<typename VertexType>
bool constexpr hasAdjacentFaces()
{
	return comp::hasAdjacentFaces<VertexType>();
}

/* Port AdjacentVertices class into vert namespace */
template<typename VertexType>
using AdjacentVertices = comp::AdjacentVertices<VertexType, -1>;

template<typename VertexType>
bool constexpr hasAdjacentVertices()
{
	return comp::hasAdjacentVertices<VertexType>();
}

/* Port BitFlags class into vert namespace */
using BitFlags = comp::BitFlags;

template<typename VertexType>
bool constexpr hasBitFlags()
{
	return comp::hasBitFlags<VertexType>();
}

/* Port Color class into vert namespace */
using Color = comp::Color;

template<typename VertexType>
bool constexpr hasColor()
{
	return comp::hasColor<VertexType>();
}

/* Port Coordinate class into vert namespace */
template<typename ScalarType, int N>
using Coordinate = comp::Coordinate<ScalarType, N>;

template<typename ScalarType>
using Coordinate3 = comp::Coordinate3<ScalarType>;

using Coordinate3f = comp::Coordinate3f;
using Coordinate3d = comp::Coordinate3d;

template<typename VertexType>
bool constexpr hasCoordinate()
{
	return comp::hasCoordinate<VertexType>();
}

/* Port Mark class into vert namespace */
using Mark = comp::Mark;

template<typename VertexType>
bool constexpr hasMark()
{
	return comp::hasMark<VertexType>();
}

/* Port MutableBitFlags class into vert namespace */
using MutableBitFlags = comp::MutableBitFlags;

template<typename VertexType>
bool constexpr hasMutableBitFlags()
{
	return comp::hasMutableBitFlags<VertexType>();
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
