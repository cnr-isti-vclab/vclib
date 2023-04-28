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

template<typename Scalar, typename El, bool o>
bool PrincipalCurvature<Scalar, El, o>::isEnabled() const
{
	return data.template isComponentEnabled<El>(this);
}

template<typename Scalar, typename El, bool o>
bool PrincipalCurvature<Scalar, El, o>::isPrincipalCurvatureEnabled() const
{
	return isEnabled();
}

template<typename Scalar, typename El, bool o>
const typename PrincipalCurvature<Scalar, El, o>::PrincipalCurvatureType&
PrincipalCurvature<Scalar, El, o>::principalCurvature() const
{
	return princCurv();
}

template<typename Scalar, typename El, bool o>
typename PrincipalCurvature<Scalar, El, o>::PrincipalCurvatureType&
PrincipalCurvature<Scalar, El, o>::principalCurvature()
{
	return princCurv();
}

template<typename Scalar, typename El, bool o>
template<typename Element>
void PrincipalCurvature<Scalar, El, o>::importFrom(const Element& e)
{
	if constexpr (HasPrincipalCurvature<Element>) {
		if (isPrincipalCurvatureEnabledOn(e)) {
			princCurv() = e.principalCurvature().template cast<Scalar>();
		}
	}
}

template<typename Scalar, typename El, bool o>
typename PrincipalCurvature<Scalar, El, o>::PrincipalCurvatureType&
PrincipalCurvature<Scalar, El, o>::princCurv()
{
	return data.template get<El>(this);
}

template<typename Scalar, typename El, bool o>
const typename PrincipalCurvature<Scalar, El, o>::PrincipalCurvatureType&
PrincipalCurvature<Scalar, El, o>::princCurv() const
{
	return data.template get<El>(this);
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
