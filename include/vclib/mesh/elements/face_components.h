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

#ifndef VCL_MESH_ELEMENTS_FACE_COMPONENTS_H
#define VCL_MESH_ELEMENTS_FACE_COMPONENTS_H

#include "../components/horizontal/adjacent_edges.h"
#include "../components/horizontal/adjacent_faces.h"
#include "../components/horizontal/bit_flags.h"
#include "../components/horizontal/color.h"
#include "../components/horizontal/face_half_edge_reference.h"
#include "../components/horizontal/mark.h"
#include "../components/horizontal/normal.h"
#include "../components/horizontal/polygon_bit_flags.h"
#include "../components/horizontal/principal_curvature.h"
#include "../components/horizontal/scalar.h"
#include "../components/horizontal/triangle_bit_flags.h"
#include "../components/horizontal/vertex_references.h"
#include "../components/horizontal/wedge_colors.h"
#include "../components/horizontal/wedge_tex_coords.h"

namespace vcl::face {

/* Port AdjacentEdges class into face namespace */
template<typename EdgeType>
using AdjacentPolygonEdges = comp::AdjacentEdges<EdgeType, -1>;

template<typename EdgeType>
using AdjacentTriangleEdges = comp::AdjacentEdges<EdgeType, 3>;

template<typename FaceType>
bool constexpr hasAdjacentEdges()
{
	return comp::hasAdjacentEdges<FaceType>();
}

template<typename FaceType>
bool constexpr sanityCheckAdjacentEdges()
{
	return comp::sanityCheckAdjacentEdges<FaceType>();
}

/* Port AdjacentFaces class into face namespace */
template<typename FaceType>
using AdjacentPolygons = comp::AdjacentFaces<FaceType, -1>;

template<typename FaceType>
using AdjacentTriangles = comp::AdjacentFaces<FaceType, 3>;

template<typename FaceType>
bool constexpr hasAdjacentFaces()
{
	return comp::hasAdjacentFaces<FaceType>();
}

template<typename FaceType>
bool constexpr sanityCheckAdjacentFaces()
{
	return comp::sanityCheckAdjacentFaces<FaceType>();
}

/* Port BitFlags class into face namespace */
using BitFlags = comp::BitFlags;

template<typename FaceType>
bool constexpr hasBitFlags()
{
	return comp::hasBitFlags<FaceType>();
}

template<typename FaceType>
bool constexpr hasFaceBitFlags()
{
	return comp::hasFaceBitFlags<FaceType>();
}

/* Port Color class into face namespace */
using Color = comp::Color;

template<typename FaceType>
bool constexpr hasColor()
{
	return comp::hasColor<FaceType>();
}

/* Port Half Edge reference into face namespace*/
template<typename HalfEdge>
using HalfEdgeReference = comp::FaceHalfEdgeReference<HalfEdge>;

template<typename FaceType>
bool constexpr hasHalfEdgeReference()
{
	return comp::hasFaceHalfEdgeReference<FaceType>();
}

/* Port Mark class into face namespace */
using Mark = comp::Mark;

template<typename FaceType>
bool constexpr hasMark()
{
	return comp::hasMark<FaceType>();
}

/* Port Normal classes into face namespace */
template<typename ScalarType, int N>
using Normal = comp::Normal<ScalarType, N>;

template<typename ScalarType>
using Normal3 = comp::Normal3<ScalarType>;

using Normal3f = comp::Normal3f;
using Normal3d = comp::Normal3d;

template<typename FaceType>
bool constexpr hasNormal()
{
	return comp::hasNormal<FaceType>();
}

/* Port PolygonBitFlags class into face namespace */
using PolygonBitFlags = comp::PolygonBitFlags;

template<typename FaceType>
bool constexpr hasPolygonBitFlags()
{
	return comp::hasPolygonBitFlags<FaceType>();
}

/* Port PrincipalCurvature class into face namespace */
template<typename FaceType>
using PrincipalCurvature = comp::PrincipalCurvature<FaceType>;

using PrincipalCurvaturef = comp::PrincipalCurvaturef;
using PrincipalCurvatured = comp::PrincipalCurvatured;

template<typename FaceType>
bool constexpr hasPrincipalCurvature()
{
	return comp::hasPrincipalCurvature<FaceType>();
}

/* Port Scalar class into face namespace */
template<typename ScalarType>
using Scalar = comp::Scalar<ScalarType>;

using Scalarf = comp::Scalarf;
using Scalard = comp::Scalard;

template<typename FaceType>
bool constexpr hasScalar()
{
	return comp::hasScalar<FaceType>();
}

/* Port TriangleBitFlags class into face namespace */
using TriangleBitFlags = comp::TriangleBitFlags;

template<typename FaceType>
bool constexpr hasTriangleBitFlags()
{
	return comp::hasTriangleBitFlags<FaceType>();
}

/* Port VertexReferences class into face namespace */
template<typename Vertex, int N>
using VertexReferences = comp::VertexReferences<Vertex, N>;

template<typename Vertex>
using TriangleVertexRefs = comp::VertexReferences<Vertex, 3>;

template<typename Vertex>
using PolygonVertexRefs = comp::VertexReferences<Vertex, -1>;

template<typename FaceType>
bool constexpr hasVertexReferences()
{
	return comp::hasVertexReferences<FaceType>();
}

/* Port WedgeColors class into face namespace */
using PolygonWedgeColors = comp::WedgeColors<-1>;

using TriangleWedgeColors = comp::WedgeColors<3>;

template<typename FaceType>
bool constexpr hasWedgeColors()
{
	return comp::hasWedgeColors<FaceType>();
}

template<typename FaceType>
bool constexpr sanityCheckWedgeColors()
{
	return comp::sanityCheckWedgeColors<FaceType>();
}

/* Port WedgeTexCoords class into face namespace */
template<typename ScalarType>
using PolygonWedgeTexCoords = comp::WedgeTexCoords<ScalarType, -1>;

template<typename ScalarType>
using TriangleWedgeTexCoords = comp::WedgeTexCoords<ScalarType, 3>;

using PolygonWedgeTexCoordsf = comp::WedgeTexCoords<float, -1>;
using PolygonWedgeTexCoordsd = comp::WedgeTexCoords<double, -1>;
using TriangleWedgeTexCoordsf = TriangleWedgeTexCoords<float>;
using TriangleWedgeTexCoordsd = TriangleWedgeTexCoords<double>;

template<typename FaceType>
bool constexpr hasWedgeTexCoords()
{
	return comp::hasWedgeTexCoords<FaceType>();
}

template<typename FaceType>
bool constexpr sanityCheckWedgeTexCoords()
{
	return comp::sanityCheckWedgeTexCoords<FaceType>();
}

} // namespace vcl::face

#endif // VCL_MESH_ELEMENTS_FACE_COMPONENTS_H
