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

/**
 * @brief Concept for types representing const triangles in Euclidean space.
 *
 * This concept is less strict than the vcl::TriangleConcept as it does not
 * require the type to be mutable (i.e., it does not require the type to provide
 * non-const member functions).
 *
 * @ingroup space_concepts
 */
template<typename T>
concept ConstTriangleConcept = requires (const T& cObj) {
    typename T::ScalarType;
    typename T::PointType;

    cObj.DIM;
    cObj.size() == 3;

    { cObj.point(uint()) } -> std::same_as<const typename T::PointType&>;
    { cObj.sideLength(uint()) } -> std::same_as<typename T::ScalarType>;
    { cObj.barycenter() } -> std::same_as<typename T::PointType>;
    { cObj.perimeter() } -> std::same_as<typename T::ScalarType>;
    { cObj.area() } -> std::same_as<typename T::ScalarType>;
};

/**
 * @brief Concept for types representing triangles in Euclidean space.
 *
 * This concept is more strict than the vcl::ConstTriangleConcept as it requires
 * the type to be mutable (i.e., it requires the type to provide non-const
 * member functions).
 *
 * @ingroup space_concepts
 */
template<typename T>
concept TriangleConcept =
    ConstTriangleConcept<T> && requires (T obj, const T& cObj) {
        { obj.point(uint()) } -> std::same_as<typename T::PointType&>;
    };

template<typename T>
concept ConstTriangle2Concept = ConstTriangleConcept<T> && T::DIM == 2;

template<typename T>
concept Triangle2Concept = ConstTriangle2Concept<T> && TriangleConcept<T>;

template<typename T>
concept ConstTriangle3Concept =
    ConstTriangleConcept<T> && T::DIM == 3 && requires (const T& cObj) {
        { cObj.normal() } -> std::same_as<typename T::PointType>;
    };

template<typename T>
concept Triangle3Concept = ConstTriangle3Concept<T> && TriangleConcept<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_TRIANGLE_H
