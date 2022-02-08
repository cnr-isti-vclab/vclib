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

#ifndef VCL_MESH_COMPONENTS_BIT_FLAGS_DETECTION_H
#define VCL_MESH_COMPONENTS_BIT_FLAGS_DETECTION_H

#include <vclib/misc/types.h>

namespace vcl::comp {

class BitFlags;
class PolygonBitFlags;
class TriangleBitFlags;

/* Detector to check if a class has (inherits) BitFlags */

template<typename T>
using hasBitFlagsT = std::is_base_of<BitFlags, T>;

template<typename T>
bool constexpr hasBitFlags()
{
	return hasBitFlagsT<T>::value;
}

/* Detector to check if a class has (inherits) PolygonBitFlags */

template<typename T>
using hasPolygonBitFlagsT = std::is_base_of<PolygonBitFlags, T>;

template<typename T>
bool constexpr hasPolygonBitFlags()
{
	return hasPolygonBitFlagsT<T>::value;
}

/* Detector to check if a class has (inherits) TriangleBitFlags */

template<typename T>
using hasTriangleBitFlagsT = std::is_base_of<TriangleBitFlags, T>;

template<typename T>
bool constexpr hasTriangleBitFlags()
{
	return hasTriangleBitFlagsT<T>::value;
}

/* Detector to check if a class has (inherits) TriangleBitFlags or PolygonBitFlags */

template<typename T>
bool constexpr hasFaceBitFlags()
{
	return hasTriangleBitFlagsT<T>::value || hasPolygonBitFlagsT<T>::value;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BIT_FLAGS_DETECTION_H
