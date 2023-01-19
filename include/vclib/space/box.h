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

#include "point.h"

namespace vcl {

template<PointConcept PointT>
class Box
{
public:
	using PointType = PointT;

	Box();
	Box(const PointT& p);
	Box(const PointT& min, const PointT& max);

	template<typename P>
	Box(const Box<P>& ob);

	bool isNull() const;
	bool isEmpty() const;

	bool isInside(const PointT& p) const;
	bool isInsideOpenBox(const PointT& p) const;
	bool collide(const Box<PointT>& b) const;
	auto diagonal() const;
	auto squaredDiagonal() const;
	PointT center() const;
	PointT size() const;
	auto volume() const;
	auto dim(uint i) const;
	auto minDim() const;
	auto maxDim() const;

	void setNull();

	void add(const Box<PointT>& b);
	void add(const PointT& p);
	template<typename Scalar>
	void add(const PointT& p, Scalar radius);

	void intersect(const Box<PointT>& p);
	void translate(const PointT& p);

	PointT min;
	PointT max;
};

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
