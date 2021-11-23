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

#ifndef VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H
#define VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H

#include <vclib/space/principal_curvature.h>

namespace vcl::comp {

class PrincipalCurvatureTriggerer
{
};

template<typename Scalar>
class PrincipalCurvature
{
public:
	using PrincipalCurvatureType = vcl::PrincipalCurvature<Scalar>;

	PrincipalCurvature();

	const PrincipalCurvatureType& principalCurvature() const;
	PrincipalCurvatureType&       principalCurvature();

private:
	vcl::PrincipalCurvature<Scalar> princCurv;
};

using PrincipalCurvaturef = PrincipalCurvature<float>;
using PrincipalCurvatured = PrincipalCurvature<double>;

/**
 * Detector to check if a class has (inherits) PrincipalCurvature
 */

template<typename T>
using hasPrincipalCurvatureT = std::is_base_of<PrincipalCurvatureTriggerer, T>;

template<typename T>
bool constexpr hasPrincipalCurvature()
{
	return hasPrincipalCurvatureT<T>::value;
}

} // namespace vcl::comp

#include "principal_curvature.cpp"

#endif // VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H
