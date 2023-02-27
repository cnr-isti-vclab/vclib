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

#ifndef VCL_MESH_MESH_ALGORITHMS_H
#define VCL_MESH_MESH_ALGORITHMS_H

#include <vclib/space/point.h>

namespace vcl::mesh {

template<PointConcept PointType>
PointType polygonNormal(const std::vector<PointType>& p);

template<typename Scalar>
std::vector<uint> earCut(const std::vector<Point2<Scalar>>& polygon);

template<typename Scalar>
std::vector<uint> earCut(const std::vector<Point3<Scalar>>& polygon);

template<typename Face>
std::vector<uint> earCut(const Face& polygon);

} // namespace vcl::mesh

#include "mesh_algorithms.cpp"

#endif // VCL_MESH_MESH_ALGORITHMS_H
