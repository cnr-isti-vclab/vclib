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

#ifndef VCL_MESH_ELEMENTS_FACE_COMPONENTS_OPTIONAL_H
#define VCL_MESH_ELEMENTS_FACE_COMPONENTS_OPTIONAL_H

#include "../components_optional/custom_components.h"
#include "../components_optional/optional_adjacent_faces.h"
#include "../components_optional/optional_color.h"
#include "../components_optional/optional_mark.h"
#include "../components_optional/optional_mutable_bit_flags.h"
#include "../components_optional/optional_normal.h"
#include "../components_optional/optional_scalar.h"
#include "../components_optional/optional_wedge_tex_coords.h"

namespace vcl::face {

/** Port OptionalInfo class into face namespace **/
template<typename T>
using OptionalInfo = comp::OptionalInfo<T>;

template<typename T>
bool constexpr hasOptionalInfo()
{
	return comp::hasOptionalInfo<T>();
}

/** Port OptionalMutableBitFlags class into face namespace **/
template<typename T>
using OptionalMutableBitFlags = comp::OptionalMutableBitFlags<T>;

template<typename T>
bool constexpr hasOptionalMutableBitFlags()
{
	return comp::hasOptionalMutableBitFlags<T>();
}

template<typename U, typename T>
using ReturnIfHasOptionalMutableBitFlags = comp::ReturnIfHasOptionalMutableBitFlags<U, T>;

/** Port OptionalColor class into face namespace **/
template<typename T>
using OptionalColor = comp::OptionalColor<T>;

template<typename T>
bool constexpr hasOptionalColor()
{
	return comp::hasOptionalColor<T>();
}

template<typename U, typename T>
using ReturnIfHasOptionalColor = comp::ReturnIfHasOptionalColor<U, T>;

/** Port OptionalNormal class into face namespace **/
template<typename Scalar, int N, typename T>
using OptionalNormal = comp::OptionalNormal<Scalar, N, T>;

template<typename Scalar, typename T>
using OptionalNormal3 = comp::OptionalNormal3<Scalar, T>;

template<typename T>
using OptionalNormal3f = comp::OptionalNormal3f<T>;

template<typename T>
using OptionalNormal3d = comp::OptionalNormal3d<T>;

template<typename T>
bool constexpr hasOptionalNormal()
{
	return comp::hasOptionalNormal<T>();
}

template<typename U, typename T>
using ReturnIfHasOptionalNormal = comp::ReturnIfHasOptionalNormal<U, T>;

/** Port OptionalScalar class into face namespace **/
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

template<typename U, typename T>
using ReturnIfHasOptionalScalar = comp::ReturnIfHasOptionalScalar<U, T>;

/** Port OptionalMark class into face namespace **/
template<typename T>
using OptionalMark = comp::OptionalMark<T>;

template<typename T>
bool constexpr hasOptionalMark()
{
	return comp::hasOptionalMark<T>();
}

template<typename U, typename T>
using ReturnIfHasOptionalMark = comp::ReturnIfHasOptionalMark<U, T>;

/** Port OptionalAdjacentFaces class into face namespace **/
template<typename T>
using OptionalAdjacentFaces = comp::OptionalAdjacentFaces<T, -1, T>;

template<typename T>
using OptionalAdjacentTriangles = comp::OptionalAdjacentFaces<T, 3, T>;

template<typename T>
bool constexpr hasOptionalAdjacentFaces()
{
	return comp::hasOptionalAdjacentFaces<T>();
}

template<typename U, typename T>
using ReturnIfHasOptionalAdjacentFaces = comp::ReturnIfHasOptionalAdjacentFaces<U, T>;

/** Port OptionalWedgeTexCoords class into face namespace **/
template<typename S, typename T>
using OptionalWedgeTexCoords = comp::OptionalWedgeTexCoords<S, -1, T>;

template<typename S, typename T>
using OptionalTriangleAdjacentTriangles = comp::OptionalWedgeTexCoords<S, 3, T>;

template<typename T>
bool constexpr hasOptionalWedgeTexCoords()
{
	return comp::hasOptionalWedgeTexCoords<T>();
}

template<typename U, typename T>
using ReturnIfHasOptionalWedgeTexCoords = comp::ReturnIfHasOptionalWedgeTexCoords<U, T>;

template<typename T>
using OptionalWedgeTexCoordsf = comp::OptionalWedgeTexCoords<float, -1, T>;
template<typename T>
using OptionalWedgeTexCoordsd = comp::OptionalWedgeTexCoords<double, -1, T>;
template<typename T>
using OptionalTriangleWedgeTexCoordsf = comp::OptionalWedgeTexCoords<float, 3, T>;
template<typename T>
using OptionalTriangleWedgeTexCoordsd = comp::OptionalWedgeTexCoords<double, 3, T>;

/** Port CustomComponents class into face namespace **/
template<typename T>
using CustomComponents = comp::CustomComponents<T>;

template<typename T>
bool constexpr hasCustomComponents()
{
	return comp::hasCustomComponents<T>();
}

template<typename U, typename T>
using ReturnIfHasCustomComponents = comp::ReturnIfHasCustomComponents<U, T>;

} // namespace vcl::face

#endif // VCL_MESH_ELEMENTS_FACE_COMPONENTS_OPTIONAL_H
