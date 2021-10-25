/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "box.h"

#include <algorithm>

namespace mgp {

template<typename PointType>
Box<PointType>::Box()
{
	setNull();
}

template<typename PointType>
Box<PointType>::Box(const PointType& p) : min(p), max(p)
{
}

template<typename PointType>
Box<PointType>::Box(const PointType& min, const PointType& max) : min(min), max(max)
{
}

template<typename PointType>
bool Box<PointType>::isNull() const
{
	for (unsigned int i = 0; i < PointType::DIM; ++i) {
		if (min[i] > max[i])
			return true;
	}
	return false;
}

template<typename PointType>
bool Box<PointType>::isEmpty() const
{
	return min == max;
}

/**
 * @brief Box::isInside
 * @param p
 * @return true if p is inside the box, bounds included; e.g. p in [min, max].
 */
template<typename PointType>
bool Box<PointType>::isInside(const PointType& p) const
{
	for (unsigned int i = 0; i < PointType::DIM; ++i) {
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
template<typename PointType>
bool Box<PointType>::isInsideOpenBox(const PointType& p) const
{
	for (unsigned int i = 0; i < PointType::DIM; ++i) {
		if (p[i] < min[i] || p[i] >= max[i])
			return false;
	}
	return true;
}

template<typename PointType>
bool Box<PointType>::collide(const Box<PointType>& b) const
{
	for (unsigned int i = 0; i < PointType::DIM; ++i) {
		if (b.min[i] >= max[i] || b.max[i] <= min[i])
			return false;
	}
	return true;
}

template<typename PointType>
auto Box<PointType>::diagonal() const
{
	return (max - min).norm();
}

template<typename PointType>
auto Box<PointType>::squaredDiagonal() const
{
	return (max - min).squaredNorm();
}

template<typename PointType>
PointType Box<PointType>::center() const
{
	return (max + min) / 2;
}

template<typename PointType>
PointType Box<PointType>::size() const
{
	return max - min;
}

template<typename PointType>
auto Box<PointType>::volume() const
{
	auto vol = max[0] - min[0];
	for (unsigned int i = 1; i < PointType::DIM; ++i) {
		vol *= max[i] - min[i];
	}
	return vol;
}

template<typename PointType>
auto Box<PointType>::dim(unsigned int i) const
{
	assert(i < PointType::DIM);
	return max[i]-min[i];
}

template<typename PointType>
auto Box<PointType>::minDim() const
{
	auto m = max[0] - min[0];
	for (unsigned int i = 1; i < PointType::DIM; ++i) {
		if (max[i] - min[i] < m)
			m = max[i] - min[i];
	}
	return m;
}

template<typename PointType>
auto Box<PointType>::maxDim() const
{
	auto m = max[0] - min[0];
	for (unsigned int i = 1; i < PointType::DIM; ++i) {
		if (max[i] - min[i] > m)
			m = max[i] - min[i];
	}
	return m;
}

/**
 * @brief Box::setNull sets the Box to null. A box is considered null if at least one min component
 * is greater than the corresponding max component.
 *
 * This member function sets all the min values to 1 and the max values to -1.
 */
template<typename PointType>
void Box<PointType>::setNull()
{
	min.setConstant(1);
	max.setConstant(-1);
}

/**
 * @brief Box::add adds the given box to the current box, expanding this box in order that will
 * contain also the b values.
 *
 * If this box was null, the current box will have the same values of b.
 * @param b: the box to add to the current box.
 */
template<typename PointType>
void Box<PointType>::add(const Box<PointType>& b)
{
	// Adding a null box should do nothing
	if (!b.isNull()) {
		if (isNull()) {
			*this = b;
		}
		else {
			for (unsigned int i = 0; i < PointType::DIM; ++i) {
				if (min[i] > b.min[i])
					min[i] = b.min[i];
				if (max[i] < b.max[i])
					max[i] = b.max[i];
			}
		}
	}
}

template<typename PointType>
/**
 * @brief Box::add adds the given point to the current box, expanding this box in order that will
 * contain also the p values.
 *
 * If this box was null, both the values the current box will be set to p.
 * @param p: the point to add to the current box.
 */
void Box<PointType>::add(const PointType& p)
{
	if (isNull())
		*this = Box(p);
	else {
		for (unsigned int i = 0; i < PointType::DIM; ++i) {
			if (min[i] > p[i])
				min[i] = p[i];
			if (max[i] < p[i])
				max[i] = p[i];
		}
	}
}

/**
 * @brief Box::intersect sets the current box as the intersection between the current box and the
 * given box b
 * @param b
 */
template<typename PointType>
void Box<PointType>::intersect(const Box<PointType>& b)
{
	for (unsigned int i = 0; i < PointType::DIM; ++i) {
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
template<typename PointType>
void Box<PointType>::translate(const PointType& p)
{
	min += p;
	max += p;
}

template<typename PointType>
template<typename Scalar>
void Box<PointType>::add(const PointType& p, Scalar radius)
{
	if (isNull())
		*this = Box(p);
	else {
		for (unsigned int i = 0; i < PointType::DIM; ++i) {
			min[i] = std::min(min[i], p[i] - radius);
			max[i] = std::max(max[i], p[i] + radius);
		}
	}
}

} // namespace mgp
