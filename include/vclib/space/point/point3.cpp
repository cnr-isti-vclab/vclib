/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "point3.h"

namespace vcl {

template<typename ScalarType>
Point3<ScalarType>::Point3(const Point<ScalarType, 3>& p1) :
		Point<ScalarType, 3>(p1)
{
}

template<typename ScalarType>
Point3<ScalarType>::Point3(const ScalarType& x, const ScalarType& y, const ScalarType& z)
{
	Point<ScalarType, 3>::p << x, y, z;
}

template<typename ScalarType>
ScalarType Point3<ScalarType>::x() const
{
	return Point<ScalarType, 3>::p.x();
}

template<typename ScalarType>
ScalarType Point3<ScalarType>::y() const
{
	return Point<ScalarType, 3>::p.y();
}

template<typename ScalarType>
ScalarType Point3<ScalarType>::z() const
{
	return Point<ScalarType, 3>::p.z();
}

template<typename ScalarType>
ScalarType& Point3<ScalarType>::x()
{
	return Point<ScalarType, 3>::p.x();
}

template<typename ScalarType>
ScalarType& Point3<ScalarType>::y()
{
	return Point<ScalarType, 3>::p.y();
}

template<typename ScalarType>
ScalarType& Point3<ScalarType>::z()
{
	return Point<ScalarType, 3>::p.z();
}

} // namespace vcl
