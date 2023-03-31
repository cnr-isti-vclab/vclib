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

#ifndef VCL_SPACE_POINT_POINT3_H
#define VCL_SPACE_POINT_POINT3_H

#include "point_t.h"

namespace vcl {

/**
 * @brief The Point3 class represents a 3-dimensional point containing 3 ScalarType values.
 * Specializes the Point class, providing some additional member functions useful for 3D points.
 */
template<typename Scalar>
using Point3 = Point<Scalar, 3>;

using Point3i = Point3<int>;
using Point3f = Point3<float>;
using Point3d = Point3<double>;

// makes sure that Point3 classes satisfy PointConcept
static_assert(Point3Concept<Point3i>, "Point3i does not satisfy the Point3Concept");
static_assert(Point3Concept<Point3f>, "Point3f does not satisfy the Point3Concept");
static_assert(Point3Concept<Point3d>, "Point3d does not satisfy the Point3Concept");

} // namespace vcl

#endif // VCL_SPACE_POINT_POINT3_H
