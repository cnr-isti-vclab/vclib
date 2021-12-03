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

#ifndef VCL_MESH_COMPONENTS_MUTABLE_BIT_FLAGS_DETECTION_H
#define VCL_MESH_COMPONENTS_MUTABLE_BIT_FLAGS_DETECTION_H

#include <vclib/misc/vcl_types.h>

namespace vcl::comp {

class MutableBitFlags;
template<typename T>
class OptionalMutableBitFlags;

/* Detector to check if a class has (inherits) MutableBitFlags or OptionalMutableBitFlags*/

template<typename T>
using hasMutableBitFlagsT = std::is_base_of<MutableBitFlags, T>;

template<typename T>
using hasOptionalMutableBitFlagsT = std::is_base_of<OptionalMutableBitFlags<T>, T>;

template<typename T>
bool constexpr hasMutableBitFlags()
{
	return hasMutableBitFlagsT<T>::value || hasOptionalMutableBitFlagsT<T>::value;
}

template<typename T>
bool constexpr hasOptionalMutableBitFlags()
{
	return hasOptionalMutableBitFlagsT<T>::value;
}

template <typename T>
bool isMutableBitFlagsEnabledOn(const T& element)
{
	if constexpr(hasOptionalMutableBitFlags<T>()) {
		return element.isMutableBitFlagsEnabled();
	}
	else {
		return hasMutableBitFlags<T>();
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_MUTABLE_BIT_FLAGS_DETECTION_H
