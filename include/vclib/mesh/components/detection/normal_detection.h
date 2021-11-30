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

#ifndef VCL_MESH_COMPONENTS_NORMAL_DETECTION_H
#define VCL_MESH_COMPONENTS_NORMAL_DETECTION_H

#include <vclib/misc/vcl_types.h>

namespace vcl::comp {

/* Triggerers */

class NormalTriggerer
{
};

class OptionalNormalTriggerer
{
};

/* Detector to check if a class has (inherits) Normal */

template<typename T>
using hasNormalT = std::is_base_of<NormalTriggerer, T>;

template<typename T>
bool constexpr hasNormal()
{
	return hasNormalT<T>::value;
}

/* Detector to check if a class has (inherits) OpionalNormal */

template<typename T>
using hasOptionalNormalT = std::is_base_of<OptionalNormalTriggerer, T>;

template<typename T>
bool constexpr hasOptionalNormal()
{
	return hasOptionalNormalT<T>::value;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_NORMAL_DETECTION_H
