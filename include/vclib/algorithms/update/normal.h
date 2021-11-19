/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_ALGORITHMS_NORMAL_H
#define VCL_ALGORITHMS_NORMAL_H

#include <vclib/space/point.h>

namespace vcl {

template <typename PointType>
PointType triangleNormal(const PointType& p1, const PointType& p2, const PointType& p3);

template<typename Triangle, typename NormalType = Point3f>
NormalType triangleNormal(const Triangle& t);

template<typename Polygon, typename NormalType = Point3f>
typename Polygon::CoordType polygonNormal(const Polygon& p);

template<typename MeshType>
void normalizePerFaceNormals(MeshType& m);

template<typename MeshType>
void updatePerFaceNormals(MeshType& m, bool normalize = true);

} // namespace vcl

#include "normal.cpp"

#endif // VCL_ALGORITHMS_NORMAL_H
