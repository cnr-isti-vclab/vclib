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

#include "triangle_wrapper.h"

namespace vcl {

template<PointConcept PointT>
TriangleWrapper<PointT>::TriangleWrapper(const PointT& p0, const PointT& p1, const PointT& p2) :
		p0(p0), p1(p1), p2(p2)
{
}

template<PointConcept PointT>
const PointT& TriangleWrapper<PointT>::point(uint i) const
{
	switch(i) {
	case 0:
		return p0;
	case 1:
		return p1;
	default:
		return p2;
	}
}

template<PointConcept PointT>
const PointT& TriangleWrapper<PointT>::point0() const
{
	return p0;
}

template<PointConcept PointT>
const PointT& TriangleWrapper<PointT>::point1() const
{
	return p1;
}

template<PointConcept PointT>
const PointT& TriangleWrapper<PointT>::point2() const
{
	return p2;
}

template<PointConcept PointT>
typename TriangleWrapper<PointT>::ScalarType TriangleWrapper<PointT>::sideLength(uint i) const
{
	switch(i%3) {
	case 0:
		return p0.dist(p1);
	case 1:
		return p1.dist(p2);
	default:
		return p2.dist(p0);
	}
}

template<PointConcept PointT>
typename TriangleWrapper<PointT>::ScalarType TriangleWrapper<PointT>::sideLength0() const
{
	return p0.dist(p1);
}

template<PointConcept PointT>
typename TriangleWrapper<PointT>::ScalarType TriangleWrapper<PointT>::sideLength1() const
{
	return p1.dist(p2);
}

template<PointConcept PointT>
typename TriangleWrapper<PointT>::ScalarType TriangleWrapper<PointT>::sideLength2() const
{
	return p2.dist(p0);
}

template<PointConcept PointT>
PointT TriangleWrapper<PointT>::normal() const requires (PointT::DIM == 3)
{
	return Triangle<PointT>::normal(p0, p1, p2);
}

template<PointConcept PointT>
PointT TriangleWrapper<PointT>::barycenter() const
{
	return Triangle<PointT>::barycenter(p0, p1, p2);
}

template<PointConcept PointT>
PointT TriangleWrapper<PointT>::circumcenter() const
{
	return Triangle<PointT>::circumcenter(p0, p1, p2);
}

template<PointConcept PointT>
typename TriangleWrapper<PointT>::ScalarType TriangleWrapper<PointT>::perimeter() const
{
	return Triangle<PointT>::perimeter(p0, p1, p2);
}

template<PointConcept PointT>
typename TriangleWrapper<PointT>::ScalarType TriangleWrapper<PointT>::area() const
{
	return Triangle<PointT>::area(p0, p1, p2);
}

} // namespace vcl
