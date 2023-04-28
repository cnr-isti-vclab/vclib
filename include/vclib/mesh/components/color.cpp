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

#include "color.h"

namespace vcl::comp {

template<typename El, bool o>
bool Color<El, o>::isEnabled() const
{
	return data.template isComponentEnabled<El>(this);
}

template<typename El, bool o>
bool Color<El, o>::isColorEnabled() const
{
	return isEnabled();
}

/**
 * @brief Returns const reference of the color of the element.
 */
template<typename El, bool o>
const vcl::Color& Color<El, o>::color() const
{
	return c();
}

/**
 * @brief Returns a reference pf the color of the element.
 */
template<typename El, bool o>
vcl::Color& Color<El, o>::color()
{
	return c();
}

template<typename El, bool o>
template<typename Element>
void Color<El, o>::importFrom(const Element& e)
{
	if constexpr (HasColor<Element>) {
		if (isColorEnabledOn(e)) {
			c() = e.color();
		}
	}
}

template<typename El, bool o>
vcl::Color& Color<El, o>::c()
{
	return data.template get<El>(this);
}

template<typename El, bool o>
const vcl::Color& Color<El, o>::c() const
{
	return data.template get<El>(this);
}

template <typename T>
bool isColorEnabledOn(const T& element)
{
	if constexpr (HasOptionalColor<T>) {
		return element.isColorEnabled();
	}
	else {
		return HasColor<T>;
	}
}

} // namespace vcl::comp
