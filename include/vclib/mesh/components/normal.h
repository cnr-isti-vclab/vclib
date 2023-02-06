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
#include "internal/get_vertical_component_data.h"

namespace vcl::comp {

namespace internal {

template<PointConcept P, bool>
struct NormalTData { P n; };

template<PointConcept P>
struct NormalTData<P, false> { };

} // vcl::comp::internal

template <PointConcept P, typename ElementType, bool horizontal>
class NormalT
{
	using ThisType = NormalT<P, ElementType, horizontal>;
public:
	using DataValueType = P;         // data that the component stores internally (or vertically)
	using NormalComponent = ThisType; // expose the type to allow access to this component

	static const bool IS_VERTICAL = !horizontal;

	using NormalType = P;

	const P& normal() const;
	P&       normal();

	constexpr bool isNormalEnabled() const { return true; }

protected:
	template<typename Element>
	void importFrom(const Element& e);

private:
	// members that allow to access the normal, trough data (horizontal) or trough parent (vertical)
	P& n();
	const P& n() const;

	// contians the actual point, if the component is horizontal
	internal::NormalTData<P, horizontal> data;
};

template<typename Scalar, int N, typename ElementType, bool horizontal>
using Normal = NormalT<Point<Scalar, N>, ElementType, horizontal>;

template<typename Scalar, typename ElementType, bool horizontal>
using Normal3 = NormalT<Point3<Scalar>, ElementType, horizontal>;

template<typename ElementType, bool horizontal>
using Normal3f = Normal3<float, ElementType, horizontal>;

template<typename ElementType, bool horizontal>
using Normal3d = Normal3<double, ElementType, horizontal>;

} // namespace vcl::comp

#include "normal.cpp"

#endif // VCL_MESH_COMPONENTS_NORMAL_H
