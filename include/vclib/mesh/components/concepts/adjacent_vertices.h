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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_VERTICES_H
#define VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_VERTICES_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/**
 * @brief HasAdjacentVertices concept
 *
 * This concept is satisfied only if a class has a member function 'adjVerticesNumber()' which
 * returns an uint
 */
template<typename T>
concept HasAdjacentVertices = requires(T v) // requires that an object of type T has the following members
{
	{ v.adjVerticesNumber() } -> std::same_as<uint>;
};

/**
 * @brief HasOptionalAdjacentVertices concept
 *
 * This concept is satisfied only if a class has two member functions:
 * - 'adjVerticesNumber()' which returns an uint
 * - '__optionalAdjVertices()'
 */
template<typename T>
concept HasOptionalAdjacentVertices = HasAdjacentVertices<T> && requires(T v)
{
	{ v.__optionalAdjVertices() } -> std::same_as<void>;
};

/* Detector functions to check if a class has AdjacentVertices or OptionalAdjacentVertices */

template<typename T>
bool constexpr hasAdjacentVertices()
{
	return HasAdjacentVertices<T>;
}

template<typename T>
bool constexpr hasOptionalAdjacentVertices()
{
	return HasOptionalAdjacentVertices<T>;
}

template <typename T>
bool isAdjacentVerticesEnabledOn(const T& element)
{
	if constexpr (hasOptionalAdjacentVertices<T>()) {
		return element.isAdjVerticesEnabled();
	}
	else {
		return hasAdjacentVertices<T>();
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_VERTICES_H
