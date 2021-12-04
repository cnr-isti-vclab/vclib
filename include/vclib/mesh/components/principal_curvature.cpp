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

#include "principal_curvature.h"

namespace vcl::comp {

template<typename Scalar>
const typename PrincipalCurvature<Scalar>::PrincipalCurvatureType&
PrincipalCurvature<Scalar>::principalCurvature() const
{
	return princCurv;
}

template<typename Scalar>
typename PrincipalCurvature<Scalar>::PrincipalCurvatureType&
PrincipalCurvature<Scalar>::principalCurvature()
{
	return princCurv;
}

template<typename Scalar>
template<typename Element>
void PrincipalCurvature<Scalar>::importFrom(const Element& e)
{
	if constexpr (hasPrincipalCurvature<Element>()) {
		if (isPrincipalCurvatureEnabledOn(e)) {
			princCurv = e.principalCurvature();
		}
	}
}

} // namespace vcl::comp
