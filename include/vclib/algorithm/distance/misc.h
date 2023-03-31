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

#ifndef VCL_ALGORITHM_DISTANCE_MISC_H
#define VCL_ALGORITHM_DISTANCE_MISC_H

#include <vclib/space/plane.h>
#include <vclib/space/segment.h>

namespace vcl {

template<Point3Concept PointType, PlaneConcept PlaneType>
auto pointPlaneDistance(const PointType& p, const PlaneType& plane, bool signedDist = false);

template<PointConcept PointType, SegmentConcept SegmentType>
auto pointSegmentDistance(const PointType& p, const SegmentType& s, PointType& closest)
	requires (PointType::DIM == SegmentType::DIM);

template<PointConcept PointType, SegmentConcept SegmentType>
auto pointSegmentDistance(const PointType& p, const SegmentType& s)
	requires (PointType::DIM == SegmentType::DIM);

} // namespace vcl

#include "misc.cpp"

#endif // VCL_ALGORITHM_DISTANCE_MISC_H
