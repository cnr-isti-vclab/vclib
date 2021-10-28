/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_BOX_H
#define MGP_BOX_H

#include <mgp/misc/type_traits.h>

#include "point.h"

namespace mgp {

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
	auto dim(unsigned int i) const;
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

} // namespace mgp

#include "box.cpp"

#endif // MGP_BOX_H
