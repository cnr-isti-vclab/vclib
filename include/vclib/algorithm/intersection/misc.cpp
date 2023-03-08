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
 * https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
 *
 * @param p
 * @param b
 * @return
 */
template<typename PlaneType, typename BoxType>
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
 * @brief Computes the intersectin between a plane and a segment. Returns `true` if the
 * intersection exists, false otherwise.
 *
 * @param[in] p: the plane on which conpute the intersection.
 * @param[in] s: the segment on which compute the intersection.
 * @param[out] intersection: the resulting intersection between the plane and the segment.
 * @return True if the intersection exists, false otherwise.
 */
template<typename PlaneType, Segment3Concept SegmentType>
bool planeSegmentIntersect(
	const PlaneType&                 p,
	const SegmentType&               s,
	typename SegmentType::PointType& intersection)
{
	using ScalarType = typename SegmentType::ScalarType;

	ScalarType p1_proj = s.p1() * p.direction() - p.offset();
	ScalarType p0_proj = s.p0() * p.direction() - p.offset();
	if ( (p1_proj>0)-(p0_proj<0))
		return false;

	if(p0_proj == p1_proj) return false;

	// check that we perform the computation in a way that is independent with v0 v1 swaps
	if(p0_proj < p1_proj)
		intersection =  s.first + (s.second - s.first) * std::abs(p0_proj/(p1_proj-p0_proj));
	if(p0_proj > p1_proj)
		intersection =  s.second + (s.first - s.second) * std::abs(p1_proj/(p0_proj-p1_proj));

	return true;
}

/**
 * @brief Computes the intersectin between a plane and a segment. Returns `true` if the
 * intersection exists, false otherwise.
 *
 * @param[in] p: the plane on which conpute the intersection.
 * @param[in] s: the segment on which compute the intersection.
 * @return True if the intersection exists, false otherwise.
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
