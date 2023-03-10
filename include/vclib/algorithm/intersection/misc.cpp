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

#include "misc.h"

namespace vcl {

/**
 * @brief Checks if a plane intersects with a box.
 *
 * Uses the algorithm from
 * https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
 *
 * @tparam PlaneType: The type of plane used in the intersection check
 * @tparam BoxType: The type of box used in the intersection check, must satisfy the Box3Concept
 *
 * @param[in] p: The plane to check intersection with
 * @param[in] box: The box to check intersection with
 * @return True if the plane intersects with the box, false otherwise
 */
template<typename PlaneType, Box3Concept BoxType>
bool planeBoxIntersect(const PlaneType& p, const BoxType& box)
{
	using PointType = typename BoxType::PointType;
	using ScalarType = typename PointType::ScalarType;

	// Convert AABB to center-extents representation
	PointType c = (box.max() + box.min()) * 0.5f; // Compute AABB center
	PointType e = box.max() - c; // Compute positive extents

	PointType n = p.direction();
	// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
	ScalarType r = e[0]*std::abs(n[0]) + e[1]*std::abs(n[1]) + e[2]*std::abs(n[2]);

	// Compute distance of box center from plane
	ScalarType s = n.dot(c) - p.offset();

	// Intersection occurs when distance s falls within [-r,+r] interval
	return std::abs(s) <= r;
}

/**
 * @brief This function computes the intersection between a plane and a 3D segment. If the
 * intersection exists, it returns true and stores the intersection point in the output parameter
 * intersection, otherwise, it returns false.
 *
 * @param[in] p: the plane to compute the intersection with.
 * @param[in] s: the 3D segment to compute the intersection with.
 * @param[out] intersection: A reference to the point that stores the resulting intersection between
 * the plane and the segment. This parameter is only written to if the function returns true.
 * @return A boolean value indicating whether an intersection was found or not.
 */
template<typename PlaneType, Segment3Concept SegmentType>
bool planeSegmentIntersect(
	const PlaneType&                 p,
	const SegmentType&               s,
	typename SegmentType::PointType& intersection)
{
	using ScalarType = typename SegmentType::ScalarType;

	// Compute the projection of the segment endpoints onto the plane.
	ScalarType p1_proj = s.p1() * p.direction() - p.offset();
	ScalarType p0_proj = s.p0() * p.direction() - p.offset();

	// If both endpoints are on the same side of the plane, there is no intersection.
	if ( (p1_proj>0)-(p0_proj<0))
		return false;

	// If both endpoints have the same projection onto the plane, there is no intersection.
	if(p0_proj == p1_proj)
		return false;

	// check that we perform the computation in a way that is independent with v0 v1 swaps
	if(p0_proj < p1_proj)
		intersection = s.p0() + (s.p1() - s.p0()) * std::abs(p0_proj/(p1_proj-p0_proj));
	if(p0_proj > p1_proj)
		intersection = s.p1() + (s.p0() - s.p1()) * std::abs(p1_proj/(p0_proj-p1_proj));

	return true;
}

/**
 * @brief This function computes the intersection between a plane and a 3D segment. If the
 * intersection exists, it returns true, otherwise, it returns false.
 *
 * @param[in] p: the plane to compute the intersection with.
 * @param[in] s: the 3D segment to compute the intersection with.
 * @return A boolean value indicating whether an intersection was found or not.
 */
template<typename PlaneType, Segment3Concept SegmentType>
bool planeSegmentIntersect(
	const PlaneType&                 p,
	const SegmentType&               s)
{
	typename SegmentType::PointType intersection;

	return planeSegmentIntersect(p, s, intersection);
}

} // namespace vcl
