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

#ifndef VCL_MESH_COMPONENTS_TEX_COORD_DETECTION_H
#define VCL_MESH_COMPONENTS_TEX_COORD_DETECTION_H

#include <vclib/misc/vcl_types.h>

namespace vcl::comp {

/* Triggerers */

class TexCoordTriggerer
{
};

class OptionalTexCoordTriggerer
{
};

/* Detector to check if a class has (inherits) TexCoord */

template<typename T>
using hasTexCoordT = std::is_base_of<TexCoordTriggerer, T>;

template<typename T>
bool constexpr hasTexCoord()
{
	return hasTexCoordT<T>::value;
}

/* Detector to check if a class has (inherits) OptionalTexCoord */

template<typename T>
using hasOptionalTexCoordT = std::is_base_of<OptionalTexCoordTriggerer, T>;

template<typename T>
bool constexpr hasOptionalTexCoord()
{
	return hasOptionalTexCoordT<T>::value;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_TEX_COORD_DETECTION_H
