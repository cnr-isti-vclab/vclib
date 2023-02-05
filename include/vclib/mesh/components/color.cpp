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

#include "color.h"

namespace vcl::comp {

/**
 * @brief Returns const reference of the color of the element.
 */
template<typename El, bool h>
const vcl::Color& Color<El, h>::color() const
{
	return c();
}

/**
 * @brief Returns a reference pf the color of the element.
 */
template<typename El, bool h>
vcl::Color& Color<El, h>::color()
{
	return c();
}

template<typename El, bool h>
template<typename Element>
void Color<El, h>::importFrom(const Element& e)
{
	if constexpr (HasColor<Element>) {
		if (isColorEnabledOn(e)) {
			c() = e.color();
		}
	}
}

template<typename El, bool h>
vcl::Color& Color<El, h>::c()
{
	if constexpr (h) {
		return data.c;
	}
	else {
		return internal::getVerticalComponentData(this);
	}
}

template<typename El, bool h>
const vcl::Color& Color<El, h>::c() const
{
	if constexpr (h) {
		return data.c;
	}
	else {
		return internal::getVerticalComponentData(this);
	}
}

} // namespace vcl::comp
