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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_PRINCIPAL_CURVATURE_H
#define VCL_MESH_COMPONENTS_OPTIONAL_PRINCIPAL_CURVATURE_H

#include "optional_info.h"

#include <vclib/space/principal_curvature.h>

namespace vcl::comp {

class OptionalPrincipalCurvatureTriggerer
{
};

template<typename Scalar, typename T>
class OptionalPrincipalCurvature :
		public OptionalPrincipalCurvatureTriggerer,
		public virtual OptionalInfo<T>
{
private:
	using B = OptionalInfo<T>;

public:
	using PrincipalCurvatureType = vcl::PrincipalCurvature<Scalar>;

	OptionalPrincipalCurvature();

	const PrincipalCurvatureType& principalCurvature() const;
	PrincipalCurvatureType&       principalCurvature();

private:
	uint thisId() const;
};

template<typename T>
using OptionalPrincipalCurvaturef = OptionalPrincipalCurvature<float, T>;

template<typename T>
using OptionalPrincipalCurvatured = OptionalPrincipalCurvature<double, T>;

/**
 * Detector to check if a class has (inherits) OptionalPrincipalCurvature
 */

template<typename T>
using hasOptionalPrincipalCurvatureT = std::is_base_of<OptionalPrincipalCurvatureTriggerer, T>;

template<typename T>
bool constexpr hasOptionalPrincipalCurvature()
{
	return hasOptionalPrincipalCurvatureT<T>::value;
}

} // namespace vcl::comp

#include "optional_principal_curvature.cpp"

#endif // VCL_MESH_COMPONENTS_OPTIONAL_PRINCIPAL_CURVATURE_H
