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

#ifndef VCL_CONCEPT_SPACE_PLANE_H
#define VCL_CONCEPT_SPACE_PLANE_H

#include <vclib/misc/types.h>

namespace vcl {

template<typename T, int N>
class Point;

/**
 * @brief A C++ concept that requires a type to represent a plane in 3D space.
 * @tparam T: The type to check for compliance with the PlaneConcept.
 *
 * This concept requires that the input type T has the following public member functions:
 *  - typename T::ScalarType: A type alias representing the scalar type used in the plane equations.
 *  - const Point<typename T::ScalarType, 3>& T::direction(): A const reference to the direction
 *    vector of the plane.
 *  - typename T::ScalarType T::offset(): The offset of the plane from the origin.
 *  - Point<typename T::ScalarType, 3> T::projectPoint(const Point<typename T::ScalarType, 3>& p):
 *    Projects a point onto the plane.
 *  - Point<typename T::ScalarType, 3> T::mirrorPoint(const Point<typename T::ScalarType, 3>& p):
 *    Mirrors a point across the plane.
 *  - bool T::operator==(const T& o): Equality comparison operator for planes.
 *  - bool T::operator!=(const T& o): Inequality comparison operator for planes.
 *
 * All of the member functions must satisfy the requirements listed in the Doxygen comments above.
 */
template<typename T>
concept PlaneConcept = requires(
	T o,
	const T& co,
	const Point<typename T::ScalarType, 3>& p)
{
	typename T::ScalarType;

	{ co.direction() } -> std::same_as<const Point<typename T::ScalarType, 3>&>;
	{ co.offset() } -> std::same_as<typename T::ScalarType>;

	{ co.projectPoint(p) } -> std::same_as<Point<typename T::ScalarType, 3>>;
	{ co.mirrorPoint(p) } -> std::same_as<Point<typename T::ScalarType, 3>>;

	{ co == co } -> std::same_as<bool>;
	{ co != co } -> std::same_as<bool>;
};

} // namespace vcl

#endif // VCL_CONCEPT_SPACE_PLANE_H
