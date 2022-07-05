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

#ifndef VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H
#define VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H

#include <vclib/space/principal_curvature.h>

#include "../concepts/principal_curvature.h"

namespace vcl::comp {

template<typename Scalar>
class PrincipalCurvature
{
public:
	using PrincipalCurvatureType = vcl::PrincipalCurvature<Scalar>;

	const PrincipalCurvatureType& principalCurvature() const;
	PrincipalCurvatureType&       principalCurvature();

protected:
	template<typename Element>
	void importFrom(const Element& e);

private:
	vcl::PrincipalCurvature<Scalar> princCurv;
};

using PrincipalCurvaturef = PrincipalCurvature<float>;
using PrincipalCurvatured = PrincipalCurvature<double>;

} // namespace vcl::comp

#include "principal_curvature.cpp"

#endif // VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H
