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

template<typename PointType>
void Box<PointType>::setNull()
{
	min.setConstant(1);
	max.setConstant(-1);
}

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
