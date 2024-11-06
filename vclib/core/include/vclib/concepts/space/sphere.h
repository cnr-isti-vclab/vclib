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

#ifndef VCL_CONCEPTS_SPACE_SPHERE_H
#define VCL_CONCEPTS_SPACE_SPHERE_H

#include "point.h"

namespace vcl {

/**
 * @brief Concept for types representing spheres in Euclidean space.
 *
 * A type `T` models the `SphereConcept` if it provides the following:
 *
 * - `typename T::PointType`: a type that represents a point in Euclidean space.
 * - `typename T::ScalarType`: a type that represents the scalar used for the
 * coordinates of the sphere's points.
 * - `typename T::BoxType`: a type that represents a box in Euclidean space.
 * - `o.center()`: a member function that returns a mutable reference to the
 * center of the sphere.
 * - `co.center()`: a member function that returns a constant reference to the
 * center of the sphere.
 * - `o.radius()`: a member function that returns a mutable reference to the
 * radius of the sphere.
 * - `co.radius()`: a member function that returns a constant reference to the
 * radius of the sphere.
 * - `co.diameter()`: a member function that returns the diameter of the sphere.
 * - `co.circumference()`: a member function that returns the circumference of
 * the sphere.
 * - `co.surfaceArea()`: a member function that returns the surface area of the
 * sphere.
 * - `co.volume()`: a member function that returns the volume of the sphere.
 * - `co.isInside(p)`: a member function that returns `true` if the point `p` is
 * inside the sphere, `false` otherwise.
 * - `co.intersects(b)`: a member function that returns `true` if the sphere
 * intersects the box `b`, `false` otherwise.
 *
 * The `PointType` type should be a model of the `PointConcept`.
 * The `BoxType` type should be a model of the `BoxConcept`.
 *
 * @tparam T The type to be tested for conformity to the SphereConcept.
 */
template<typename T>
concept SphereConcept = requires (
    T                            obj,
    const T&                     cObj,
    const typename T::PointType& p,
    const typename T::BoxType&   b) {
    typename T::ScalarType;
    typename T::PointType;
    typename T::BoxType;

    T::PointType::DIM == 3;

    { obj.center() } -> std::same_as<typename T::PointType&>;
    { cObj.center() } -> std::same_as<const typename T::PointType&>;
    { obj.radius() } -> std::same_as<typename T::ScalarType&>;
    { cObj.radius() } -> std::same_as<const typename T::ScalarType&>;

    { cObj.diameter() } -> std::same_as<typename T::ScalarType>;
    { cObj.circumference() } -> std::same_as<typename T::ScalarType>;
    { cObj.surfaceArea() } -> std::same_as<typename T::ScalarType>;
    { cObj.volume() } -> std::same_as<typename T::ScalarType>;

    { cObj.isInside(p) } -> std::same_as<bool>;
    { cObj.intersects(b) } -> std::same_as<bool>;
};

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_SPHERE_H
