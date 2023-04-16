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

#ifndef VCL_CONCEPTS_SPACE_PLANE_H
#define VCL_CONCEPTS_SPACE_PLANE_H

#include <vclib/types.h>

namespace vcl {

/**
 * @brief A C++ concept that requires a type to represent a plane in 3D space.
 * @tparam T: The type to check for compliance with the PlaneConcept.
 *
 * This concept requires that the input type T has the following public member functions:
 *  - typename T::ScalarType: A type alias representing the scalar type used in the plane equations.
 *  - typename T::PointType: A type alias representing the Point type used in the plane equations.
 *  - const typename T::PointType& T::direction(): A const reference to the direction vector of the
 *    plane.
 *  - typename T::ScalarType T::offset(): The offset of the plane from the origin.
 *  - typename T::PointType T::projectPoint(const typename T::PointType& p): Projects a point onto
 *    the plane.
 *  - typename T::PointType T::mirrorPoint(const typename T::PointType& p): Mirrors a point across
 *    the plane.
 *  - bool T::operator==(const T& o): Equality comparison operator for planes.
 *  - bool T::operator!=(const T& o): Inequality comparison operator for planes.
 *
 * All of the member functions must satisfy the requirements listed in the Doxygen comments above.
 */
template<typename T>
concept PlaneConcept = requires(
	T o,
	const T& co,
	const typename T::PointType& p)
{
	typename T::ScalarType;
	typename T::PointType;

	{ co.direction() } -> std::same_as<const typename T::PointType&>;
	{ co.offset() } -> std::same_as<typename T::ScalarType>;

	{ co.projectPoint(p) } -> std::same_as<typename T::PointType>;
	{ co.mirrorPoint(p) } -> std::same_as<typename T::PointType>;

	{ co == co } -> std::same_as<bool>;
	{ co != co } -> std::same_as<bool>;
};

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_PLANE_H
