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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_TEX_COORD_H
#define VCL_MESH_COMPONENTS_CONCEPTS_TEX_COORD_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/**
 * @brief HasTexCoord concept
 *
 * This concept is satisfied only if a class has a member function that 'texCoord()'
 */
template<typename T>
concept HasTexCoord = requires(T o)
{
	o.texCoord();
};

/**
 * @brief HasOptionalTexCoord concept
 *
 * This concept is satisfied only if a class has two member functions:
 * - 'texCoord()' which returns an int&
 * - '__optionalTexCoord()'
 */
template<typename T>
concept HasOptionalTexCoord = HasTexCoord<T> && requires(T o)
{
	{ o.__optionalTexCoord() } -> std::same_as<void>;
};

/* Detector functions to check if a class has TexCoord or OptionalTexCoord */

template<typename T>
bool constexpr hasTexCoord()
{
	return HasTexCoord<T>;
}

template<typename T>
bool constexpr hasOptionalTexCoord()
{
	return HasOptionalTexCoord<T>;
}

template <typename T>
bool isTexCoordEnabledOn(const T& element)
{
	if constexpr (hasOptionalTexCoord<T>()) {
		return element.isTexCoordEnabled();
	}
	else {
		return hasTexCoord<T>();
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_TEX_COORD_H
