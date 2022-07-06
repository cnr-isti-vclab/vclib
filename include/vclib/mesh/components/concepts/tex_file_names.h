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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_TEX_FILE_NAMES_H
#define VCL_MESH_COMPONENTS_CONCEPTS_TEX_FILE_NAMES_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/**
 * @brief HasTexFileNames concept
 *
 * This concept is satisfied only if a class has a member function 'textureNumber()' which returns
 * an uint
 */
template<typename T>
concept HasTexFileNames = requires(T v) // requires that an object of type T has the following members
{
	{ v.textureNumber() } -> std::same_as<uint>;
};

/* Detector functions to check if a class has TexFileNames */

template<typename T>
constexpr bool hasTexFileNames()
{
	return HasTexFileNames<T>;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_TEX_FILE_NAMES_H
