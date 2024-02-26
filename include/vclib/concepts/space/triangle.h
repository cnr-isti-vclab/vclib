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

#ifndef VCL_CONCEPTS_SPACE_TRIANGLE_H
#define VCL_CONCEPTS_SPACE_TRIANGLE_H

#include <vclib/types.h>

namespace vcl {

template<typename T>
concept ConstTriangleConcept = requires (const T& co) {
    // clang-format off
    typename T::ScalarType;
    typename T::PointType;

    co.DIM;
    co.size() == 3;

    { co.point(uint()) } -> std::same_as<const typename T::PointType&>;
    { co.sideLength(uint()) } -> std::same_as<typename T::ScalarType>;
    { co.barycenter() } -> std::same_as<typename T::PointType>;
    { co.perimeter() } -> std::same_as<typename T::ScalarType>;
    { co.area() } -> std::same_as<typename T::ScalarType>;
    // clang-format on
};

template<typename T>
concept TriangleConcept =
    ConstTriangleConcept<T> && requires (T o, const T& co) {
        // clang-format off
    { o.point(uint()) } -> std::same_as<typename T::PointType&>;
        // clang-format on
    };

template<typename T>
concept ConstTriangle2Concept = ConstTriangleConcept<T> && T::DIM == 2;

template<typename T>
concept Triangle2Concept = ConstTriangle2Concept<T> && TriangleConcept<T>;

template<typename T>
concept ConstTriangle3Concept =
    ConstTriangleConcept<T> && T::DIM == 3 && requires (const T& co) {
        // clang-format off
    { co.normal() } -> std::same_as<typename T::PointType>;
        // clang-format on
    };

template<typename T>
concept Triangle3Concept = ConstTriangle3Concept<T> && TriangleConcept<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_TRIANGLE_H
