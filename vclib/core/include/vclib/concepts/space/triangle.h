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

#include "point.h"

namespace vcl {

/**
 * @brief Concept for types representing triangles in Euclidean space.
 *
 * @ingroup space_concepts
 */
template<typename T>
concept TriangleConcept = requires (T&& obj) {
    typename T::ScalarType;
    typename T::PointType;

    obj.DIM;
    obj.size() == 3;

    { obj.point(uint()) } -> PointConcept;
    { obj.sideLength(uint()) } -> std::same_as<typename T::ScalarType>;
    { obj.barycenter() } -> std::same_as<typename T::PointType>;
    { obj.perimeter() } -> std::same_as<typename T::ScalarType>;
    { obj.area() } -> std::same_as<typename T::ScalarType>;
};

template<typename T>
concept Triangle2Concept = TriangleConcept<T> && T::DIM == 2;

template<typename T>
concept Triangle3Concept =
    TriangleConcept<T> && T::DIM == 3 && requires (T&& obj) {
        { obj.normal() } -> Point3Concept;
    };

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_TRIANGLE_H
