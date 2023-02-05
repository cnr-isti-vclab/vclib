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

#ifndef VCL_MESH_COMPONENTS_NORMAL_H
#define VCL_MESH_COMPONENTS_NORMAL_H

#include <vclib/space/point.h>

#include "concepts/normal.h"

namespace vcl::comp {

template <PointConcept P>
class NormalT
{
public:
	using NormalType = P;

	const P& normal() const;
	P&       normal();

	constexpr bool isNormalEnabled() const { return true; }

protected:
	template<typename Element>
	void importFrom(const Element& e);

private:
	P n;
};

template<typename Scalar, int N>
using Normal = NormalT<Point<Scalar, N>>;

template<typename Scalar>
using Normal3 = NormalT<Point3<Scalar>>;

using Normal3f = Normal3<float>;
using Normal3d = Normal3<double>;

} // namespace vcl::comp

#include "normal.cpp"

#endif // VCL_MESH_COMPONENTS_NORMAL_H
