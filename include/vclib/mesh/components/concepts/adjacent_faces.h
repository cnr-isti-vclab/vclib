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
concept HasAdjacentFaces = requires(T o)
{
	{ o.adjFacesNumber() } -> std::same_as<uint>;
};

/**
 * @brief HasOptionalAdjacentFaces concept
 *
 * This concept is satisfied only if a class has two member functions:
 * - 'adjFacesNumber()' which returns an uint
 * - '__optionalAdjFaces()'
 */
template<typename T>
concept HasOptionalAdjacentFaces = HasAdjacentFaces<T> && requires(T o)
{
	{ o.__optionalAdjFaces() } -> std::same_as<void>;
};

/**
 * @brief HasRightNumberOfAdjacentFaces concept
 *
 * This concept is designed to be used with Face components, where the number of adjacent faces must
 * be consisted w.r.t. the number of vertices of the face.
 *
 * This concept is satisfied only if static number of adjacent faces is the same of the static
 * number of vertices.
 */
template<typename T>
concept HasRightNumberOfAdjacentFaces = T::VERTEX_NUMBER == T::ADJ_FACE_NUMBER;

/**
 * @brief SanityCheckAdjacentFaces concept
 *
 * This concept is designed to be used with Face components, where the number of adjacent faces must
 * be consistent w.r.t. the number of vertices of the face.
 *
 * It is satisfied if:
 * - the component does *not* have adjacent faces;
 * - in case it has adjacent faces, they have the same number of vertices of the face.
 */
template<typename T>
concept SanityCheckAdjacentFaces = !HasAdjacentFaces<T> || HasRightNumberOfAdjacentFaces<T>;

/* Detector functions to check if a class has AdjacentFaces or OptionalAdjacentFaces */

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

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_FACES_H
