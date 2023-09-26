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

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

/**
 * @brief A class representing a line segment in n-dimensional space. The class is parameterized
 * by a `PointConcept`, which must provide the `DIM` constant and the `[]` operator for
 * accessing the point coordinates.
 *
 * @tparam PointT The type of the two endpoint points of the segment.
 *
 * @ingroup space
 */
template<PointConcept PointT>
class Segment
{
public:
	/**
	 * @brief The type of point used to represent the endpoint points of the segment.
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

	Segment();
	Segment(const PointT& p0, const PointT& p1);

	PointT& p0();
	const PointT& p0() const;

	PointT& p1();
	const PointT& p1() const;

	PointT midPoint() const;
	PointT direction() const;
	PointT normalizedDirection() const;

	ScalarType length() const;
	ScalarType squaredLength() const;

	void flip();

	bool operator==(const Segment<PointT>& s) const = default;
	bool operator!=(const Segment<PointT>& s) const = default;

	Segment<PointT> operator+(const Segment<PointT>& s) const;
	Segment<PointT> operator-(const Segment<PointT>& s) const;
	Segment<PointT> operator*(const ScalarType& s) const;
	Segment<PointT> operator/(const ScalarType& s) const;

	Segment<PointT>& operator+=(const Segment<PointT>& s) const;
	Segment<PointT>& operator-=(const Segment<PointT>& s) const;
	Segment<PointT>& operator*=(const ScalarType& s) const;
	Segment<PointT>& operator/=(const ScalarType& s) const;

private:
	PointT point0;
	PointT point1;
};

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

/******************************************************************************
 *                                Definitions                                 *
 ******************************************************************************/

/**
 * @brief Default constructor. Creates a segment with endpoints at the origin.
 */
template<PointConcept PointT>
Segment<PointT>::Segment()
{
}

/**
 * @brief Creates a segment with the given endpoints.
 *
 * @param[in] p0: The first endpoint of the segment.
 * @param[in] p1: The second endpoint of the segment.
 */
template<PointConcept PointT>
Segment<PointT>::Segment(const PointT& p0, const PointT& p1) : point0(p0), point1(p1)
{
}

/**
 * @brief Returns the first endpoint of the segment.
 *
 * @return A reference to the first endpoint of the segment.
 */
template<PointConcept PointT>
PointT& Segment<PointT>::p0()
{
	return point0;
}

/**
 * @brief Returns the first endpoint of the segment.
 *
 * @return A const reference to the first endpoint of the segment.
 */
template<PointConcept PointT>
const PointT& Segment<PointT>::p0() const
{
	return point0;
}

/**
 * @brief Returns the second endpoint of the segment.
 *
 * @return A reference to the second endpoint of the segment.
 */
template<PointConcept PointT>
PointT& Segment<PointT>::p1()
{
	return point1;
}

/**
 * @brief Returns the second endpoint of the segment.
 *
 * @return A const reference to the second endpoint of the segment.
 */
template<PointConcept PointT>
const PointT& Segment<PointT>::p1() const
{
	return point1;
}

template<PointConcept PointT>
PointT Segment<PointT>::midPoint() const
{
	return (point0 + point1) / 2.0;
}

template<PointConcept PointT>
PointT Segment<PointT>::direction() const
{
	return point1 - point0;
}

template<PointConcept PointT>
PointT Segment<PointT>::normalizedDirection() const
{
	return (point1 - point0).normalize();
}

template<PointConcept PointT>
typename Segment<PointT>::ScalarType Segment<PointT>::length() const
{
	(point0 - point1).norm();
}

template<PointConcept PointT>
typename Segment<PointT>::ScalarType Segment<PointT>::squaredLength() const
{
	(point0 - point1).squaredNorm();
}

template<PointConcept PointT>
void Segment<PointT>::flip()
{
	std::swap(point0, point1);
}

template<PointConcept PointT>
Segment<PointT> Segment<PointT>::operator+(const Segment<PointT>& s) const
{
	return Segment<PointT>(point0 + s.point0, point1 + s.point1);
}

template<PointConcept PointT>
Segment<PointT> Segment<PointT>::operator-(const Segment<PointT>& s) const
{
	return Segment<PointT>(point0 - s.point0, point1 - s.point1);
}

template<PointConcept PointT>
Segment<PointT> Segment<PointT>::operator*(const ScalarType& s) const
{
	return Segment<PointT>(point0 * s, point1 * s);
}

template<PointConcept PointT>
Segment<PointT> Segment<PointT>::operator/(const ScalarType& s) const
{
	return Segment<PointT>(point0 / s, point1 / s);
}

template<PointConcept PointT>
Segment<PointT>& Segment<PointT>::operator+=(const Segment<PointT>& s) const
{
	point0 += s.point0;
	point1 += s.point1;
	return *this;
}

template<PointConcept PointT>
Segment<PointT>& Segment<PointT>::operator-=(const Segment<PointT>& s) const
{
	point0 -= s.point0;
	point1 -= s.point1;
	return *this;
}

template<PointConcept PointT>
Segment<PointT>& Segment<PointT>::operator*=(const ScalarType& s) const
{
	point0 *= s;
	point1 *= s;
	return *this;
}

template<PointConcept PointT>
Segment<PointT>& Segment<PointT>::operator/=(const ScalarType& s) const
{
	point0 /= s;
	point1 /= s;
	return *this;
}

} // namespace vcl

#endif // VCL_SPACE_SEGMENT_H
