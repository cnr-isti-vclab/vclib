/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#ifndef VCL_MATH_BASE_H
#define VCL_MATH_BASE_H

#include <cmath>

namespace vcl {

template<typename Scalar>
bool isDegenerate(Scalar t)
{
	return std::isinf(t) || std::isnan(t);
}

template <typename Scalar>
bool epsilonCompare(Scalar p1, Scalar p2, Scalar epsilon = std::numeric_limits<Scalar>::epsilon())
{
	return (std::abs(p1 - p2) <= epsilon);
}

template<typename Scalar>
Scalar toRad(const Scalar& a)
{
	return M_PI*a/180.0;
}

template<typename Scalar>
Scalar toDeg(const Scalar& a)
{
	return a*180.0/M_PI;
}

} // namespace vcl

#endif // VCL_MATH_BASE_H
