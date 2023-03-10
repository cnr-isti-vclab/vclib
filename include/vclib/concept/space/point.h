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

#ifndef VCL_CONCEPT_SPACE_POINT_H
#define VCL_CONCEPT_SPACE_POINT_H

#include <vclib/misc/types.h>

namespace vcl {

/**
 * @brief A concept that defines the requirements for a type to be considered a point in space.
 *
 * A type that satisfies the PointConcept must have a member `ScalarType`, which indicates the type
 * of the elements of the point, and a member `DIM`, which indicates the dimension of the point.
 *
 * In addition, the type must have a set of methods that represent common operations on points, such
 * as dot product, norm, distance, and so on.
 *
 * @tparam T: The type to be tested for conformity to the PointConcept.
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

/**
 * @brief A concept for points in two-dimensional space.
 *
 * A type satisfies this concept if it satisfies the `PointConcept` and if the `DIM` value
 * of the point type is 2.
 *
 * @tparam T: The type to be tested for conformity to the Point2Concept.
 */
template<typename T>
concept Point2Concept = PointConcept<T> && T::DIM == 2;

/**
 * @brief A concept for points in three-dimensional space.
 *
 * A type satisfies this concept if it satisfies the `PointConcept` and if the `DIM` value
 * of the point type is 3.
 *
 * @tparam T: The type to be tested for conformity to the Point3Concept.
 */
template<typename T>
concept Point3Concept = PointConcept<T> && T::DIM == 3;

/**
 * @brief A concept for points in four-dimensional space.
 *
 * A type satisfies this concept if it satisfies the `PointConcept` and if the `DIM` value
 * of the point type is 4.
 *
 * @tparam T: The type to be tested for conformity to the Point4Concept.
 */
template<typename T>
concept Point4Concept = PointConcept<T> && T::DIM == 4;

} // namespace vcl

#endif // VCL_CONCEPT_SPACE_POINT_H
