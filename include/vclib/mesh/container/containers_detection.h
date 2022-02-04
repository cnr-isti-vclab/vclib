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

#include "../components/vertical/vertical_info.h"

namespace vcl::mesh {

/* Triggerers */

class ElementContainerTriggerer
{
};

class EdgeContainerTriggerer
{
};

class FaceContainerTriggerer
{
};

class HalfEdgeContainerTriggerer
{
};

class VertexContainerTriggerer
{
};

/* Detector to check if a class is (inherits) a Container */

template<typename T>
using isContainerT = std::is_base_of<ElementContainerTriggerer, T>;

template<typename T>
constexpr bool isContainer()
{
	return isContainerT<T>::value;
}

/* Detector to check if a class has (inherits) an EdgeContainer */

template<typename T>
using hasEdgeContainerT = std::is_base_of<EdgeContainerTriggerer, T>;

template<typename T>
constexpr bool hasEdges()
{
	return hasEdgeContainerT<T>::value;
}

template<typename T>
constexpr bool hasEdgeOptionalContainer()
{
	if constexpr (hasEdges<T>()) {
		return comp::hasVerticalInfo<typename T::EdgeType>();
	}
	else {
		return false;
	}
}

/* Detector to check if a class has (inherits) a FaceContainer */

template<typename T>
using hasFaceContainerT = std::is_base_of<FaceContainerTriggerer, T>;

template<typename T>
constexpr bool hasFaces()
{
	return hasFaceContainerT<T>::value;
}

template<typename T>
constexpr bool hasFaceOptionalContainer()
{
	if constexpr (hasFaces<T>()) {
		return comp::hasVerticalInfo<typename T::FaceType>();
	}
	else {
		return false;
	}
}

/* Detector to check if a class has (inherits) an HalfEdgeContainer */

template<typename T>
using hasHalfEdgeContainerT = std::is_base_of<HalfEdgeContainerTriggerer, T>;

template<typename T>
constexpr bool hasHalfEdges()
{
	return hasHalfEdgeContainerT<T>::value;
}

template<typename T>
constexpr bool hasHalfEdgeOptionalContainer()
{
	if constexpr (hasHalfEdges<T>()) {
		return comp::hasVerticalInfo<typename T::HalfEdgeType>();
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
		return comp::hasVerticalInfo<typename T::VertexType>();
	}
	else {
		return false;
	}
}

} // namespace vcl::mesh

#endif // VCL_MESH_CONTAINERS_DETECTION_H
