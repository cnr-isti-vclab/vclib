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

#ifndef VCL_SPACE_TRIANGLE_H
#define VCL_SPACE_TRIANGLE_H

#include <vclib/concept/space/triangle.h>
#include <vclib/space/point.h>

namespace vcl {

template<PointConcept PointT>
class Triangle
{
public:
	using ScalarType = typename PointT::ScalarType;
	using PointType = PointT;

	static const int DIM = PointT::DIM;

	Triangle();
	Triangle(const PointT& p0, const PointT& p1, const PointT& p2);

	constexpr uint size() const { return 3; }

	PointT& point(uint i);
	const PointT& point(uint i) const;
	PointT& point0();
	const PointT& point0() const;
	PointT& point1();
	const PointT& point1() const;
	PointT& point2();
	const PointT& point2() const;

	ScalarType sideLength(uint i) const;
	ScalarType sideLength0() const;
	ScalarType sideLength1() const;
	ScalarType sideLength2() const;

	PointT normal() const requires (PointT::DIM == 3);

	PointT barycenter() const;
	PointT weightedBarycenter(ScalarType w0, ScalarType w1, ScalarType w2) const;
	PointT weightedBarycenter(const Point3<ScalarType>& w) const;
	PointT barycentricCoordinatePoint(ScalarType b0, ScalarType b1, ScalarType b2) const;
	PointT barycentricCoordinatePoint(const Point3<ScalarType>& b) const;

	PointT circumcenter() const;

	ScalarType perimeter() const;
	ScalarType area() const;

	ScalarType quality() const;
	ScalarType qualityRadii() const;
	ScalarType qualityMeanRatio() const;

	// static member functions

	static PointT normal(const PointT& p0, const PointT& p1, const PointT& p2)
		requires (PointT::DIM == 3);

	static PointT barycenter(const PointT& p0, const PointT& p1, const PointT& p2);
	static PointT weightedBarycenter(
		const PointT& p0,
		const PointT& p1,
		const PointT& p2,
		ScalarType    w0,
		ScalarType    w1,
		ScalarType    w2);
	static PointT barycentricCoordinatePoint(
		const PointT& p0,
		const PointT& p1,
		const PointT& p2,
		ScalarType    b0,
		ScalarType    b1,
		ScalarType    b2);
	static PointT circumcenter(const PointT& p0, const PointT& p1, const PointT& p2);
	static ScalarType perimeter(const PointT& p0, const PointT& p1, const PointT& p2);
	static ScalarType area(const PointT& p0, const PointT& p1, const PointT& p2);
	static ScalarType quality(const PointT& p0, const PointT& p1, const PointT& p2);
	static ScalarType qualityRadii(const PointT& p0, const PointT& p1, const PointT& p2);
	static ScalarType qualityMeanRatio(const PointT& p0, const PointT& p1, const PointT& p2);

private:
	std::array<PointT, 3> p;
};

template<typename Scalar>
using Triangle2 = Triangle<Point2<Scalar>>;

using Triangle2f = Triangle<Point2f>;
using Triangle2d = Triangle<Point2d>;

template<typename Scalar>
using Triangle3 = Triangle<Point3<Scalar>>;

using Triangle3f = Triangle<Point3f>;
using Triangle3d = Triangle<Point3d>;

static_assert(Triangle2Concept<Triangle2f>, "Triangle2f does not satisfy the Triangle2Concept");
static_assert(Triangle2Concept<Triangle2d>, "Triangle2d does not satisfy the Triangle2Concept");

static_assert(Triangle3Concept<Triangle3f>, "Triangle3f does not satisfy the Triangle3Concept");
static_assert(Triangle3Concept<Triangle3d>, "Triangle3d does not satisfy the Triangle3Concept");

} // namespace vcl

#include "triangle.cpp"

#endif // VCL_SPACE_TRIANGLE_H