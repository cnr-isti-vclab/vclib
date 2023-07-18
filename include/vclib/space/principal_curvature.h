/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_SPACE_PRINCIPAL_CURVATURE_H
#define VCL_SPACE_PRINCIPAL_CURVATURE_H

#include "point.h"

namespace vcl {

/**
 * @brief The PrincipalCurvature class stores the principal curvature directions
 * and values at a point on a surface.
 *
 * It stores the two principal curvature directions (`maxDir()` and `minDir()`,
 * also note as k1 and k2 respectively) and the maximum and minimum values of
 * the curvature (`maxValue()` and `minValue()`).
 *
 * @tparam Scalar: The scalar type of the curvature values.
 *
 * @ingroup space
 */
template<typename Scalar>
class PrincipalCurvature
{
public:
	/**
	 * @brief The scalar type of the curvature values.
	 */
	using ScalarType = Scalar;

	PrincipalCurvature();

	template<typename S>
	PrincipalCurvature<S> cast() const;

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
	Scalar         k1 = 0, k2 = 0;
};

} // namespace vcl

#include "principal_curvature.cpp"

#endif // VCL_SPACE_PRINCIPAL_CURVATURE_H
