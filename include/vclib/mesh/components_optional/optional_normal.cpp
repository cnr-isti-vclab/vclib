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

#include "optional_normal.h"

namespace vcl::comp {

template<typename Scalar, int N, typename T>
const typename OptionalNormal<Scalar, N, T>::NormalType&
OptionalNormal<Scalar, N, T>::normal() const
{
	return B::optCont().normal(thisId());
}

template<typename Scalar, int N, typename T>
typename OptionalNormal<Scalar, N, T>::NormalType& OptionalNormal<Scalar, N, T>::normal()
{
	return B::optCont().normal(thisId());
}

template<typename Scalar, int N, typename T>
bool OptionalNormal<Scalar, N, T>::isNormalEnabled() const
{
	if (B::contPtr != nullptr)
		return B::optCont().isNormalEnabled();
	else
		return false;
}

template<typename Scalar, int N, typename T>
template<typename Element>
void OptionalNormal<Scalar, N, T>::importFrom(const Element& e)
{
	if constexpr (hasNormal<Element>()) {
		if (isNormalEnabled() && isNormalEnabled(e)) {
			normal() = e.normal();
		}
	}
}

} // namespace vcl::comp
