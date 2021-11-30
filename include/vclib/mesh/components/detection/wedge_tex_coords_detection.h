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

#ifndef VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_DETECTION_H
#define VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_DETECTION_H

#include <vclib/misc/vcl_types.h>

namespace vcl::comp {

/* Triggerers */

class WedgeTexCoordsTriggerer
{
};

class OptionalWedgeTexCoordsTriggerer
{
};

/* Detector to check if a class has (inherits) WedgeTexCoords */

template<typename T>
using hasWedgeTexCoordsT = std::is_base_of<WedgeTexCoordsTriggerer, T>;

template<typename T>
bool constexpr hasWedgeTexCoords()
{
	return hasWedgeTexCoordsT<T>::value;
}

template<typename T>
bool constexpr sanityCheckWedgeTexCoords()
{
	if constexpr (hasWedgeTexCoords<T>()) {
		return T::VERTEX_NUMBER == T::WEDGE_TEX_COORD_NUMBER;
	}
	else {
		return true;
	}
}

/* Detector to check if a class has (inherits) OptionalWedgeTexCoords */

template<typename T>
using hasOptionalWedgeTexCoordsT = std::is_base_of<OptionalWedgeTexCoordsTriggerer, T>;

template<typename T>
bool constexpr hasOptionalWedgeTexCoords()
{
	return hasOptionalWedgeTexCoordsT<T>::value;
}

template<typename T>
bool constexpr sanityCheckOptionalWedgeTexCoords()
{
	if constexpr (hasOptionalWedgeTexCoords<T>()) {
		return T::VERTEX_NUMBER == T::WEDGE_TEX_COORD_NUMBER;
	}
	else {
		return true;
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_DETECTION_H
