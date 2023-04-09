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

#ifndef VCL_CONCEPTS_MESH_COMPONENT_MARK_H
#define VCL_CONCEPTS_MESH_COMPONENT_MARK_H

#include "component.h"

namespace vcl::comp {

/**
 * @brief HasMark concept is satisfied only if a Element/Mesh class provides the types and member
 * functions specified in this concept. These types and member functions allow to access to a
 * Mark component of a given element/mesh.
 *
 * Note that this concept does not discriminate between the Horizontal Mark component
 * and the vertical OptionalMark component, therefore it does not guarantee that a
 * template Element type that satisfies this concept provides Mark component at runtime
 * (it is guaranteed only that the proper member functions are available at compile time).
 *
 * To be completely sure that Color is available at runtime, you need to call the member
 * function `isMarkEnabled()`.
 */
template<typename T>
concept HasMark = requires(
	T o,
	const T& co)
{
	typename T::MarkComponent;
	{ co.mark() } -> std::same_as<int>;
	{ o.resetMark() } -> std::same_as<void>;
	{ o.incrementMark() } -> std::same_as<void>;
	{ o.decrementMark() } -> std::same_as<void>;
	{ co.isMarkEnabled() } -> std::same_as<bool>;
};

/**
 * @brief HasOptionalMark concept is satisfied only if a class satisfis the HasMark concept and
 * and the static boolean constant IS_OPTIONAL is set to true.
 */
template<typename T>
concept HasOptionalMark = HasMark<T> && IsOptionalComponent<typename T::MarkComponent>;

/* Detector function to check if a class has Mark enabled */

template <typename T>
bool isMarkEnabledOn(const T& element)
{
	if constexpr (HasOptionalMark<T>) {
		return element.isMarkEnabled();
	}
	else {
		return HasMark<T>;
	}
}

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENT_MARK_H
