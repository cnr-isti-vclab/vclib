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

#ifndef VCL_MESH_CONTAINERS_DETECTION_H
#define VCL_MESH_CONTAINERS_DETECTION_H

#include <vclib/misc/types.h>

#include "../components_optional/optional_info.h"

namespace vcl::mesh {

/* Triggerers */

class EdgeContainerTriggerer
{
};

class FaceContainerTriggerer
{
};

class VertexContainerTriggerer
{
};

/* Detector to check if a class has (inherits) an EdgeContainer */

template<typename T>
using hasEdgeContainer = std::is_base_of<EdgeContainerTriggerer, T>;

template<typename T>
constexpr bool hasEdges()
{
	return hasEdgeContainer<T>::value;
}

template<typename T>
constexpr bool hasEdgeOptionalContainer()
{
	if constexpr (hasEdges<T>()) {
		return comp::hasOptionalInfo<typename T::EdgeType>();
	}
	else {
		return false;
	}
}

/* Detector to check if a class has (inherits) a FaceContainer */

template<typename T>
using hasFaceContainer = std::is_base_of<FaceContainerTriggerer, T>;

template<typename T>
constexpr bool hasFaces()
{
	return hasFaceContainer<T>::value;
}

template<typename T>
constexpr bool hasFaceOptionalContainer()
{
	if constexpr (hasFaces<T>()) {
		return comp::hasOptionalInfo<typename T::FaceType>();
	}
	else {
		return false;
	}
}

/* Detector to check if a class has (inherits) a VertexContainer */

template<typename T>
using hasVertexContainerT = std::is_base_of<VertexContainerTriggerer, T>;

template<typename T>
constexpr bool hasVertices()
{
	return hasVertexContainerT<T>::value;
}

template<typename T>
constexpr bool hasVertexOptionalContainer()
{
	if constexpr (hasVertices<T>()) {
		return comp::hasOptionalInfo<typename T::VertexType>();
	}
	else {
		return false;
	}
}

} // namespace vcl::mesh

#endif // VCL_MESH_CONTAINERS_DETECTION_H
