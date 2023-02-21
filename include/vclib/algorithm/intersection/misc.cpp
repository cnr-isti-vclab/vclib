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
	PointType c = (box.max + box.min) * 0.5f; // Compute AABB center
	PointType e = box.max - c; // Compute positive extents

	PointType n = p.direction();
	// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
	ScalarType r = e[0]*std::abs(n[0]) + e[1]*std::abs(n[1]) + e[2]*std::abs(n[2]);

	// Compute distance of box center from plane
	ScalarType s = n.dot(c) - p.offset();

	// Intersection occurs when distance s falls within [-r,+r] interval
	return std::abs(s) <= r;
}

} // namespace vcl
