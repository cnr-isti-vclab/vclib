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

#ifndef VCL_CONCEPTS_SPACE_SEGMENT_H
#define VCL_CONCEPTS_SPACE_SEGMENT_H

#include <vclib/types.h>

namespace vcl {

/**
 * @brief Concept for types representing line segments in Euclidean space.
 *
 * A type `T` models the `SegmentConcept` if it provides the following:
 *
 * - `typename T::PointType`: a type that represents a point in Euclidean space.
 * - `typename T::ScalarType`: a type that represents the scalar used for the
 * coordinates of the segment's points.
 * - `o.DIM`: a static data member or constant expression that specifies the
 * dimension of the segment.
 * - `o.p0()`: a member function that returns a mutable reference to the first
 * endpoint of the segment.
 * - `co.p0()`: a member function that returns a constant reference to the first
 * endpoint of the segment.
 * - `o.p1()`: a member function that returns a mutable reference to the second
 * endpoint of the segment.
 * - `co.p1()`: a member function that returns a constant reference to the
 * second endpoint of the segment.
 *
 * The `PointType` type should be a model of the `PointConcept`.
 *
 * @tparam T: The type to be tested for conformity to the SegmentConcept.
 */
template<typename T>
concept SegmentConcept = requires (T o, const T& co) {
    // clang-format off
    typename T::PointType;
    typename T::ScalarType;
    o.DIM;
    { o.p0() } -> std::same_as<typename T::PointType&>;
    { co.p0() } -> std::same_as<const typename T::PointType&>;
    { o.p1() } -> std::same_as<typename T::PointType&>;
    { co.p1() } -> std::same_as<const typename T::PointType&>;
    // clang-format on
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
concept Segment2Concept = SegmentConcept<T> && T::DIM == 2;

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
concept Segment3Concept = SegmentConcept<T> && T::DIM == 3;

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_SEGMENT_H
