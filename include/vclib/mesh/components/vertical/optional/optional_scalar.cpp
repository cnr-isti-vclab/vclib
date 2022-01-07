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

#include "optional_scalar.h"

namespace vcl::comp {

template<typename S, typename T>
const typename OptionalScalar<S, T>::ScalarType& OptionalScalar<S, T>::scalar() const
{
	return B::optCont().scalar(thisId());
}

template<typename S, typename T>
typename OptionalScalar<S, T>::ScalarType& OptionalScalar<S, T>::scalar()
{
	return B::optCont().scalar(thisId());
}

template<typename S, typename T>
bool OptionalScalar<S, T>::isScalarEnabled() const
{
	if (B::contPtr != nullptr)
		return B::optCont().isScalarEnabled();
	else
		return false;
}

template<typename S, typename T>
template<typename Element>
void OptionalScalar<S, T>::importFrom(const Element& e)
{
	if constexpr (hasScalar<Element>()) {
		if (isScalarEnabled() && isScalarEnabledOn(e)) {
			scalar() = e.scalar();
		}
	}
}

} // namespace vcl::comp
