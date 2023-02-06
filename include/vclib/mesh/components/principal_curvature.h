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

#ifndef VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H
#define VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H

#include <vclib/space/principal_curvature.h>

#include "concepts/principal_curvature.h"
#include "internal/component_data.h"

namespace vcl::comp {

namespace internal {

template<typename Scalar, bool>
struct PrincipalCurvatureData { vcl::PrincipalCurvature<Scalar> princCurv; };

template<typename Scalar>
struct PrincipalCurvatureData<Scalar, false> { };

} // vcl::comp::internal

template<typename Scalar, typename ElementType, bool horizontal>
class PrincipalCurvature
{
	using ThisType = PrincipalCurvature<Scalar, ElementType, horizontal>;
public:
	// data that the component stores internally (or vertically)
	using DataValueType = vcl::PrincipalCurvature<Scalar>;
	// expose the type to allow access to this component
	using PrincipalCurvatureComponent = ThisType;

	static const bool IS_VERTICAL = !horizontal;

	using PrincipalCurvatureType = vcl::PrincipalCurvature<Scalar>;

	const PrincipalCurvatureType& principalCurvature() const;
	PrincipalCurvatureType&       principalCurvature();

	constexpr bool isPrincipalCurvatureEnabled() const { return true; }

protected:
	template<typename Element>
	void importFrom(const Element& e);

private:
	// members that allow to access the curvature, trough data (hor) or trough parent (vert)
	vcl::PrincipalCurvature<Scalar>& princCurv();
	const vcl::PrincipalCurvature<Scalar>& princCurv() const;

	// contians the actual data of the component, if the component is horizontal
	internal::ComponentData<DataValueType, horizontal> data;
};

template<typename ElementType, bool horizontal>
using PrincipalCurvaturef = PrincipalCurvature<float, ElementType, horizontal>;

template<typename ElementType, bool horizontal>
using PrincipalCurvatured = PrincipalCurvature<double, ElementType, horizontal>;

} // namespace vcl::comp

#include "principal_curvature.cpp"

#endif // VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H
