/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_CONCEPTS_SPACE_POINT_H
#define VCL_CONCEPTS_SPACE_POINT_H

#include <vclib/concepts/iterators.h>
#include <vclib/types.h>

namespace vcl {

/**
 * @brief Concept for types representing points in Euclidean space.
 *
 * A type `T` models the `PointConcept` if it provides the following:
 *
 * - `typename T::ScalarType`: a type that represents the scalar used for the
 * coordinates of the point.
 * - `o.DIM`: a static data member or constant expression that specifies the
 * dimension of the point.
 * - `o.isDegenerate()`: a member function that returns true if the point is
 * degenerate (e.g., the zero vector).
 * - `co.dot(co)`: a member function that returns the dot product of the point
 * with another point `co`.
 * - `co.angle(co)`: a member function that returns the angle between the point
 * and another point `co`.
 * - `co.dist(co)`: a member function that returns the Euclidean distance
 * between the point and another point `co`.
 * - `co.squaredDist(co)`: a member function that returns the squared Euclidean
 * distance between the point and another point `co`.
 * - `co.norm()`: a member function that returns the Euclidean norm (length) of
 * the point.
 * - `co.squaredNorm()`: a member function that returns the squared Euclidean
 * norm (length) of the point.
 * - `co.size()`: a member function that returns the size (number of
 * coordinates) of the point.
 * - `o.setConstant(typename T::ScalarType())`: a member function that sets all
 * coordinates of the point to a constant scalar value.
 * - `o.setZero()`: a member function that sets all coordinates of the point to
 * zero.
 * - `o.setOnes()`: a member function that sets all coordinates of the point to
 * one.
 * - `o.normalize()`: a member function that normalizes the point to have unit
 * length.
 * - `co.hash()`: a member function that returns a hash value for the point.
 *
 * @tparam T: The type to be tested for conformity to the PointConcept.
 */
template<typename T>
concept PointConcept = requires (T o, const T& co) {
    // clang-format off
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

    { co.normalized() } -> std::convertible_to<T>;
    { co + typename T::ScalarType() } -> std::convertible_to<T>;
    { co + co } -> std::convertible_to<T>;

    { -co } -> std::convertible_to<T>;
    { co - typename T::ScalarType() } -> std::convertible_to<T>;
    { co - co } -> std::convertible_to<T>;

    { co * typename T::ScalarType() } -> std::convertible_to<T>;

    { co / typename T::ScalarType() } -> std::convertible_to<T>;
    // clang-format on
};

/**
 * @brief A concept for points in two-dimensional space.
 *
 * A type satisfies this concept if it satisfies the `PointConcept` and if the
 * `DIM` value of the point type is 2.
 *
 * @tparam T: The type to be tested for conformity to the Point2Concept.
 */
template<typename T>
concept Point2Concept = PointConcept<T> && T::DIM == 2;

/**
 * @brief A concept for points in three-dimensional space.
 *
 * A type satisfies this concept if it satisfies the `PointConcept` and if the
 * `DIM` value of the point type is 3.
 *
 * @tparam T: The type to be tested for conformity to the Point3Concept.
 */
template<typename T>
concept Point3Concept = PointConcept<T> && T::DIM == 3;

/**
 * @brief A concept for points in four-dimensional space.
 *
 * A type satisfies this concept if it satisfies the `PointConcept` and if the
 * `DIM` value of the point type is 4.
 *
 * @tparam T: The type to be tested for conformity to the Point4Concept.
 */
template<typename T>
concept Point4Concept = PointConcept<T> && T::DIM == 4;

/**
 * @brief Concept for iterators that iterate over Points (class that satisfies
 * the PointConcept).
 *
 * A type satisfies this concept if it is an iterator having its `value_type`
 * that satisfies the `PointConcpet`.
 *
 * @tparam It: The type to be tested for conformity to the PointIteratorConcept.
 */
template<typename It>
concept PointIteratorConcept =
    IteratorConcept<It> && PointConcept<typename It::value_type>;

/**
 * @brief Concept for iterators that iterate over 2D Points (class that
 * satisfies the Point2Concept).
 *
 * A type satisfies this concept if it is an iterator having its `value_type`
 * that satisfies the `Point2Concpet`.
 *
 * @tparam It: The type to be tested for conformity to the
 * Point2IteratorConcept.
 */
template<typename It>
concept Point2IteratorConcept =
    IteratorConcept<It> && Point2Concept<typename It::value_type>;

/**
 * @brief Concept for iterators that iterate over 3D Points (class that
 * satisfies the Point3Concept).
 *
 * A type satisfies this concept if it is an iterator having its `value_type`
 * that satisfies the `Point3Concpet`.
 *
 * @tparam It: The type to be tested for conformity to the
 * Point3IteratorConcept.
 */
template<typename It>
concept Point3IteratorConcept =
    IteratorConcept<It> && Point3Concept<typename It::value_type>;

/**
 * @brief Concept for iterators that iterate over 4D Points (class that
 * satisfies the Point4Concept).
 *
 * A type satisfies this concept if it is an iterator having its `value_type`
 * that satisfies the `Point4Concpet`.
 *
 * @tparam It: The type to be tested for conformity to the
 * Point4IteratorConcept.
 */
template<typename It>
concept Point4IteratorConcept =
    IteratorConcept<It> && Point4Concept<typename It::value_type>;

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_POINT_H
