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

#ifndef VCL_MESH_COMPONENTS_WEDGE_COLORS_DETECTION_H
#define VCL_MESH_COMPONENTS_WEDGE_COLORS_DETECTION_H

#include <vclib/misc/vcl_types.h>

namespace vcl::comp {

/* Triggerers */

class WedgeColorsTriggerer
{
};

class OptionalWedgeColorsTriggerer
{
};

/* Detector to check if a class has (inherits) WedgeColors or OptionalWedgeColors */

template<typename T>
using hasWedgeColorsT = std::is_base_of<WedgeColorsTriggerer, T>;

template<typename T>
using hasOptionalWedgeColorsT = std::is_base_of<OptionalWedgeColorsTriggerer, T>;


template<typename T>
bool constexpr hasWedgeColors()
{
	return hasWedgeColorsT<T>::value || hasOptionalWedgeColorsT<T>::value;
}

template<typename T>
bool constexpr hasOptionalWedgeColors()
{
	return hasOptionalWedgeColorsT<T>::value;
}

template <typename T>
bool isWedgeColorsEnabledOn(const T& element)
{
	if constexpr(hasOptionalWedgeColors<T>()) {
		return element.isWedgeColorsEnabled();
	}
	else {
		return hasWedgeColors<T>();
	}
}

template<typename T>
bool constexpr sanityCheckWedgeColors()
{
	if constexpr (hasWedgeColors<T>()) {
		return T::VERTEX_NUMBER == T::WEDGE_COLOR_NUMBER;
	}
	else {
		return true;
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_WEDGE_COLORS_DETECTION_H
