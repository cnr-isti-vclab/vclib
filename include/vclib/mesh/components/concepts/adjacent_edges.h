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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_EDGES_H
#define VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_EDGES_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/**
 * @brief HasAdjacentEdges concept
 *
 * This concept is satisfied only if a class has a member function 'adjEdgesNumber()' which returns
 * an uint
 */
template<typename T>
concept HasAdjacentEdges = requires(T v) // requires that an object of type T has the following members
{
	{ v.adjEdgesNumber() } -> std::same_as<uint>;
};

/**
 * @brief HasOptionalAdjacentEdges concept
 *
 * This concept is satisfied only if a class has two member functions:
 * - 'adjEdgesNumber()' which returns an uint
 * - 'isAdjEdgesEnabled()' which returns a bool
 */
template<typename T>
concept HasOptionalAdjacentEdges = HasAdjacentEdges<T> && requires(T v)
{
	{ v.isAdjEdgesEnabled() } -> std::same_as<bool>;
};

/* Detector functions to check if a class has AdjacentEdges or OptionalAdjacentEdges */

template<typename T>
bool constexpr hasAdjacentEdges()
{
	return HasAdjacentEdges<T>;
}

template<typename T>
bool constexpr hasOptionalAdjacentEdges()
{
	return HasOptionalAdjacentEdges<T>;
}

template <typename T>
bool isAdjacentEdgesEnabledOn(const T& element)
{
	if constexpr (HasOptionalAdjacentEdges<T>) {
		return element.isAdjEdgesEnabled();
	}
	else {
		return HasAdjacentEdges<T>;
	}
}

template<typename T>
bool constexpr sanityCheckAdjacentEdges()
{
	if constexpr (hasAdjacentEdges<T>()) {
		return T::VERTEX_NUMBER == T::ADJ_EDGE_NUMBER;
	}
	else {
		return true;
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_EDGES_H
