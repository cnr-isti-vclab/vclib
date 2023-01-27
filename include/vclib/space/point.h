/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#ifndef VCL_SPACE_POINT_H
#define VCL_SPACE_POINT_H

#include "point/point2.h"
#include "point/point3.h"
#include "point/point4.h"

namespace vcl {

template<PointConcept PointType>
PointType min(const PointType& p1, const PointType& p2);

template<PointConcept PointType>
PointType max(const PointType& p1, const PointType& p2);

template<PointConcept PointType>
auto outerProduct(const PointType& p1, const PointType& p2);

template<typename Scalar>
void orthoBase(const Point3<Scalar>& n, Point3<Scalar>& u, Point3<Scalar>& v);

} // namespace vcl

#include "point.cpp"

#endif // VCL_SPACE_POINT_H
