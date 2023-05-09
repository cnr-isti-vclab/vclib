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

#ifndef VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H
#define VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H

#include <vclib/concepts/mesh/components/principal_curvature.h>
#include <vclib/space/principal_curvature.h>

#include "bases/component.h"

namespace vcl::comp {

/**
 * @brief The PrincipalCurvature class
 *
 * @ingroup components
 */
template<typename Scalar, typename ElementType = void, bool optional = false>
class PrincipalCurvature : public Component<vcl::PrincipalCurvature<Scalar>, ElementType, optional>
{
	using Base = Component<vcl::PrincipalCurvature<Scalar>, ElementType, optional>;
	using ThisType = PrincipalCurvature<Scalar, ElementType, optional>;

public:
	// expose the type to allow access to this component
	using PrincipalCurvatureComponent = ThisType;

	using PrincipalCurvatureType = vcl::PrincipalCurvature<Scalar>;

	bool isEnabled() const;
	bool isPrincipalCurvatureEnabled() const;

	const PrincipalCurvatureType& principalCurvature() const;
	PrincipalCurvatureType&       principalCurvature();

protected:
	template<typename Element>
	void importFrom(const Element& e);
};

/* Detector function to check if a class has PrincipalCurvature enabled */

template <typename T>
bool isPrincipalCurvatureEnabledOn(const T& element);

template<typename ElementType = void, bool optional = false>
using PrincipalCurvaturef = PrincipalCurvature<float, ElementType, optional>;

template<typename ElementType = void, bool optional = false>
using PrincipalCurvatured = PrincipalCurvature<double, ElementType, optional>;

} // namespace vcl::comp

#include "principal_curvature.cpp"

#endif // VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H
