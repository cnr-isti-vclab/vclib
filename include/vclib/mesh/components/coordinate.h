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

#ifndef VCL_MESH_COMPONENTS_COORDINATE_H
#define VCL_MESH_COMPONENTS_COORDINATE_H

#include "concepts/coordinate.h"
#include "internal/get_vertical_component_data.h"

#include <vclib/space/point.h>

namespace vcl::comp {

namespace internal {

template<PointConcept P, bool>
struct CoordTData { P p; };

template<PointConcept P>
struct CoordTData<P, false> { };

} // vcl::comp::internal

template<PointConcept P, typename ElementType, bool horizontal>
class CoordT
{
	using ThisType = CoordT<P, ElementType, horizontal>;
public:
	using DataValueType = P;         // data that the component stores internally (or vertically)
	using CoordComponent = ThisType; // expose the type to allow access to this component

	static const bool IS_VERTICAL = !horizontal;

	using CoordType = P;

	const P& coord() const;
	P&       coord();

protected:
	template<typename Element>
	void importFrom(const Element& v);

private:
	// members that allow to access the point, trough data (horizontal) or trough parent (vertical)
	P& p();
	const P& p() const;

	// contians the actual point, if the component is horizontal
	internal::CoordTData<P, horizontal> data;
};

template<typename Scalar, int N, typename ElementType, bool horizontal>
using Coordinate = CoordT<Point<Scalar, N>, ElementType, horizontal>;

template<typename Scalar, typename ElementType, bool horizontal>
using Coordinate3 = CoordT<Point3<Scalar>, ElementType, horizontal>;

template<typename ElementType, bool horizontal>
using Coordinate3f = Coordinate3<float, ElementType, horizontal>;

template<typename ElementType, bool horizontal>
using Coordinate3d = Coordinate3<double, ElementType, horizontal>;

} // namespace vcl::comp

#include "coordinate.cpp"

#endif // VCL_MESH_COMPONENTS_COORDINATE_H
