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

#ifndef VCL_SPACE_SEGMENT_H
#define VCL_SPACE_SEGMENT_H

#include <vclib/concepts/space/segment.h>

#include "point.h"

namespace vcl {

/**
 * @brief A class representing a line segment in n-dimensional space. The class
 * is parameterized by a `PointConcept`, which must provide the `DIM` constant
 * and the `[]` operator for accessing the point coordinates.
 *
 * @tparam PointT The type of the two endpoint points of the segment.
 *
 * @ingroup space
 */
template<PointConcept PointT>
class Segment
{
	PointT point0;
	PointT point1;

public:
	/**
	 * @brief The type of point used to represent the endpoint points of the
	 * segment.
	 */
	using PointType = PointT;

	/**
	 * @brief The scalar type of the endpoint points.
	 */
	using ScalarType = PointT::ScalarType;

	/**
	 * @brief The dimensionality of the segment.
	 */
	static const uint DIM = PointT::DIM;

	/**
	 * @brief Default constructor. Creates a segment with endpoints at the
	 * origin.
	 */
	Segment() {}

	/**
	 * @brief Creates a segment with the given endpoints.
	 *
	 * @param[in] p0: The first endpoint of the segment.
	 * @param[in] p1: The second endpoint of the segment.
	 */
	Segment(const PointT& p0, const PointT& p1) : point0(p0), point1(p1) {}

	/**
	 * @brief Returns the first endpoint of the segment.
	 *
	 * @return A reference to the first endpoint of the segment.
	 */
	PointT& p0() { return point0; }

	/**
	 * @brief Returns the first endpoint of the segment.
	 *
	 * @return A const reference to the first endpoint of the segment.
	 */
	const PointT& p0() const { return point0; }

	/**
	 * @brief Returns the second endpoint of the segment.
	 *
	 * @return A reference to the second endpoint of the segment.
	 */
	PointT& p1() { return point1; }

	/**
	 * @brief Returns the second endpoint of the segment.
	 *
	 * @return A reference to the second endpoint of the segment.
	 */
	const PointT& p1() const { return point1; }

	PointT midPoint() const { return (point0 + point1) / 2.0; }

	PointT direction() const { return point1 - point0; }

	PointT normalizedDirection() const { return (point1 - point0).normalize(); }

	ScalarType length() const { (point0 - point1).norm(); }

	ScalarType squaredLength() const { (point0 - point1).squaredNorm(); }

	void flip() { std::swap(point0, point1); }

	bool operator==(const Segment<PointT>& s) const = default;

	bool operator!=(const Segment<PointT>& s) const = default;

	Segment<PointT> operator+(const Segment<PointT>& s) const
	{
		return Segment<PointT>(point0 + s.point0, point1 + s.point1);
	}

	Segment<PointT> operator-(const Segment<PointT>& s) const
	{
		return Segment<PointT>(point0 - s.point0, point1 - s.point1);
	}

	Segment<PointT> operator*(const ScalarType& s) const
	{
		return Segment<PointT>(point0 * s, point1 * s);
	}

	Segment<PointT> operator/(const ScalarType& s) const
	{
		return Segment<PointT>(point0 / s, point1 / s);
	}

	Segment<PointT>& operator+=(const Segment<PointT>& s) const
	{
		point0 += s.point0;
		point1 += s.point1;
		return *this;
	}

	Segment<PointT>& operator-=(const Segment<PointT>& s) const
	{
		point0 -= s.point0;
		point1 -= s.point1;
		return *this;
	}

	Segment<PointT>& operator*=(const ScalarType& s) const
	{
		point0 *= s;
		point1 *= s;
		return *this;
	}

	Segment<PointT>& operator/=(const ScalarType& s) const
	{
		point0 /= s;
		point1 /= s;
		return *this;
	}
};

/* Specialization Aliases */

template < typename S>
using Segment2 = Segment<Point2<S>>;

using Segment2i = Segment<Point2i>;
using Segment2f = Segment<Point2f>;
using Segment2d = Segment<Point2d>;

template < typename S>
using Segment3 = Segment<Point3<S>>;

using Segment3i = Segment<Point3i>;
using Segment3f = Segment<Point3f>;
using Segment3d = Segment<Point3d>;

} // namespace vcl

#endif // VCL_SPACE_SEGMENT_H
