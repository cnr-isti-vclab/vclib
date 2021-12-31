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

#ifndef VCL_MESH_COMPONENTS_ADJACENT_EDGES_DETECTION_H
#define VCL_MESH_COMPONENTS_ADJACENT_EDGES_DETECTION_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/* Triggerers */

class AdjacentEdgesTriggerer
{
};

class OptionalAdjacentEdgesTriggerer
{
};

/* Detectors to check if a class has (inherits) AdjacenctEdges or OptionalAdjacentEdges */

template<typename T>
using hasAdjacentEdgesT = std::is_base_of<AdjacentEdgesTriggerer, T>;

template<typename T>
using hasOptionalAdjacentEdgesT = std::is_base_of<OptionalAdjacentEdgesTriggerer, T>;

template<typename T>
bool constexpr hasAdjacentEdges()
{
	return hasAdjacentEdgesT<T>::value || hasOptionalAdjacentEdgesT<T>::value;
}

template<typename T>
bool constexpr hasOptionalAdjacentEdges()
{
	return hasOptionalAdjacentEdgesT<T>::value;
}

template <typename T>
bool isAdjacentEdgesEnabledOn(const T& element)
{
	if constexpr (hasOptionalAdjacentEdges<T>()) {
		return element.isAdjEdgesEnabled();
	}
	else {
		return hasAdjacentEdges<T>();
	}
}

template<typename T>
bool constexpr sanityCheckAdjacentEdges()
{
	if constexpr (hasAdjacentEdges<T>()) {
		return T::VERTEX_NUMBER == T::ADJ_EDGE_NUMBER;
	}
	else {
		return true;
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_ADJACENT_EDGES_DETECTION_H
