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

#ifndef VCL_CONCEPTS_SPACE_BOX_H
#define VCL_CONCEPTS_SPACE_BOX_H

#include "point.h"

#include <vclib/types.h>

namespace vcl {

/**
 * @brief A concept representing a N-Dimensional Box.
 *
 * This concept defines a set of requirements that must be satisfied by any type
 * that wishes to be considered a Box. A Box is a geometric object that is
 * defined by two points in space, typically referred to as the minimum and
 * maximum corners of the box.
 *
 * @tparam T: The type to be tested for conformity to the BoxConcept.
 */
template<typename T>
concept BoxConcept = requires (
    T&&                                          obj,
    typename RemoveRef<T>::PointType             p,
    typename RemoveRef<T>::PointType&            pR,
    typename RemoveRef<T>::PointType::ScalarType s) {
    typename RemoveRef<T>::PointType;
    obj.DIM;

    // Constructors
    RemoveRef<T>();
    RemoveRef<T>(p);
    RemoveRef<T>(p, p);

    // Accessors for the minimum and maximum corners of the box.
    { obj.min() } -> PointConcept;
    { obj.max() } -> PointConcept;

    // Boolean tests for the nullity and emptiness of the box.
    { obj.isNull() } -> std::same_as<bool>;
    { obj.isEmpty() } -> std::same_as<bool>;

    // Boolean tests for whether a point lies inside or outside the box.
    { obj.isInside(p) } -> std::same_as<bool>;
    { obj.isInsideOpenBox(p) } -> std::same_as<bool>;

    // Boolean tests for whether two boxes overlap with each other.
    { obj.overlap(obj) } -> std::same_as<bool>;
    { obj.collide(obj) } -> std::same_as<bool>;
    { obj.intersects(obj) } -> std::same_as<bool>;

    // Accessors for various properties of the box.
    { obj.diagonal() } -> std::same_as<decltype(s)>;
    { obj.squaredDiagonal() } -> std::same_as<decltype(s)>;
    { obj.center() } -> PointConcept;
    { obj.size() } -> PointConcept;
    { obj.volume() } -> std::convertible_to<decltype(s)>;
    { obj.dim(uint()) } -> std::convertible_to<decltype(s)>;
    { obj.minDim() } -> std::convertible_to<decltype(s)>;
    { obj.maxDim() } -> std::convertible_to<decltype(s)>;
    { obj.intersection(obj) } -> std::convertible_to<RemoveRef<T>>;

    // Comparison operators.
    { obj == obj } -> std::same_as<bool>;
    { obj != obj } -> std::same_as<bool>;

    // non const requirements
    requires vcl::IsConst<T> || requires {
        { obj.min() } -> std::same_as<decltype(pR)>;
        { obj.max() } -> std::same_as<decltype(pR)>;

        // Mutators for modifying the state of the box.
        { obj.setNull() } -> std::same_as<void>;
        { obj.add(p) } -> std::same_as<void>;
        { obj.add(p, s) } -> std::same_as<void>;
        { obj.add(obj) } -> std::same_as<void>;
        { obj.translate(p) } -> std::same_as<void>;
    };
};

/**
 * @brief A concept that requires a type to satisfy the BoxConcept and have a
 * dimension of 2.
 *
 * The Box2Concept concept requires that the given type satisfies the BoxConcept
 * concept and that its dimension is equal to 2. This can be used to constrain
 * template parameters of functions or classes that require a 2D box.
 *
 * @tparam T: The type to be tested for conformity to the Box2Concept.
 */
template<typename T>
concept Box2Concept = BoxConcept<T> && RemoveRef<T>::DIM == 2;

/**
 * @brief A concept that requires a type to satisfy the BoxConcept and have a
 * dimension of 3.
 *
 * The Box3Concept concept requires that the given type satisfies the BoxConcept
 * concept and that its dimension is equal to 3. This can be used to constrain
 * template parameters of functions or classes that require a 3D box.
 *
 * @tparam T: The type to be tested for conformity to the Box2Concept.
 */
template<typename T>
concept Box3Concept = BoxConcept<T> && RemoveRef<T>::DIM == 3;

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_BOX_H
