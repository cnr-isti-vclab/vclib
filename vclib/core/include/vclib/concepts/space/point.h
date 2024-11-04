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
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_CONCEPTS_SPACE_POINT_H
#define VCL_CONCEPTS_SPACE_POINT_H

#include <vclib/concepts/iterators.h>
#include <vclib/types.h>

namespace vcl {

/**
 * @brief Concept for types representing points in Euclidean space.
 *
 * @tparam T: The type to be tested for conformity to the PointConcept.
 *
 * @ingroup space_concepts
 */
template<typename T>
concept PointConcept = requires (T o, const T& co) {
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

    { co* typename T::ScalarType() } -> std::convertible_to<T>;

    { co / typename T::ScalarType() } -> std::convertible_to<T>;
};

/**
 * @brief Concept for points in two-dimensional space.
 *
 * A type satisfies this concept if it satisfies the vcl::PointConcept and if
 * the `DIM` value of the point type is 2.
 *
 * @tparam T: The type to be tested for conformity to the Point2Concept.
 *
 * @ingroup space_concepts
 */
template<typename T>
concept Point2Concept = PointConcept<T> && T::DIM == 2;

/**
 * @brief Concept for points in three-dimensional space.
 *
 * A type satisfies this concept if it satisfies the vcl::PointConcept and if
 * the `DIM` value of the point type is 3.
 *
 * @tparam T: The type to be tested for conformity to the Point3Concept.
 *
 * @ingroup space_concepts
 */
template<typename T>
concept Point3Concept = PointConcept<T> && T::DIM == 3;

/**
 * @brief Concept for points in four-dimensional space.
 *
 * A type satisfies this concept if it satisfies the vcl::PointConcept and if
 * the `DIM` value of the point type is 4.
 *
 * @tparam T: The type to be tested for conformity to the Point4Concept.
 *
 * @ingroup space_concepts
 */
template<typename T>
concept Point4Concept = PointConcept<T> && T::DIM == 4;

/**
 * @brief Concept for iterators that iterate over Points (class that satisfies
 * the vcl::PointConcept).
 *
 * A type satisfies this concept if it is an iterator having its `value_type`
 * that satisfies the vcl::PointConcept.
 *
 * @tparam It: The type to be tested for conformity to the PointIteratorConcept.
 *
 * @ingroup space_concepts
 */
template<typename It>
concept PointIteratorConcept =
    IteratorConcept<It> && PointConcept<typename It::value_type>;

/**
 * @brief Concept for iterators that iterate over 2D Points (class that
 * satisfies the vcl::Point2Concept).
 *
 * A type satisfies this concept if it is an iterator having its `value_type`
 * that satisfies the vcl::Point2Concept.
 *
 * @tparam It: The type to be tested for conformity to the
 * Point2IteratorConcept.
 *
 * @ingroup space_concepts
 */
template<typename It>
concept Point2IteratorConcept =
    IteratorConcept<It> && Point2Concept<typename It::value_type>;

/**
 * @brief Concept for iterators that iterate over 3D Points (class that
 * satisfies the vcl::Point3Concept).
 *
 * A type satisfies this concept if it is an iterator having its `value_type`
 * that satisfies the vcl::Point3Concept.
 *
 * @tparam It: The type to be tested for conformity to the
 * Point3IteratorConcept.
 *
 * @ingroup space_concepts
 */
template<typename It>
concept Point3IteratorConcept =
    IteratorConcept<It> && Point3Concept<typename It::value_type>;

/**
 * @brief Concept for iterators that iterate over 4D Points (class that
 * satisfies the vcl::Point4Concept).
 *
 * A type satisfies this concept if it is an iterator having its `value_type`
 * that satisfies the vcl::Point4Concept.
 *
 * @tparam It: The type to be tested for conformity to the
 * Point4IteratorConcept.
 *
 * @ingroup space_concepts
 */
template<typename It>
concept Point4IteratorConcept =
    IteratorConcept<It> && Point4Concept<typename It::value_type>;

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_POINT_H
