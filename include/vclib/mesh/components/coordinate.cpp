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

#include "coordinate.h"

namespace vcl::comp {

template<typename Scalar, int N>
const Point<Scalar, N>& Coordinate<Scalar, N>::coord() const
{
	return p;
}

template<typename Scalar, int N>
Point<Scalar, N>& Coordinate<Scalar, N>::coord()
{
	return p;
}

template<typename Scalar>
const Point3<Scalar>& Coordinate3<Scalar>::coord() const
{
	return p;
}

template<typename Scalar>
Point3<Scalar>& Coordinate3<Scalar>::coord()
{
	return p;
}

} // namespace vcl::comp
