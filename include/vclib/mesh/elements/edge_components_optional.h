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

#ifndef VCL_MESH_ELEMENTS_EDGE_COMPONENTS_OPTIONAL_H
#define VCL_MESH_ELEMENTS_EDGE_COMPONENTS_OPTIONAL_H

#include "../components_optional/custom_components.h"
#include "../components_optional/optional_adjacent_edges.h"
#include "../components_optional/optional_adjacent_faces.h"
#include "../components_optional/optional_color.h"
#include "../components_optional/optional_mark.h"
#include "../components_optional/optional_mutable_bit_flags.h"
#include "../components_optional/optional_scalar.h"

namespace vcl::edge {

/** Port OptionalAdjacentEdges class into edge namespace **/
template<typename T>
using OptionalAdjacentEdges = comp::OptionalAdjacentEdges<T, -1, T>;

template<typename T>
bool constexpr hasOptionalAdjacentEdges()
{
	return comp::hasOptionalAdjacentEdges<T>();
}

/** Port OptionalAdjacentFaces class into edge namespace **/
template<typename Face, typename T>
using OptionalAdjacentFaces = comp::OptionalAdjacentFaces<Face, -1, T>;

template<typename T>
bool constexpr hasOptionalAdjacentFaces()
{
	return comp::hasOptionalAdjacentFaces<T>();
}

/** Port OptionalColor class into edge namespace **/
template<typename T>
using OptionalColor = comp::OptionalColor<T>;

template<typename T>
bool constexpr hasOptionalColor()
{
	return comp::hasOptionalColor<T>();
}

/** Port OptionalInfo class into edge namespace **/
template<typename T>
using OptionalInfo = comp::OptionalInfo<T>;

template<typename T>
bool constexpr hasOptionalInfo()
{
	return comp::hasOptionalInfo<T>();
}

/** Port OptionalMark class into edge namespace **/
template<typename T>
using OptionalMark = comp::OptionalMark<T>;

template<typename T>
bool constexpr hasOptionalMark()
{
	return comp::hasOptionalMark<T>();
}

/** Port OptionalMutableBitFlags class into edge namespace **/
template<typename T>
using OptionalMutableBitFlags = comp::OptionalMutableBitFlags<T>;

template<typename T>
bool constexpr hasOptionalMutableBitFlags()
{
	return comp::hasOptionalMutableBitFlags<T>();
}

/** Port OptionalScalar class into edge namespace **/
template<typename S, typename T>
using OptionalScalar = comp::OptionalScalar<S, T>;

template<typename T>
using OptionalScalarf = comp::OptionalScalar<float, T>;

template<typename T>
using OptionalScalard = comp::OptionalScalar<double, T>;

template<typename T>
bool constexpr hasOptionalScalar()
{
	return comp::hasOptionalScalar<T>();
}

/** Port CustomComponents class into face namespace **/
template<typename T>
using CustomComponents = comp::CustomComponents<T>;

template<typename T>
bool constexpr hasCustomComponents()
{
	return comp::hasCustomComponents<T>();
}

} // namespace vcl::edge

#endif // VCL_MESH_ELEMENTS_EDGE_COMPONENTS_OPTIONAL_H
