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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_COLOR__H
#define VCL_MESH_COMPONENTS_CONCEPTS_COLOR__H

#include <vclib/misc/types.h>
#include <vclib/space/color.h>

namespace vcl::comp {

/**
 * @brief HasColor concept
 *
 * This concept is satisfied only if a class has a member function that 'color()' which returns a
 * vcl::Color&
 */
template<typename T>
concept HasColor = requires(T v) // requires that an object of type T has the following members
{
	{ v.color() } -> std::same_as<vcl::Color&>;
};

/**
 * @brief HasOptionalColor concept
 *
 * This concept is satisfied only if a class has two member functions:
 * - 'color()' which returns a vcl::Color&
 * - 'isColorEnabled()' which returns a bool
 */
template<typename T>
concept HasOptionalColor = HasColor<T> && requires(T v)
{
	{ v.isColorEnabled() } -> std::same_as<bool>;
};

/* Detector functions to check if a class has Color or OptionalColor */

template<typename T>
constexpr bool hasColor()
{
	return HasColor<T> || HasOptionalColor<T>;
}

template<typename T>
bool constexpr hasOptionalColor()
{
	return HasOptionalColor<T>;
}

template <typename T>
bool isColorEnabledOn(const T& element)
{
	if constexpr (hasOptionalColor<T>()) {
		return element.isColorEnabled();
	}
	else {
		return hasColor<T>();
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_COLOR__H
