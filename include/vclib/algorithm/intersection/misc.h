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

#ifndef VCL_ALGORITHM_INTERSECTION_MISC_H
#define VCL_ALGORITHM_INTERSECTION_MISC_H

#include <vclib/space/box.h>
#include <vclib/space/plane.h>
#include <vclib/space/segment.h>
#include <vclib/space/sphere.h>
#include <vclib/space/triangle.h>

namespace vcl {

template<PlaneConcept PlaneType, Box3Concept BoxType>
bool planeBoxIntersect(const PlaneType& p, const BoxType& box);

template<PlaneConcept PlaneType, Segment3Concept SegmentType>
bool planeSegmentIntersect(
	const PlaneType&                 p,
	const SegmentType&               s,
	typename SegmentType::PointType& intersection);

template<PlaneConcept PlaneType, Segment3Concept SegmentType>
bool planeSegmentIntersect(const PlaneType& p, const SegmentType& s);

template<SphereConcept SphereType, Box3Concept BoxType>
bool sphereBoxIntersect(const SphereType& s, const BoxType& box);

template<Triangle2Concept TriangleType, Point2Concept PointType>
bool trianglePointIntersect(const TriangleType& t, const PointType& p);

template<Triangle3Concept TriangleType, Point3Concept PointType>
bool trianglePointIntersect(const TriangleType& t, const PointType& p);

template<ConstTriangle3Concept TriangleType, Box3Concept BoxType>
bool triangleBoxIntersect(const TriangleType& t, const BoxType& box);

template<
	ConstTriangle3Concept TriangleType,
	SphereConcept         SphereType,
	Point3Concept         PointType,
	typename ScalarType>
bool triangleSphereItersect(
	const TriangleType&                t,
	const SphereType&                  sphere,
	PointType&                         witness,
	std::pair<ScalarType, ScalarType>& res);

template<ConstTriangle3Concept TriangleType, SphereConcept SphereType>
bool triangleSphereItersect(
	const TriangleType&   t,
	const SphereType& sphere);

} // namespace vcl

#include "misc.cpp"

#endif // VCL_ALGORITHM_INTERSECTION_MISC_H
