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

#ifndef VCL_MESH_COMPONENTS_COORDINATE_H
#define VCL_MESH_COMPONENTS_COORDINATE_H

#include <vclib/space/point.h>

namespace vcl::comp {

class CoordinateTriggerer
{
};

template<typename Scalar, int N>
class Coordinate : public CoordinateTriggerer
{
public:
	using CoordType = Point<Scalar, N>;

	const Point<Scalar, N>& coord() const;
	Point<Scalar, N>&       coord();

private:
	Point<Scalar, N> p;
};

template<typename Scalar>
class Coordinate3 : public CoordinateTriggerer
{
public:
	using CoordType = Point3<Scalar>;

	const Point3<Scalar>& coord() const;
	Point3<Scalar>&       coord();

private:
	Point3<Scalar> p;
};

using Coordinate3f = Coordinate3<float>;
using Coordinate3d = Coordinate3<double>;

/**
 * Detector to check if a class has (inherits) Coordinate
 */

template<typename T>
using hasCoordinateT = std::is_base_of<CoordinateTriggerer, T>;

template<typename T>
bool constexpr hasCoordinate()
{
	return hasCoordinateT<T>::value;
}

} // namespace vcl::comp

#include "coordinate.cpp"

#endif // VCL_MESH_COMPONENTS_COORDINATE_H
