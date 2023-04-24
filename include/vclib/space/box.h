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

#ifndef VCL_SPACE_BOX_H
#define VCL_SPACE_BOX_H

#include <vclib/concepts/space/box.h>

#include "point.h"

namespace vcl {

/**
 * @brief A class representing a box in N-dimensional space.
 *
 * The `Box` class represents an axis-aligned box in N-dimensional space, defined
 * by its minimum and maximum corners. It provides functions for performing various
 * operations on boxes, such as checking if a point is inside the box, computing the
 * box's diagonal, and adding other boxes to the current box. The class is parameterized
 * by a `PointConcept`, which must provide the `DIM` constant and the `[]` operator for
 * accessing the point coordinates.
 *
 * @tparam PointT A type that satisfies the `PointConcept` requirements.
 *
 * @ingroup space
 */
template<PointConcept PointT>
class Box
{
public:
	/**
	 * @brief The type of point used to represent the corners of the box.
	 */
	using PointType = PointT;

	/**
	 * @brief The dimensionality of the box.
	 */
	static const uint DIM = PointT::DIM;

	Box();
	Box(const PointT& p);
	Box(const PointT& min, const PointT& max);

	PointT& min();
	const PointT& min() const;

	PointT& max();
	const PointT& max() const;

	template<typename Scalar>
	auto cast() const;

	bool isNull() const;
	bool isEmpty() const;

	bool isInside(const PointT& p) const;
	bool isInsideOpenBox(const PointT& p) const;
	bool overlap(const Box<PointT>& b) const;
	bool collide(const Box<PointT>& b) const;
	bool intersects(const Box<PointT>& b) const;
	auto diagonal() const;
	auto squaredDiagonal() const;
	PointT center() const;
	PointT size() const;
	auto volume() const;
	auto dim(uint i) const;
	auto minDim() const;
	auto maxDim() const;
	Box<PointT> intersection(const Box<PointT>& p) const;

	void setNull();

	void add(const PointT& p);

	template<typename Scalar>
	void add(const PointT& p, Scalar radius);

	void add(const Box<PointT>& b);

	void translate(const PointT& p);

	bool operator==(const Box<PointT>& b) const;
	bool operator!=(const Box<PointT>& b) const;

private:
	PointT minP;
	PointT maxP;
};

// common types
template <typename S>
using Box2 = Box<Point2<S>>;

using Box2i = Box<Point2i>;
using Box2f = Box<Point2f>;
using Box2d = Box<Point2d>;

template <typename S>
using Box3 = Box<Point3<S>>;

using Box3i = Box<Point3i>;
using Box3f = Box<Point3f>;
using Box3d = Box<Point3d>;

} // namespace vcl

#include "box.cpp"

#endif // VCL_SPACE_BOX_H
