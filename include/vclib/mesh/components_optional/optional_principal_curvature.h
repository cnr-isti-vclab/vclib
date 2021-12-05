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

#include "../components/detection/principal_curvature_detection.h"

namespace vcl::comp {

template<typename Scalar, typename T>
class OptionalPrincipalCurvature :
		public OptionalPrincipalCurvatureTriggerer,
		public virtual OptionalInfo<T>
{
private:
	using B = OptionalInfo<T>;
	uint thisId() const { return B::index((T*)this); }

public:
	using PrincipalCurvatureType = vcl::PrincipalCurvature<Scalar>;

	OptionalPrincipalCurvature();

	const PrincipalCurvatureType& principalCurvature() const;
	PrincipalCurvatureType&       principalCurvature();

	bool isPrincipalCurvatureEnabled() const;

protected:
	template <typename Element>
	void importFrom(const Element& e);
};

template<typename T>
using OptionalPrincipalCurvaturef = OptionalPrincipalCurvature<float, T>;

template<typename T>
using OptionalPrincipalCurvatured = OptionalPrincipalCurvature<double, T>;

} // namespace vcl::comp

#include "optional_principal_curvature.cpp"

#endif // VCL_MESH_COMPONENTS_OPTIONAL_PRINCIPAL_CURVATURE_H
