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

#ifndef VCL_MESH_COMPONENTS_MARK_DETECTION_H
#define VCL_MESH_COMPONENTS_MARK_DETECTION_H

#include <vclib/misc/types.h>

namespace vcl::comp {

class Mark;
template<typename T>
class OptionalMark;

/* Detector to check if a class has (inherits) Mark or OptionalMark */

template<typename T>
using hasMarkT = std::is_base_of<Mark, T>;

template<typename T>
using hasOptionalMarkT = std::is_base_of<OptionalMark<T>, T>;

template<typename T>
constexpr bool hasMark()
{
	return hasMarkT<T>::value || hasOptionalMarkT<T>::value;
}

template<typename T>
bool constexpr hasOptionalMark()
{
	return hasOptionalMarkT<T>::value;
}

template <typename T>
bool isMarkEnabledOn(const T& element)
{
	if constexpr (hasOptionalMark<T>()) {
		return element.isMarkEnabled();
	}
	else {
		return hasMark<T>();
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_MARK_DETECTION_H
