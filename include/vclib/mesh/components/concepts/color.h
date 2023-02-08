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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_COLOR_H
#define VCL_MESH_COMPONENTS_CONCEPTS_COLOR_H

#include <vclib/space/color.h>

#include "component.h"

namespace vcl::comp {

/**
 * @brief HasColor concept is satisfied only if a Element/Mesh class provides the types and member
 * functions specified in this concept. These types and member functions allow to access to a
 * Color component of a given element/mesh.
 *
 * Note that this concept does not discriminate between the Horizontal Color component
 * and the vertical OptionalColor component, therefore it does not guarantee that a
 * template Element type that satisfies this concept provides Color component at runtime
 * (it is guaranteed only that the proper member functions are available at compile time).
 *
 * To be completely sure that Color is available at runtime, you need to call the member
 * function `isColorEnabled()`.
 */
template<typename T>
concept HasColor = requires(
	T o,
	const T& co)
{
	typename T::ColorType;
	typename T::ColorComponent;
	{ o.color() } -> std::same_as<vcl::Color&>;
	{ co.color() } -> std::same_as<const vcl::Color&>;
	{ co.isColorEnabled() } -> std::same_as<bool>;
};

/**
 * @brief HasOptionalColor concept is satisfied only if a class satisfis the HasColor concept and
 * has the additional member function '__optionalColor()', which is the discriminator between the
 * non-optional and optional component.
 */
template<typename T>
concept HasOptionalColor = HasColor<T> && IsOptionalComponent<typename T::ColorComponent>;

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
