/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

namespace vcl::comp {

class NormalTrigger
{
};

template<typename Scalar, int N>
class Normal : public NormalTrigger
{
public:
	using NormalType = Point<Scalar, N>;

	const Point<Scalar, N>& normal() const { return n; }
	Point<Scalar, N>&       normal() { return n; }

private:
	Point<Scalar, N> n;
};

template<typename Scalar>
class Normal3 : public NormalTrigger
{
public:
	using NormalType = Point3<Scalar>;

	const Point3<Scalar>& normal() const { return n; }
	Point3<Scalar>&       normal() { return n; }

private:
	Point3<Scalar> n;
};

using Normal3f = Normal3<float>;
using Normal3d = Normal3<double>;

/**
 * Detector to check if a class has (inherits) Normal
 */

template<typename T>
using hasNormalT = std::is_base_of<NormalTrigger, T>;

template<typename T>
bool constexpr hasNormal()
{
	return hasNormalT<T>::value;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_NORMAL_H
