/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#ifndef VCL_BOX_H
#define VCL_BOX_H

#include "point.h"

namespace vcl {

template<typename PointType>
class Box
{
public:
	Box();
	Box(const PointType& p);
	Box(const PointType& min, const PointType& max);

	bool isNull() const;
	bool isEmpty() const;

	bool isInside(const PointType& p) const;
	bool isInsideOpenBox(const PointType& p) const;
	bool collide(const Box<PointType>& b) const;
	auto diagonal() const;
	auto squaredDiagonal() const;
	PointType center() const;
	PointType size() const;
	auto volume() const;
	auto dim(uint i) const;
	auto minDim() const;
	auto maxDim() const;

	void setNull();

	void add(const Box<PointType>& b);
	void add(const PointType& p);
	template<typename Scalar>
	void add(const PointType& p, Scalar radius);

	void intersect(const Box<PointType>& p);
	void translate(const PointType& p);

	PointType min;
	PointType max;
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

#endif // VCL_BOX_H
