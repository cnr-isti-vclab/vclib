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

#include "detection/coordinate_detection.h"

#include <vclib/space/point.h>

namespace vcl::comp {

template<typename P>
class CoordT : public CoordinateTriggerer
{
public:
	using CoordType = P;

	const P& coord() const;
	P&       coord();

	template<typename VertexType>
	void importFrom(const VertexType& v);

private:
	P p;
};

template<typename Scalar, int N>
using Coordinate = CoordT<Point<Scalar, N>>;

template<typename Scalar>
using Coordinate3 = CoordT<Point3<Scalar>>;

using Coordinate3f = Coordinate3<float>;
using Coordinate3d = Coordinate3<double>;

} // namespace vcl::comp

#include "coordinate.cpp"

#endif // VCL_MESH_COMPONENTS_COORDINATE_H
