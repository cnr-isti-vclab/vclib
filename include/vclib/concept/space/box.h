/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_CONCEPT_SPACE_BOX_H
#define VCL_CONCEPT_SPACE_BOX_H

#include <vclib/misc/types.h>

namespace vcl {

template<typename T>
concept BoxConcept = requires(
	T o,
	const T& co,
	const typename T::PointType& p)
{
	typename T::PointType;
	o.DIM;

	{ co.min() } -> std::same_as<const typename T::PointType&>;
	{ o.min() } -> std::same_as<typename T::PointType&>;
	{ co.max() } -> std::same_as<const typename T::PointType&>;
	{ o.max() } -> std::same_as<typename T::PointType&>;
	{ co.isNull() } -> std::same_as<bool>;
	{ co.isEmpty() } -> std::same_as<bool>;
	{ co.isInside(p) }  -> std::same_as<bool>;
	{ co.isInsideOpenBox(p) }  -> std::same_as<bool>;
	{ co.collide(T()) }  -> std::same_as<bool>;

	{ co.diagonal() } -> std::same_as<typename T::PointType::ScalarType>;
	{ co.squaredDiagonal() } -> std::same_as<typename T::PointType::ScalarType>;
	{ co.center() } -> std::same_as<typename T::PointType>;
	{ co.size() } -> std::same_as<typename T::PointType>;
	{ co.volume() } -> std::same_as<typename T::PointType::ScalarType>;
	{ co.dim(uint()) } -> std::same_as<typename T::PointType::ScalarType>;
	{ co.minDim() } -> std::same_as<typename T::PointType::ScalarType>;
	{ co.maxDim() } -> std::same_as<typename T::PointType::ScalarType>;
};

} // namespace vcl

#endif // VCL_CONCEPT_SPACE_BOX_H
