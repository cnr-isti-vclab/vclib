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

#ifndef VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_OPTIONAL_H
#define VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_OPTIONAL_H

#include "../components/vertical/persistent/custom_components.h"
#include "../components/vertical/optional/optional_adjacent_edges.h"
#include "../components/vertical/optional/optional_adjacent_faces.h"
#include "../components/vertical/optional/optional_adjacent_vertices.h"
#include "../components/vertical/optional/optional_color.h"
#include "../components/vertical/optional/optional_mark.h"
#include "../components/vertical/optional/optional_normal.h"
#include "../components/vertical/optional/optional_principal_curvature.h"
#include "../components/vertical/optional/optional_scalar.h"
#include "../components/vertical/optional/optional_tex_coord.h"

namespace vcl::vert {

/* Port OptionalAdjacentEdges class into vert namespace */
template<typename EdgeType, typename VertexType>
using OptionalAdjacentEdges = comp::OptionalAdjacentEdges<EdgeType, -1, VertexType>;

template<typename VertexType>
bool constexpr hasOptionalAdjacentEdges()
{
	return comp::hasOptionalAdjacentEdges<VertexType>();
}

/* Port OptionalAdjacentFaces class into vert namespace */
template<typename FaceType, typename VertexType>
using OptionalAdjacentFaces = comp::OptionalAdjacentFaces<FaceType, -1, VertexType>;

template<typename VertexType>
bool constexpr hasOptionalAdjacentFaces()
{
	return comp::hasOptionalAdjacentFaces<VertexType>();
}

/* Port OptionalAdjacentVertices class into vert namespace */
template<typename VertexType>
using OptionalAdjacentVertices = comp::OptionalAdjacentVertices<VertexType, VertexType>;

template<typename VertexType>
bool constexpr hasOptionalAdjacentVertices()
{
	return comp::hasOptionalAdjacentVertices<VertexType>();
}

/* Port OptionalColor class into vert namespace */
template<typename VertexType>
using OptionalColor = comp::OptionalColor<VertexType>;

template<typename VertexType>
bool constexpr hasOptionalColor()
{
	return comp::hasOptionalColor<VertexType>();
}

/* Port OptionalMark class into vert namespace */
template<typename VertexType>
using OptionalMark = comp::OptionalMark<VertexType>;

template<typename VertexType>
bool constexpr hasOptionalMark()
{
	return comp::hasOptionalMark<VertexType>();
}

/* Port OptionalNormal class into vert namespace */
template<typename ScalarType, int N, typename VertexType>
using OptionalNormal = comp::OptionalNormal<ScalarType, N, VertexType>;

template<typename ScalarType, typename VertexType>
using OptionalNormal3 = comp::OptionalNormal3<ScalarType, VertexType>;

template<typename VertexType>
using OptionalNormal3f = comp::OptionalNormal3f<VertexType>;

template<typename VertexType>
using OptionalNormal3d = comp::OptionalNormal3d<VertexType>;

template<typename VertexType>
bool constexpr hasOptionalNormal()
{
	return comp::hasOptionalNormal<VertexType>();
}

/* Port OptionalPrincipalCurvature class into vert namespace */
template<typename ScalarType, typename VertexType>
using OptionalPrincipalCurvature = comp::OptionalPrincipalCurvature<ScalarType, VertexType>;

template<typename VertexType>
using OptionalPrincipalCurvaturef = comp::OptionalPrincipalCurvaturef<VertexType>;
template<typename VertexType>
using OptionalPrincipalCurvatured = comp::OptionalPrincipalCurvatured<VertexType>;

template<typename VertexType>
bool constexpr hasOptionalPrincipalCurvature()
{
	return comp::hasOptionalPrincipalCurvature<VertexType>();
}

/* Port OptionalScalar class into vert namespace */
template<typename ScalarType, typename VertexType>
using OptionalScalar = comp::OptionalScalar<ScalarType, VertexType>;

template<typename VertexType>
using OptionalScalarf = comp::OptionalScalar<float, VertexType>;

template<typename VertexType>
using OptionalScalard = comp::OptionalScalar<double, VertexType>;

template<typename VertexType>
bool constexpr hasOptionalScalar()
{
	return comp::hasOptionalScalar<VertexType>();
}

/* Port OptionalTexCoord class into vert namespace */
template<typename ScalarType, typename VertexType>
using OptionalTexCoord = comp::OptionalTexCoord<ScalarType, VertexType>;

template<typename VertexType>
using OptionalTexCoordf = comp::OptionalTexCoord<float, VertexType>;

template<typename VertexType>
using OptionalTexCoordd = comp::OptionalTexCoord<double, VertexType>;

template<typename VertexType>
bool constexpr hasOptionalTexCoord()
{
	return comp::hasOptionalTexCoord<VertexType>();
}

/* Port CustomComponents class into vert namespace */
template<typename VertexType>
using CustomComponents = comp::CustomComponents<VertexType>;

template<typename VertexType>
bool constexpr hasCustomComponents()
{
	return comp::hasCustomComponents<VertexType>();
}

/* Port VerticalInfo class into vert namespace */
template<typename VertexType>
using VerticalInfo = comp::VerticalInfo<VertexType>;

template<typename VertexType>
bool constexpr hasVerticalInfo()
{
	return comp::hasVerticalInfo<VertexType>();
}

} // namespace vcl::vert

#endif // VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_OPTIONAL_H
