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

#ifndef VCL_CONCEPT_SPACE_SPHERE_H
#define VCL_CONCEPT_SPACE_SPHERE_H

#include "point.h"

namespace vcl {

template<PointConcept P>
class Box;

/**
 * @brief Concept for types representing spheres in Euclidean space.
 *
 * A type `T` models the `SphereConcept` if it provides the following:
 *
 * - `typename T::PointType`: a type that represents a point in Euclidean space.
 * - `typename T::ScalarType`: a type that represents the scalar used for the coordinates of the
 *    sphere's points.
 * - `o.center()`: a member function that returns a mutable reference to the center of the sphere.
 * - `co.center()`: a member function that returns a constant reference to the center of the sphere.
 * - `o.radius()`: a member function that returns a mutable reference to the radius of the sphere.
 * - `co.radius()`: a member function that returns a constant reference to the radius of the sphere.
 * - `co.diameter()`: a member function that returns the diameter of the sphere.
 * - `co.circumference()`: a member function that returns the circumference of the sphere.
 * - `co.surfaceArea()`: a member function that returns the surface area of the sphere.
 * - `co.volume()`: a member function that returns the volume of the sphere.
 * - `co.isInside(p)`: a member function that returns `true` if the point `p` is inside the sphere,
 *    `false` otherwise.
 * - `co.intersects(b)`: a member function that returns `true` if the sphere intersects the box `b`,
 *    `false` otherwise.
 *
 * The `PointType` type should be a model of the `PointConcept`.
 * The `Box` type should be a model of the `BoxConcept`.
 *
 * @tparam T The type to be tested for conformity to the SphereConcept.
 */
template<typename T>
concept SphereConcept = requires(
	T o,
	const T& co,
	const typename T::PointType& p,
	const Box<typename T::PointType>& b)
{
	typename T::ScalarType;
	typename T::PointType;

	T::PointType::DIM == 3;

	{ o.center() } -> std::same_as<typename T::PointType&>;
	{ co.center() } -> std::same_as<const typename T::PointType&>;
	{ o.radius() } -> std::same_as<typename T::ScalarType&>;
	{ co.radius() } -> std::same_as<const typename T::ScalarType&>;

	{ co.diameter() } -> std::same_as<typename T::ScalarType>;
	{ co.circumference() } -> std::same_as<typename T::ScalarType>;
	{ co.surfaceArea() } -> std::same_as<typename T::ScalarType>;
	{ co.volume() } -> std::same_as<typename T::ScalarType>;

	{ co.isInside(p) } -> std::same_as<bool>;
	{ co.intersects(b) } -> std::same_as<bool>;
};

} // namespace vcl

#endif // VCL_CONCEPT_SPACE_SPHERE_H
