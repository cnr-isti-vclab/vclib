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

#ifndef VCL_SPACE_TRIANGLE_WRAPPER_H
#define VCL_SPACE_TRIANGLE_WRAPPER_H

#include "triangle.h"

namespace vcl {

template<PointConcept PointT>
class TriangleWrapper
{
public:
	using ScalarType = typename PointT::ScalarType;
	using PointType = PointT;

	static const uint DIM = PointT::DIM;

	TriangleWrapper(const PointT& p0, const PointT& p1, const PointT& p2);

	constexpr uint size() const { return 3; }

	const PointT& point(uint i) const;
	const PointT& point0() const;
	const PointT& point1() const;
	const PointT& point2() const;

	ScalarType sideLength(uint i) const;
	ScalarType sideLength0() const;
	ScalarType sideLength1() const;
	ScalarType sideLength2() const;

	PointT normal() const requires (PointT::DIM == 3);

	PointT barycenter() const;

	PointT circumcenter() const;

	ScalarType perimeter() const;
	ScalarType area() const;

private:
	const PointT& p0;
	const PointT& p1;
	const PointT& p2;
};

template<typename Scalar>
using TriangleWrapper2 = TriangleWrapper<Point2<Scalar>>;

using TriangleWrapper2f = TriangleWrapper<Point2f>;
using TriangleWrapper2d = TriangleWrapper<Point2d>;

template<typename Scalar>
using TriangleWrapper3 = TriangleWrapper<Point3<Scalar>>;

using TriangleWrapper3f = TriangleWrapper<Point3f>;
using TriangleWrapper3d = TriangleWrapper<Point3d>;

} // namespace vcl

#include "triangle_wrapper.cpp"

#endif // VCL_SPACE_TRIANGLE_WRAPPER_H
