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

#ifndef VCL_SPACE_POINT_POINT4_H
#define VCL_SPACE_POINT_POINT4_H

#include "point_t.h"

namespace vcl {

/**
 * @brief The Point4 class represents a 4-dimensional point containing 4 ScalarType values.
 * Specializes the Point class, providing some additional member functions useful for 4D points.
 */
template<typename Scalar>
using Point4 = Point<Scalar, 4>;

using Point4i = Point4<int>;
using Point4f = Point4<float>;
using Point4d = Point4<double>;

// makes sure that Point4 classes satisfy PointConcept
static_assert(Point4Concept<Point4i>, "Point4i does not satisfy the Point4Concept");
static_assert(Point4Concept<Point4f>, "Point4f does not satisfy the Point4Concept");
static_assert(Point4Concept<Point4d>, "Point4d does not satisfy the Point4Concept");

} // namespace vcl

#endif // VCL_SPACE_POINT_POINT4_H
