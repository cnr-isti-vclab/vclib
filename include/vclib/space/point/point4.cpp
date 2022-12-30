/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#include "point4.h"

namespace vcl {

template<typename ScalarType>
Point4<ScalarType>::Point4(const Point<ScalarType, 4>& p1) :
		Point<ScalarType, 4>(p1)
{
}

template<typename ScalarType>
Point4<ScalarType>::Point4(const ScalarType& x, const ScalarType& y, const ScalarType& z, const ScalarType& w)
{
	Point<ScalarType, 4>::p << x, y, z, w;
}

template<typename ScalarType>
ScalarType Point4<ScalarType>::x() const
{
	return Point<ScalarType, 4>::p.x();
}

template<typename ScalarType>
ScalarType Point4<ScalarType>::y() const
{
	return Point<ScalarType, 4>::p.y();
}

template<typename ScalarType>
ScalarType Point4<ScalarType>::z() const
{
	return Point<ScalarType, 4>::p.z();
}

template<typename ScalarType>
ScalarType Point4<ScalarType>::w() const
{
	return Point<ScalarType, 4>::p.w();
}

template<typename ScalarType>
ScalarType& Point4<ScalarType>::x()
{
	return Point<ScalarType, 4>::p.x();
}

template<typename ScalarType>
ScalarType& Point4<ScalarType>::y()
{
	return Point<ScalarType, 4>::p.y();
}

template<typename ScalarType>
ScalarType& Point4<ScalarType>::z()
{
	return Point<ScalarType, 4>::p.z();
}

template<typename ScalarType>
ScalarType& Point4<ScalarType>::w()
{
	return Point<ScalarType, 4>::p.w();
}

} // namespace vcl
