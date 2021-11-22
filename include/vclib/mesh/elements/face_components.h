/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#include "../components/adjacent_faces.h"
#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/mark.h"
#include "../components/mutable_bit_flags.h"
#include "../components/normal.h"
#include "../components/polygon_bit_flags.h"
#include "../components/scalar.h"
#include "../components/triangle_bit_flags.h"
#include "../components/vertex_references.h"
#include "../components/wedge_colors.h"
#include "../components/wedge_tex_coords.h"

namespace vcl::face {

/** Port AdjacentFaces class into face namespace **/
template<typename Face>
using AdjacentFaces = comp::AdjacentFaces<Face, -1>;

template<typename Face>
using AdjacentTriangles = comp::AdjacentFaces<Face, 3>;

template<typename T>
bool constexpr hasAdjacentFaces()
{
	return comp::hasAdjacentFaces<T>();
}

/** Port BitFlags class into face namespace **/
using BitFlags = comp::BitFlags;

template<typename T>
bool constexpr hasBitFlags()
{
	return comp::hasBitFlags<T>();
}

/** Port Color class into face namespace **/
using Color = comp::Color;

template<typename T>
bool constexpr hasColor()
{
	return comp::hasColor<T>();
}

/** Port Mark class into face namespace **/
using Mark = comp::Mark;

template<typename T>
bool constexpr hasMark()
{
	return comp::hasMark<T>();
}

/** Port MutableBitFlags class into face namespace **/
using MutableBitFlags = comp::MutableBitFlags;

template<typename T>
bool constexpr hasMutableBitFlags()
{
	return comp::hasMutableBitFlags<T>();
}

/** Port Normal classes into face namespace **/
template<typename Scalar, int N>
using Normal = comp::Normal<Scalar, N>;

template<typename Scalar>
using Normal3 = comp::Normal3<Scalar>;

using Normal3f = comp::Normal3f;
using Normal3d = comp::Normal3d;

template<typename T>
bool constexpr hasNormal()
{
	return comp::hasNormal<T>();
}

/** Port PolygonBitFlags class into face namespace **/
using PolygonBitFlags = comp::PolygonBitFlags;

template<typename T>
bool constexpr hasPolygonBitFlags()
{
	return comp::hasPolygonBitFlags<T>();
}

/** Port Scalar class into face namespace **/
template<typename T>
using Scalar = comp::Scalar<T>;

using Scalarf = comp::Scalarf;
using Scalard = comp::Scalard;

template<typename T>
bool constexpr hasScalar()
{
	return comp::hasScalar<T>();
}

/** Port TriangleBitFlags class into face namespace **/
using TriangleBitFlags = comp::TriangleBitFlags;

template<typename T>
bool constexpr hasTriangleBitFlags()
{
	return comp::hasTriangleBitFlags<T>();
}

/** Port VertexReferences class into face namespace **/
template<typename Vertex, int N>
using VertexReferences = comp::VertexReferences<Vertex, N>;

template<typename Vertex>
using TriangleVertexRefs = comp::TriVertexReferences<Vertex>;

template<typename Vertex>
using PolygonVertexRefs = comp::VertexReferences<Vertex, -1>;

template<typename T>
bool constexpr hasVertexReferences()
{
	return comp::hasVertexReferences<T>();
}

/** Port WedgeColors class into face namespace **/
template<typename Scalar>
using WedgeTexCoords = comp::WedgeTexCoords<Scalar, -1>;

template<typename Scalar>
using TriangleWedgeTexCoords = comp::WedgeTexCoords<Scalar, 3>;

using WedgeTexCoordsf         = comp::WedgeTexCoords<float, -1>;
using WedgeTexCoordsd         = comp::WedgeTexCoords<double, -1>;
using TriangleWedgeTexCoordsf = TriangleWedgeTexCoords<float>;
using TriangleWedgeTexCoordsd = TriangleWedgeTexCoords<double>;

template<typename T>
bool constexpr hasWedgeTexCoords()
{
	return comp::hasWedgeTexCoords<T>();
}

/** Port WedgeColors class into face namespace **/
using WedgeColors = comp::WedgeColors<-1>;

using TriangleWedgeColors = comp::WedgeColors<3>;

template<typename T>
bool constexpr hasWedgeColors()
{
	return comp::hasWedgeColors<T>();
}

} // namespace vcl::face

#endif // VCL_MESH_ELEMENTS_FACE_COMPONENTS_H
