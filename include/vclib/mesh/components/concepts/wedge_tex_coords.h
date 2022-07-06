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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_WEDGE_TEX_COORDS_H
#define VCL_MESH_COMPONENTS_CONCEPTS_WEDGE_TEX_COORDS_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/**
 * @brief HasWedgeTexCoords concept
 *
 * This concept is satisfied only if a class has a member function 'wedgeTexCoord(uint)'.
 */
template<typename T>
concept HasWedgeTexCoords = requires(T v) // requires that an object of type T has the following members
{
	v.wedgeTexCoord(uint());
};

/**
 * @brief HasOptionalWedgeColors concept
 *
 * This concept is satisfied only if a class has two member functions:
 * - 'wedgeTexCoord(uint)'
 * - 'isWedgeTexCoordsEnabled()' which returns a bool
 */
template<typename T>
concept HasOptionalWedgeTexCoords = HasWedgeTexCoords<T> && requires(T v)
{
	{ v.isWedgeTexCoordsEnabled() } -> std::same_as<bool>;
};

/* Detector functions to check if a class has WedgeTexCoords or OptionalWedgeTexCoords */

template<typename T>
bool constexpr hasWedgeTexCoords()
{
	return HasWedgeTexCoords<T>;
}

template<typename T>
bool constexpr hasOptionalWedgeTexCoords()
{
	return HasOptionalWedgeTexCoords<T>;
}

template <typename T>
bool isWedgeTexCoordsEnabledOn(const T& element)
{
	if constexpr (hasOptionalWedgeTexCoords<T>()) {
		return element.isWedgeTexCoordsEnabled();
	}
	else {
		return HasWedgeTexCoords<T>;
	}
}

template<typename T>
bool constexpr sanityCheckWedgeTexCoords()
{
	if constexpr (hasWedgeTexCoords<T>()) {
		return T::VERTEX_NUMBER == T::WEDGE_TEX_COORD_NUMBER;
	}
	else {
		return true;
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_WEDGE_TEX_COORDS_H
