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

#include "optional_principal_curvature.h"

namespace vcl::comp {

template<typename Scalar, typename T>
OptionalPrincipalCurvature<Scalar, T>::OptionalPrincipalCurvature()
{
}

template<typename Scalar, typename T>
const typename OptionalPrincipalCurvature<Scalar, T>::PrincipalCurvatureType&
OptionalPrincipalCurvature<Scalar, T>::principalCurvature() const
{
	return B::optCont().principalCurvature(thisId());
}

template<typename Scalar, typename T>
typename OptionalPrincipalCurvature<Scalar, T>::PrincipalCurvatureType&
OptionalPrincipalCurvature<Scalar, T>::principalCurvature()
{
	return B::optCont().principalCurvature(thisId());
}

template<typename Scalar, typename T>
bool OptionalPrincipalCurvature<Scalar, T>::isPrincipalCurvatureEnabled() const
{
	if (B::contPtr != nullptr)
		return B::optCont().isPrincipalCurvatureEnabled();
	else
		return false;
}

template<typename Scalar, typename T>
template<typename Element>
void OptionalPrincipalCurvature<Scalar, T>::importFrom(const Element& e)
{
	if constexpr (HasPrincipalCurvature<Element>) {
		if (isPrincipalCurvatureEnabled() && isPrincipalCurvatureEnabledOn(e)) {
			principalCurvature() = e.principalCurvature().template cast<Scalar>();
		}
	}
}

} // namespace vcl::comp
