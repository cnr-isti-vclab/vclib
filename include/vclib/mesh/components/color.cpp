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

#include "color.h"

namespace vcl::comp {

/**
 * @brief Returns const reference of the color of the element.
 */
const vcl::Color& Color::color() const
{
	return c;
}

/**
 * @brief Returns a reference pf the color of the element.
 */
vcl::Color& Color::color()
{
	return c;
}

template<typename Element>
void Color::importFrom(const Element& e)
{
	if constexpr (hasColor<Element>()) {
		if (isColorEnabled(e)) {
			c = e.color();
		}
	}
}

} // namespace vcl::comp
