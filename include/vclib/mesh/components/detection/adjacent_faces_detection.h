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

#ifndef VCL_MESH_COMPONENTS_ADJACENT_FACES_DETECTION_H
#define VCL_MESH_COMPONENTS_ADJACENT_FACES_DETECTION_H

#include <vclib/misc/vcl_types.h>

namespace vcl::comp {

/* Triggerers */

class AdjacentFacesTriggerer
{
};

class OptionalAdjacentFacesTriggerer
{
};

/* Detectors to check if a class has (inherits) AdjacenctFaces or OptionalAdjacenctFaces */

template<typename T>
using hasAdjacentFacesT = std::is_base_of<AdjacentFacesTriggerer, T>;

template<typename T>
using hasOptionalAdjacentFacesT = std::is_base_of<OptionalAdjacentFacesTriggerer, T>;

template<typename T>
bool constexpr hasAdjacentFaces()
{
	return hasAdjacentFacesT<T>::value || hasOptionalAdjacentFacesT<T>::value;
}

template<typename T>
bool constexpr hasOptionalAdjacentFaces()
{
	return hasOptionalAdjacentFacesT<T>::value;
}

template <typename T>
bool isAdjacentFacesEnabledOn(const T& element)
{
	if constexpr(hasOptionalAdjacentFaces<T>()) {
		return element.isAdjFacesEnabled();
	}
	else {
		return hasAdjacentFaces<T>();
	}
}

template<typename T>
bool constexpr sanityCheckAdjacentFaces()
{
	if constexpr (hasAdjacentFaces<T>()) {
		return T::VERTEX_NUMBER == T::ADJ_FACE_NUMBER;
	}
	else {
		return true;
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_ADJACENT_FACES_DETECTION_H
