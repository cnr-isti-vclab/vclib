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

#ifndef VCL_CONCEPTS_SPACE_PLANE_H
#define VCL_CONCEPTS_SPACE_PLANE_H

#include <vclib/types.h>

#include "point.h"

namespace vcl {

/**
 * @brief A C++ concept that requires a type to represent a plane in 3D space.
 * @tparam T: The type to check for compliance with the PlaneConcept.
 *
 * This concept requires that the input type T has the following public member
 * functions:
 *  - typename T::ScalarType: A type alias representing the scalar type used in
 * the plane equations.
 *  - typename T::PointType: A type alias representing the Point type used in
 * the plane equations.
 *  - const typename T::PointType& T::direction(): A const reference to the
 * direction vector of the plane.
 *  - typename T::ScalarType T::offset(): The offset of the plane from the
 * origin.
 *  - typename T::PointType T::projectPoint(const typename T::PointType& p):
 * Projects a point onto the plane.
 *  - typename T::PointType T::mirrorPoint(const typename T::PointType& p):
 * Mirrors a point across the plane.
 *  - bool T::operator==(const T& o): Equality comparison operator for planes.
 *  - bool T::operator!=(const T& o): Inequality comparison operator for planes.
 *
 * All of the member functions must satisfy the requirements listed in the
 * Doxygen comments above.
 */
template<typename T>
concept PlaneConcept = requires (
    T&&                               obj,
    typename RemoveRef<T>::PointType  p,
    typename RemoveRef<T>::ScalarType s) {

    // inner types
    typename RemoveRef<T>::ScalarType;
    typename RemoveRef<T>::PointType;

    // constructors
    RemoveRef<T>();
    RemoveRef<T>(p, s);
    RemoveRef<T>(p, p);
    RemoveRef<T>(p, p, p);

    { obj.direction() } -> Point3Concept;
    { obj.offset() } -> std::same_as<decltype(s)>;

    { obj.projectPoint(p) } -> Point3Concept;
    { obj.mirrorPoint(p) } -> Point3Concept;

    { obj == obj } -> std::same_as<bool>;
    { obj != obj } -> std::same_as<bool>;
};

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_PLANE_H
