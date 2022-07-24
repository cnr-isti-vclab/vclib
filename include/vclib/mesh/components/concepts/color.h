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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_COLOR_H
#define VCL_MESH_COMPONENTS_CONCEPTS_COLOR_H

#include <vclib/misc/types.h>
#include <vclib/space/color.h>

namespace vcl::comp {

/**
 * @brief HasColor concept
 *
 * This concept is satisfied only if a class has a member function 'color()' which returns a
 * vcl::Color&
 */
template<typename T>
concept HasColor = requires(T o)
{
	typename T::ColorType;
	{ o.color() } -> std::same_as<vcl::Color&>;
};

/**
 * @brief HasOptionalColor concept
 *
 * This concept is satisfied only if a class has two member functions:
 * - 'color()' which returns a vcl::Color&
 * - '__optionalColor()'
 */
template<typename T>
concept HasOptionalColor = HasColor<T> && requires(T o)
{
	{ o.__optionalColor() } -> std::same_as<void>;
};

/* Detector function to check if a class has Color enabled */

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

#endif // VCL_MESH_COMPONENTS_CONCEPTS_COLOR_H
