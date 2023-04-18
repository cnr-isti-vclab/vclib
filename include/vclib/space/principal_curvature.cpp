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

#include "principal_curvature.h"

namespace vcl {

template<typename Scalar>
PrincipalCurvature<Scalar>::PrincipalCurvature()
{
}

template<typename Scalar>
template<typename S>
PrincipalCurvature<S> PrincipalCurvature<Scalar>::cast() const
{
	if constexpr (std::is_same<Scalar, S>::value) {
		return *this;
	}
	else {
		PrincipalCurvature<S> tmp;
		tmp.maxDir() = dir1.template cast<S>();
		tmp.minDir() = dir2.template cast<S>();
		tmp.maxValue() = k1;
		tmp.minValue() = k2;
		return tmp;
	}
}

template<typename Scalar>
const Point3<Scalar>& PrincipalCurvature<Scalar>::maxDir() const
{
	return dir1;
}

template<typename Scalar>
Point3<Scalar>& PrincipalCurvature<Scalar>::maxDir()
{
	return dir1;
}

template<typename Scalar>
const Point3<Scalar>& PrincipalCurvature<Scalar>::minDir() const
{
	return dir2;
}

template<typename Scalar>
Point3<Scalar>& PrincipalCurvature<Scalar>::minDir()
{
	return dir2;
}

template<typename Scalar>
const Scalar& PrincipalCurvature<Scalar>::maxValue() const
{
	return k1;
}

template<typename Scalar>
Scalar& PrincipalCurvature<Scalar>::maxValue()
{
	return k1;
}

template<typename Scalar>
const Scalar& PrincipalCurvature<Scalar>::minValue() const
{
	return k2;
}

template<typename Scalar>
Scalar& PrincipalCurvature<Scalar>::minValue()
{
	return k2;
}

} // namespace vcl
