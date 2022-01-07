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

#include "optional_tex_coord.h"

namespace vcl::comp {

template<typename Scalar, typename T>
const typename OptionalTexCoord<Scalar, T>::TexCoordType&
OptionalTexCoord<Scalar, T>::texCoord() const
{
	return B::optCont().texCoord(thisId());
}

template<typename Scalar, typename T>
typename OptionalTexCoord<Scalar, T>::TexCoordType& OptionalTexCoord<Scalar, T>::texCoord()
{
	return B::optCont().texCoord(thisId());
}

template<typename Scalar, typename T>
bool OptionalTexCoord<Scalar, T>::isTexCoordEnabled() const
{
	if (B::contPtr != nullptr)
		return B::optCont().isTexCoordEnabled();
	else
		return false;
}

template<typename Scalar, typename T>
template<typename Element>
void OptionalTexCoord<Scalar, T>::importFrom(const Element& e)
{
	if constexpr (hasTexCoord<Element>()) {
		if (isTexCoordEnabled() && isTexCoordEnabledOn(e)) {
			texCoord() = e.texCoord();
		}
	}
}

} // namespace vcl::comp
