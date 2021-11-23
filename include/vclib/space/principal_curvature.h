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

#ifndef VCL_PRINCIPAL_CURVATURE_H
#define VCL_PRINCIPAL_CURVATURE_H

#include "point/point3.h"

namespace vcl {

template<typename Scalar>
class PrincipalCurvature
{
public:
	PrincipalCurvature();

	const Point3<Scalar>& maxDir() const;
	Point3<Scalar>&       maxDir();
	const Point3<Scalar>& minDir() const;
	Point3<Scalar>&       minDir();
	const Scalar&         maxValue() const;
	Scalar&               maxValue();
	const Scalar&         minValue() const;
	Scalar&               minValue();

private:
	Point3<Scalar> dir1, dir2;
	Scalar         k1, k2;
};

} // namespace vcl

#include "principal_curvature.cpp"

#endif // VCL_PRINCIPAL_CURVATURE_H
