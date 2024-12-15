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

#ifndef VCL_CONCEPTS_SPACE_SEGMENT_H
#define VCL_CONCEPTS_SPACE_SEGMENT_H

#include "point.h"

namespace vcl {

/**
 * @brief Concept for types representing line segments in Euclidean space.
 *
 * @tparam T: The type to be tested for conformity to the SegmentConcept.
 */
template<typename T>
concept SegmentConcept =
    requires (
        T&&                               obj,
        typename RemoveRef<T>::PointType  p,
        typename RemoveRef<T>::ScalarType s) {
        typename RemoveRef<T>::PointType;
        typename RemoveRef<T>::ScalarType;

        obj.DIM;

        RemoveRef<T>();
        RemoveRef<T>(p, p);

        { obj.p0() } -> PointConcept;
        { obj.p1() } -> PointConcept;

        { obj.midPoint() } -> PointConcept;
        { obj.direction() } -> PointConcept;
        { obj.normalizedDirection() } -> PointConcept;
        { obj.length() } -> std::same_as<decltype(s)>;
        { obj.squaredLength() } -> std::same_as<decltype(s)>;

        { obj == obj } -> std::same_as<bool>;
        { obj != obj } -> std::same_as<bool>;

        { obj + obj } -> std::convertible_to<RemoveRef<T>>;
        { obj - obj } -> std::convertible_to<RemoveRef<T>>;
        { obj* s } -> std::convertible_to<RemoveRef<T>>;
        { obj / s } -> std::convertible_to<RemoveRef<T>>;

        // non const requirements
        requires vcl::IsConst<T> || requires {
            { obj.flip() } -> std::same_as<void>;

            { obj = obj } -> std::same_as<T&>;

            { obj += obj } -> std::same_as<T&>;
            { obj -= obj } -> std::same_as<T&>;
            { obj *= s } -> std::same_as<T&>;
            { obj /= s } -> std::same_as<T&>;
        };
    };

/**
 * @brief A concept to check whether a type meets the requirements of a 2D
 * segment.
 *
 * This concept checks that the input type `T` satisfies the requirements of a
 * 2D segment, which means that it must have the following properties:
 *
 * - It satisfies the requirements of the SegmentConcept.
 * - It has a static constant member `DIM` equal to 2.
 *
 * @tparam T: The type to be tested for conformity to the Segment2Concept.
 */
template<typename T>
concept Segment2Concept = SegmentConcept<T> && RemoveRef<T>::DIM == 2;

/**
 * @brief A concept to check whether a type meets the requirements of a 3D
 * segment.
 *
 * This concept checks that the input type `T` satisfies the requirements of a
 * 3D segment, which means that it must have the following properties:
 *
 * - It satisfies the requirements of the SegmentConcept.
 * - It has a static constant member `DIM` equal to 3.
 *
 * @tparam T: The type to be tested for conformity to the Segment3Concept.
 */
template<typename T>
concept Segment3Concept = SegmentConcept<T> && RemoveRef<T>::DIM == 3;

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_SEGMENT_H
