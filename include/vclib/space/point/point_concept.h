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

#ifndef VCL_SPACE_POINT_POINT_CONCEPT_H
#define VCL_SPACE_POINT_POINT_CONCEPT_H

#include <vclib/misc/types.h>

namespace vcl {

/**
 * @brief The PointConcept concept lists all the requirements that a template T must
 * satisfy to be treated by the VCLib as a Point of undefined dimension.
 *
 * Lists all the member functions and operators that a Point class must implement.
 */
template<typename T>
concept PointConcept = requires(T o, const T& co)
{
	typename T::ScalarType;
	o.DIM;
	o.isDegenerate();
	{ co.dot(co) } -> std::same_as<typename T::ScalarType>;
	{ co.angle(co) } -> std::same_as<typename T::ScalarType>;
	{ co.dist(co) } -> std::same_as<typename T::ScalarType>;
	{ co.squaredDist(co) } -> std::same_as<typename T::ScalarType>;
	{ co.norm() } -> std::same_as<typename T::ScalarType>;
	{ co.squaredNorm() } -> std::same_as<typename T::ScalarType>;
	{ co.size() } -> std::same_as<uint>;
	o.setConstant(typename T::ScalarType());
	o.setZero();
	o.setOnes();
	o.normalize();
	{ co.hash() } -> std::same_as<std::size_t>;

	o(uint());
	co(uint());
	o[uint()];
	co[uint()];

	o = co;

	{ co == co } -> std::same_as<bool>;
	co <=> co;

	{ co * co } -> std::same_as<typename T::ScalarType>;

	o += typename T::ScalarType();
	o += co;

	o -= typename T::ScalarType();
	o -= co;

	o *= typename T::ScalarType();
	o /= typename T::ScalarType();

	// waiting for apple to support std::convertible_to<> and other concept features...
#ifndef __APPLE__
	{ co.normalized() } -> std::convertible_to<T>;
	{ co + typename T::ScalarType() } -> std::convertible_to<T>;
	{ co + co } -> std::convertible_to<T>;

	{ -co } -> std::convertible_to<T>;
	{ co - typename T::ScalarType() } -> std::convertible_to<T>;
	{ co - co } -> std::convertible_to<T>;

	{ co * typename T::ScalarType() } -> std::convertible_to<T>;

	{ co / typename T::ScalarType() } -> std::convertible_to<T>;
#endif
};

} // namespace vcl

#endif // VCL_SPACE_POINT_POINT_CONCEPT_H
