/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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
concept BoxConcept =
    requires (T o, const T& co, const typename T::PointType& p) {
        // clang-format off
        typename T::PointType;
        o.DIM;

        // Accessors for the minimum and maximum corners of the box.
        { co.min() } -> std::same_as<const typename T::PointType&>;
        { o.min() } -> std::same_as<typename T::PointType&>;
        { co.max() } -> std::same_as<const typename T::PointType&>;
        { o.max() } -> std::same_as<typename T::PointType&>;

        // Boolean tests for the nullity and emptiness of the box.
        { co.isNull() } -> std::same_as<bool>;
        { co.isEmpty() } -> std::same_as<bool>;

        // Boolean tests for whether a point lies inside or outside the box.
        { co.isInside(p) }  -> std::same_as<bool>;
        { co.isInsideOpenBox(p) }  -> std::same_as<bool>;

        // Boolean tests for whether two boxes overlap with each other.
        { co.overlap(co) }  -> std::same_as<bool>;
        { co.collide(co) }  -> std::same_as<bool>;
        { co.intersects(co) }  -> std::same_as<bool>;

        // Accessors for various properties of the box.
        { co.diagonal() } -> std::same_as<typename T::PointType::ScalarType>;
        { co.squaredDiagonal() } -> std::same_as<typename T::PointType::ScalarType>;
        { co.center() } -> std::same_as<typename T::PointType>;
        { co.size() } -> std::same_as<typename T::PointType>;
        { co.volume() } -> std::same_as<typename T::PointType::ScalarType>;
        { co.dim(uint()) } -> std::same_as<typename T::PointType::ScalarType>;
        { co.minDim() } -> std::same_as<typename T::PointType::ScalarType>;
        { co.maxDim() } -> std::same_as<typename T::PointType::ScalarType>;
        { co.intersection(co) } -> std::same_as<T>;

        // Mutators for modifying the state of the box.
        { o.setNull() } -> std::same_as<void>;
        { o.add(typename T::PointType()) } -> std::same_as<void>;
        { o.add(typename T::PointType(), double()) } -> std::same_as<void>;
        { o.add(co) } -> std::same_as<void>;
        { o.translate(typename T::PointType()) } -> std::same_as<void>;

        // Comparison operators.
        { co == co } -> std::same_as<bool>;
        { co != co } -> std::same_as<bool>;
        // clang-format on
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
