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

#ifndef VCL_MESH_ELEMENTS_EDGE_COMPONENTS_H
#define VCL_MESH_ELEMENTS_EDGE_COMPONENTS_H

#include "../components/adjacent_edges.h"
#include "../components/adjacent_faces.h"
#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/mark.h"
#include "../components/mutable_bit_flags.h"
#include "../components/scalar.h"
#include "../components/vertex_references.h"

namespace vcl::edge {

/** Port AdjacentEdges class into edge namespace **/
template<typename Edge>
using AdjacentEdges = comp::AdjacentEdges<Edge, -1>;

template<typename T>
bool constexpr hasAdjacentEdges()
{
	return comp::hasAdjacentEdges<T>();
}

/** Port AdjacentFaces class into edge namespace **/
template<typename Face>
using AdjacentFaces = comp::AdjacentFaces<Face, -1>;

template<typename T>
bool constexpr hasAdjacentFaces()
{
	return comp::hasAdjacentFaces<T>();
}

/** Port BitFlags class into edge namespace **/
using BitFlags = comp::BitFlags;

template<typename T>
bool constexpr hasBitFlags()
{
	return comp::hasBitFlags<T>();
}

/** Port Color class into edge namespace **/
using Color = comp::Color;

template<typename T>
bool constexpr hasColor()
{
	return comp::hasColor<T>();
}

/** Port Mark class into edge namespace **/
using Mark = comp::Mark;

template<typename T>
bool constexpr hasMark()
{
	return comp::hasMark<T>();
}

/** Port MutableBitFlags class into edge namespace **/
using MutableBitFlags = comp::MutableBitFlags;

template<typename T>
bool constexpr hasMutableBitFlags()
{
	return comp::hasMutableBitFlags<T>();
}

/** Port Scalar class into edge namespace **/
template<typename T>
using Scalar = comp::Scalar<T>;

using Scalarf = comp::Scalarf;
using Scalard = comp::Scalard;

template<typename T>
bool constexpr hasScalar()
{
	return comp::hasScalar<T>();
}

/** Port VertexReferences class into edge namespace **/
template<typename Vertex>
using VertexReferences = comp::VertexReferences<Vertex, 2>;

template<typename T>
bool constexpr hasVertexReferences()
{
	return comp::hasVertexReferences<T>();
}

} // namespace vcl::edge

#endif // VCL_MESH_ELEMENTS_EDGE_COMPONENTS_H
