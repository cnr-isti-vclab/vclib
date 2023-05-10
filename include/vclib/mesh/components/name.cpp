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

#include "name.h"

namespace vcl::comp {

/**
 * @brief Returns the name of this object.
 * @return The name of this object.
 */
template<typename El, bool o>
std::string& Name<El, o>::name()
{
	return Base::data(this);
}

/**
 * @brief Returns the name of this object.
 * @return The name of this object.
 */
template<typename El, bool o>
const std::string& Name<El, o>::name() const
{
	return Base::data(this);
}

template<typename El, bool o>
template<typename Element>
void Name<El, o>::importFrom(const Element &e)
{
	if constexpr(HasName<Element>) {
		name() = e.name();
	}
}

} // namespace vcl::comp
