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

#ifndef VCL_MESH_COMPONENTS_NORMAL_H
#define VCL_MESH_COMPONENTS_NORMAL_H

#include <vclib/space/point.h>

#include "detection/normal_detection.h"

namespace vcl::comp {

template<typename Scalar, int N>
class Normal : public NormalTriggerer
{
public:
	using NormalType = Point<Scalar, N>;

	const Point<Scalar, N>& normal() const;
	Point<Scalar, N>&       normal();

private:
	Point<Scalar, N> n;
};

template<typename Scalar>
class Normal3 : public NormalTriggerer
{
public:
	using NormalType = Point3<Scalar>;

	const Point3<Scalar>& normal() const;
	Point3<Scalar>&       normal();

private:
	Point3<Scalar> n;
};

using Normal3f = Normal3<float>;
using Normal3d = Normal3<double>;

} // namespace vcl::comp

#include "normal.cpp"

#endif // VCL_MESH_COMPONENTS_NORMAL_H
