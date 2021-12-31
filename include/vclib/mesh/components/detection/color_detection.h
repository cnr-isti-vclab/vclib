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

#include <vclib/misc/types.h>

#ifndef VCL_MESH_COMPONENTS_COLOR_DETECTION_H
#define VCL_MESH_COMPONENTS_COLOR_DETECTION_H

namespace vcl::comp {

class Color;
template<typename T>
class OptionalColor;

/* Detector to check if a class has (inherits) Color or OptionalColor */

template<typename T>
using hasColorT = std::is_base_of<Color, T>;

template<typename T>
using hasOptionalColorT = std::is_base_of<OptionalColor<T>, T>;

template<typename T>
constexpr bool hasColor()
{
	return hasColorT<T>::value || hasOptionalColorT<T>::value;
}

template<typename T>
bool constexpr hasOptionalColor()
{
	return hasOptionalColorT<T>::value;
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

#endif // VCL_MESH_COMPONENTS_COLOR_DETECTION_H
