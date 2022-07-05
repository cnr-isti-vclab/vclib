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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_FACES_H
#define VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_FACES_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/**
 * @brief HasAdjacentFaces concept
 *
 * This concept is satisfied only if a class has a member function 'adjFacesNumber()' which returns
 * an uint
 */
template<typename T>
concept HasAdjacentFaces = requires(T v) // requires that an object of type T has the following members
{
	{ v.adjFacesNumber() } -> std::same_as<uint>;
};

/**
 * @brief HasOptionalAdjacentFaces concept
 *
 * This concept is satisfied only if a class has two member functions:
 * - 'adjFacesNumber()' which returns an uint
 * - 'isAdjFacesEnabled()' which returns a bool
 */
template<typename T>
concept HasOptionalAdjacentFaces = HasAdjacentFaces<T> && requires(T v)
{
	{ v.isAdjFacesEnabled() } -> std::same_as<bool>;
};

/* Detector functions to check if a class has AdjacentFaces or OptionalAdjacentFaces */

template<typename T>
bool constexpr hasAdjacentFaces()
{
	return HasAdjacentFaces<T>;
}

template<typename T>
bool constexpr hasOptionalAdjacentFaces()
{
	return HasOptionalAdjacentFaces<T>;
}

template <typename T>
bool isAdjacentFacesEnabledOn(const T& element)
{
	if constexpr (HasOptionalAdjacentFaces<T>) {
		return element.isAdjFacesEnabled();
	}
	else {
		return HasAdjacentFaces<T>;
	}
}

template<typename T>
bool constexpr sanityCheckAdjacentFaces()
{
	if constexpr (hasAdjacentFaces<T>()) {
		return T::VERTEX_NUMBER == T::ADJ_FACE_NUMBER;
	}
	else {
		return true;
	}
}

}

#endif // VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_FACES_H
