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

#include "box.h"

#include <algorithm>

namespace vcl {

/**
 * @brief The Empty constructor of a box, initializes a null box.
 * @see setNull
 */
template<PointConcept PointT>
Box<PointT>::Box()
{
	setNull();
}

template<PointConcept PointT>
Box<PointT>::Box(const PointT& p) : min(p), max(p)
{
}

template<PointConcept PointT>
Box<PointT>::Box(const PointT& min, const PointT& max) : min(min), max(max)
{
}

template<PointConcept PointT>
template<typename P>
Box<PointT>::Box(const Box<P>& ob) :
		min(ob.min), max(ob.max)
{
}

template<PointConcept PointT>
bool Box<PointT>::isNull() const
{
	for (uint i = 0; i < PointT::DIM; ++i) {
		if (min[i] > max[i])
			return true;
	}
	return false;
}

template<PointConcept PointT>
bool Box<PointT>::isEmpty() const
{
	return min == max;
}

/**
 * @brief Box::isInside
 * @param p
 * @return true if p is inside the box, bounds included; e.g. p in [min, max].
 */
template<PointConcept PointT>
bool Box<PointT>::isInside(const PointT& p) const
{
	for (uint i = 0; i < PointT::DIM; ++i) {
		if (p[i] < min[i] || p[i] > max[i])
			return false;
	}
	return true;
}

/**
 * @brief Box::isInsideEx
 * @param p: the point to check if is inside the curren box.
 * @return true if p is inside the open box (max bound excluded); e.g. p in [min, max).
 */
template<PointConcept PointT>
bool Box<PointT>::isInsideOpenBox(const PointT& p) const
{
	for (uint i = 0; i < PointT::DIM; ++i) {
		if (p[i] < min[i] || p[i] >= max[i])
			return false;
	}
	return true;
}

template<PointConcept PointT>
bool Box<PointT>::collide(const Box<PointT>& b) const
{
	for (uint i = 0; i < PointT::DIM; ++i) {
		if (b.min[i] >= max[i] || b.max[i] <= min[i])
			return false;
	}
	return true;
}

template<PointConcept PointT>
auto Box<PointT>::diagonal() const
{
	return min.dist(max);
}

template<PointConcept PointT>
auto Box<PointT>::squaredDiagonal() const
{
	return min.squaredDist(max);
}

template<PointConcept PointT>
PointT Box<PointT>::center() const
{
	return (max + min) / 2;
}

template<PointConcept PointT>
PointT Box<PointT>::size() const
{
	return max - min;
}

template<PointConcept PointT>
auto Box<PointT>::volume() const
{
	auto vol = max[0] - min[0];
	for (uint i = 1; i < PointT::DIM; ++i) {
		vol *= max[i] - min[i];
	}
	return vol;
}

template<PointConcept PointT>
auto Box<PointT>::dim(uint i) const
{
	assert(i < PointT::DIM);
	return max[i]-min[i];
}

template<PointConcept PointT>
auto Box<PointT>::minDim() const
{
	auto m = max[0] - min[0];
	for (uint i = 1; i < PointT::DIM; ++i) {
		if (max[i] - min[i] < m)
			m = max[i] - min[i];
	}
	return m;
}

template<PointConcept PointT>
auto Box<PointT>::maxDim() const
{
	auto m = max[0] - min[0];
	for (uint i = 1; i < PointT::DIM; ++i) {
		if (max[i] - min[i] > m)
			m = max[i] - min[i];
	}
	return m;
}

/**
 * @brief Box::setNull sets the Box to null. A box is considered null if at least one min component
 * is greater than the corresponding max component.
 *
 * This member function sets all the min values to the max representable Scalar and the max
 * values to lowest representable Scalar.
 */
template<PointConcept PointT>
void Box<PointT>::setNull()
{
	min.setConstant(std::numeric_limits<typename PointT::ScalarType>::max());
	max.setConstant(std::numeric_limits<typename PointT::ScalarType>::lowest());
}

/**
 * @brief Box::add adds the given box to the current box, expanding this box in order that will
 * contain also the b values.
 *
 * If this box was null, the current box will have the same values of b.
 * @param b: the box to add to the current box.
 */
template<PointConcept PointT>
void Box<PointT>::add(const Box<PointT>& b)
{
	// Adding a null box should do nothing
	if (!b.isNull()) {
		if (isNull()) {
			*this = b;
		}
		else {
			add(b.min);
			add(b.max);
		}
	}
}

template<PointConcept PointT>
/**
 * @brief Box::add adds the given point to the current box, expanding this box in order that will
 * contain also the p values.
 *
 * If this box was null, both the values the current box will be set to p.
 * @param p: the point to add to the current box.
 */
void Box<PointT>::add(const PointT& p)
{
	if (isNull())
		*this = Box(p);
	else {
		min = vcl::min(min, p);
		max = vcl::max(max, p);
	}
}

/**
 * @brief Box::intersect sets the current box as the intersection between the current box and the
 * given box b
 * @param b
 */
template<PointConcept PointT>
void Box<PointT>::intersect(const Box<PointT>& b)
{
	for (uint i = 0; i < PointT::DIM; ++i) {
		if (min[i] < b.min[i])
			min[i] = b.min[i];
		if (max[i] > b.max[i])
			max[i] = b.max[i];
		if (min[i] > max[i]){
			setNull();
			return;
		}
	}
}

/**
 * @brief Translates the box by summing the values of p
 * @param p
 */
template<PointConcept PointT>
void Box<PointT>::translate(const PointT& p)
{
	min += p;
	max += p;
}

template<PointConcept PointT>
bool Box<PointT>::operator==(const Box<PointT>& p) const
{
	return min == p.min && max == p.max;
}

template<PointConcept PointT>
bool Box<PointT>::operator!=(const Box<PointT>& p) const
{
	return !(*this == p);
}

template<PointConcept PointT>
template<typename Scalar>
void Box<PointT>::add(const PointT& p, Scalar radius)
{
	if (isNull())
		*this = Box(p - radius, p + radius);
	else {
		min = vcl::min(min, p - radius);
		max = vcl::max(max, p + radius);
	}
}

} // namespace vcl
