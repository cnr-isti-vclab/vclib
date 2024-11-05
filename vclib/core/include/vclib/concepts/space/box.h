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
    T                                 obj,
    const T&                          cObj,
    typename T::PointType             p,
    typename T::PointType&            pR,
    const typename T::PointType&      cPR,
    typename T::PointType::ScalarType s) {
    typename T::PointType;
    obj.DIM;

    // Accessors for the minimum and maximum corners of the box.
    { cObj.min() } -> std::same_as<decltype(cPR)>;
    { obj.min() } -> std::same_as<decltype(pR)>;
    { cObj.max() } -> std::same_as<decltype(cPR)>;
    { obj.max() } -> std::same_as<decltype(pR)>;

    // Boolean tests for the nullity and emptiness of the box.
    { cObj.isNull() } -> std::same_as<bool>;
    { cObj.isEmpty() } -> std::same_as<bool>;

    // Boolean tests for whether a point lies inside or outside the box.
    { cObj.isInside(cPR) } -> std::same_as<bool>;
    { cObj.isInsideOpenBox(cPR) } -> std::same_as<bool>;

    // Boolean tests for whether two boxes overlap with each other.
    { cObj.overlap(cObj) } -> std::same_as<bool>;
    { cObj.collide(cObj) } -> std::same_as<bool>;
    { cObj.intersects(cObj) } -> std::same_as<bool>;

    // Accessors for various properties of the box.
    { cObj.diagonal() } -> std::same_as<decltype(s)>;
    { cObj.squaredDiagonal() } -> std::same_as<decltype(s)>;
    { cObj.center() } -> std::same_as<decltype(p)>;
    { cObj.size() } -> std::same_as<decltype(p)>;
    { cObj.volume() } -> std::same_as<decltype(s)>;
    { cObj.dim(uint()) } -> std::same_as<decltype(s)>;
    { cObj.minDim() } -> std::same_as<decltype(s)>;
    { cObj.maxDim() } -> std::same_as<decltype(s)>;
    { cObj.intersection(cObj) } -> std::same_as<T>;

    // Mutators for modifying the state of the box.
    { obj.setNull() } -> std::same_as<void>;
    { obj.add(p) } -> std::same_as<void>;
    { obj.add(p, s) } -> std::same_as<void>;
    { obj.add(cObj) } -> std::same_as<void>;
    { obj.translate(p) } -> std::same_as<void>;

    // Comparison operators.
    { cObj == cObj } -> std::same_as<bool>;
    { cObj != cObj } -> std::same_as<bool>;
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
concept Box2Concept = BoxConcept<T> && T::DIM == 2;

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
concept Box3Concept = BoxConcept<T> && T::DIM == 3;

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_BOX_H
