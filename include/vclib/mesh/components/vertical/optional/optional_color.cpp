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

#include "optional_color.h"

namespace vcl::comp {

template<typename T>
const vcl::Color& OptionalColor<T>::color() const
{
	return B::optCont().color(thisId());
}

template<typename T>
vcl::Color& OptionalColor<T>::color()
{
	return B::optCont().color(thisId());
}

template<typename T>
bool OptionalColor<T>::isColorEnabled() const
{
	if (B::contPtr != nullptr)
		return B::optCont().isColorEnabled();
	else
		return false;
}

template<typename T>
template<typename Element>
void OptionalColor<T>::importFrom(const Element& e)
{
	if constexpr (HasColor<Element>) {
		if (isColorEnabled() && isColorEnabledOn(e)) {
			color() = e.color();
		}
	}
}

} // namespace vcl::comp
