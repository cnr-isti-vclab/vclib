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

#include "name.h"

namespace vcl::comp {

/**
 * @brief Returns the name of this object.
 * @return The name of this object.
 */
template<typename El, bool h>
std::string& Name<El, h>::name()
{
	return n();
}

/**
 * @brief Returns the name of this object.
 * @return The name of this object.
 */
template<typename El, bool h>
const std::string& Name<El, h>::name() const
{
	return n();
}

template<typename El, bool h>
template<typename Element>
void Name<El, h>::importFrom(const Element &e)
{
	if constexpr(HasName<Element>) {
		n() = e.name();
	}
}

template<typename El, bool h>
std::string &Name<El, h>::n()
{
	if constexpr (h) {
		return data.n;
	}
	else {
		return internal::getVerticalComponentData(this);
	}
}

template<typename El, bool h>
const std::string &Name<El, h>::n() const
{
	if constexpr (h) {
		return data.n;
	}
	else {
		return internal::getVerticalComponentData(this);
	}
}

} // namespace vcl::comp
