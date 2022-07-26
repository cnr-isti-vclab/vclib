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

template<typename P, typename T>
const typename OptionalNormalT<P, T>::NormalType&
OptionalNormalT<P, T>::normal() const
{
	return B::optCont().normal(thisId());
}

template<typename P, typename T>
typename OptionalNormalT<P, T>::NormalType& OptionalNormalT<P, T>::normal()
{
	return B::optCont().normal(thisId());
}

template<typename P, typename T>
bool OptionalNormalT<P, T>::isNormalEnabled() const
{
	if (B::contPtr != nullptr)
		return B::optCont().isNormalEnabled();
	else
		return false;
}

template<typename P, typename T>
template<typename Element>
void OptionalNormalT<P, T>::importFrom(const Element& e)
{
	if constexpr (HasNormal<Element>) {
		if (isNormalEnabled() && isNormalEnabledOn(e)) {
			normal() = e.normal().template cast<typename NormalType::ScalarType>();
		}
	}
}

} // namespace vcl::comp
