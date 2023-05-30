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

template<typename El, bool O>
bool Color<El, O>::isEnabled() const
{
	return Base::isEnabled(this);
}

template<typename El, bool O>
bool Color<El, O>::isColorEnabled() const
{
	return isEnabled();
}

/**
 * @brief Returns const reference of the color of the element.
 */
template<typename El, bool O>
const vcl::Color& Color<El, O>::color() const
{
	return c();
}

/**
 * @brief Returns a reference pf the color of the element.
 */
template<typename El, bool O>
vcl::Color& Color<El, O>::color()
{
	return c();
}

template<typename El, bool O>
template<typename Element>
void Color<El, O>::importFrom(const Element& e)
{
	if constexpr (HasColor<Element>) {
		if (isColorEnabledOn(e)) {
			c() = e.color();
		}
	}
}

template<typename El, bool O>
vcl::Color& Color<El, O>::c()
{
	return Base::data(this);
}

template<typename El, bool O>
const vcl::Color& Color<El, O>::c() const
{
	return Base::data(this);
}

template <typename T>
bool isColorEnabledOn(const T& element)
{
	return isComponentEnabledOn<COLOR>(element);
}

} // namespace vcl::comp
