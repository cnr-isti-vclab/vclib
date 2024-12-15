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

#include <vclib/concepts/const_correctness.h>
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
concept PointConcept = requires (
    T&&                                obj,
    typename RemoveRef<T>::ScalarType  s,
    typename RemoveRef<T>::ScalarType& sR,
    typename RemoveRef<T>::Base&       baseObjR) {
    typename RemoveRef<T>::ScalarType;
    typename RemoveRef<T>::Base;
    obj.DIM;

    RemoveRef<T>();
    RemoveRef<T>(obj);

    { obj.isDegenerate() } -> std::same_as<bool>;
    { obj.dot(obj) } -> std::same_as<decltype(s)>;
    { obj.angle(obj) } -> std::same_as<decltype(s)>;
    { obj.dist(obj) } -> std::same_as<decltype(s)>;
    { obj.squaredDist(obj) } -> std::same_as<decltype(s)>;
    { obj.norm() } -> std::same_as<decltype(s)>;
    { obj.squaredNorm() } -> std::same_as<decltype(s)>;
    { obj.size() } -> std::same_as<uint>;

    { obj.hash() } -> std::same_as<std::size_t>;

    { obj(uint()) } -> std::convertible_to<decltype(s)>;
    { obj[uint()] } -> std::convertible_to<decltype(s)>;

    { obj == obj } -> std::same_as<bool>;
    { obj <=> obj } -> std::convertible_to<std::partial_ordering>;

    { obj.normalized() } -> std::convertible_to<RemoveRef<T>>;
    { obj + s } -> std::convertible_to<RemoveRef<T>>;
    { obj + obj } -> std::convertible_to<RemoveRef<T>>;

    { -obj } -> std::convertible_to<RemoveRef<T>>;
    { obj - s } -> std::convertible_to<RemoveRef<T>>;
    { obj - obj } -> std::convertible_to<RemoveRef<T>>;

    { obj* s } -> std::convertible_to<RemoveRef<T>>;

    { obj / s } -> std::convertible_to<RemoveRef<T>>;

    // non const requirements
    requires vcl::IsConst<T> || requires {
        { obj.setConstant(s) } -> std::same_as<decltype(baseObjR)>;
        { obj.setZero() } -> std::same_as<decltype(baseObjR)>;
        { obj.setOnes() } -> std::same_as<decltype(baseObjR)>;
        { obj.normalize() } -> std::same_as<void>;

        { obj(uint()) } -> std::same_as<decltype(sR)>;
        { obj[uint()] } -> std::same_as<decltype(sR)>;

        { obj = obj } -> std::same_as<T&>;

        { obj += s } -> std::same_as<T&>;
        { obj += obj } -> std::same_as<decltype(baseObjR)>;

        { obj -= s } -> std::same_as<T&>;
        { obj -= obj } -> std::same_as<decltype(baseObjR)>;

        { obj *= s } -> std::same_as<decltype(baseObjR)>;
        { obj /= s } -> std::same_as<decltype(baseObjR)>;
    };
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
concept Point2Concept = PointConcept<T> && RemoveRef<T>::DIM == 2;

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
concept Point3Concept =
    PointConcept<T> && RemoveRef<T>::DIM == 3 && requires (T&& obj) {
        { obj.cross(obj) } -> std::convertible_to<RemoveRef<T>>;
    };

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
concept Point4Concept = PointConcept<T> && RemoveRef<T>::DIM == 4;

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
