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

namespace vcl::comp {

template<typename Scalar, typename El, bool O>
bool PrincipalCurvature<Scalar, El, O>::isEnabled() const
{
	return Base::isEnabled(this);
}

template<typename Scalar, typename El, bool O>
bool PrincipalCurvature<Scalar, El, O>::isPrincipalCurvatureEnabled() const
{
	return isEnabled();
}

template<typename Scalar, typename El, bool O>
const typename PrincipalCurvature<Scalar, El, O>::PrincipalCurvatureType&
PrincipalCurvature<Scalar, El, O>::principalCurvature() const
{
	return Base::data(this);
}

template<typename Scalar, typename El, bool O>
typename PrincipalCurvature<Scalar, El, O>::PrincipalCurvatureType&
PrincipalCurvature<Scalar, El, O>::principalCurvature()
{
	return Base::data(this);
}

template<typename Scalar, typename El, bool O>
template<typename Element>
void PrincipalCurvature<Scalar, El, O>::importFrom(const Element& e)
{
	if constexpr (HasPrincipalCurvature<Element>) {
		if (isPrincipalCurvatureEnabledOn(e)) {
			principalCurvature() = e.principalCurvature().template cast<Scalar>();
		}
	}
}

template <typename T>
bool isPrincipalCurvatureEnabledOn(const T& element)
{
	if constexpr (HasOptionalPrincipalCurvature<T>) {
		return element.isPrincipalCurvatureEnabled();
	}
	else {
		return HasPrincipalCurvature<T>;
	}
}

} // namespace vcl::comp
