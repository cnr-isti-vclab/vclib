/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_VERTICES_H
#define VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_VERTICES_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/**
 * @brief HasAdjacentVertices concept
 *
 * This concept is satisfied only if a class has a member function 'adjVerticesNumber()' which
 * returns an uint
 */
template<typename T>
concept HasAdjacentVertices = requires(T o)
{
	{ o.adjVerticesNumber() } -> std::same_as<uint>;
};

/**
 * @brief HasAdjacentVerticesComponent concept
 *
 * This concept is used to discriminate between the AdjacentVertices (or OptionalAdjacentVertices)
 * component, and the VertexHalfEdgeReferences component, which using half edges allows to access
 * to adjacent vertices. This concept is intended only for internal use, useful to check that a
 * Vertex does not have both AdjacentVertices and VertexHalfEdgeReferences components.
 */
template<typename T>
concept HasAdjacentVerticesComponent = requires(T o)
{
	HasAdjacentVertices<T>;
	{ o.__compAdjVertices() } -> std::same_as<void>;
};

/**
 * @brief HasOptionalAdjacentVertices concept
 *
 * This concept is satisfied only if a class has two member functions:
 * - 'adjVerticesNumber()' which returns an uint
 * - '__optionalAdjVertices()'
 */
template<typename T>
concept HasOptionalAdjacentVertices = HasAdjacentVertices<T> && requires(T o)
{
	{ o.__optionalAdjVertices() } -> std::same_as<void>;
};

/* Detector function to check if a class has AdjacentVertices enabled */

template <typename T>
bool isAdjacentVerticesEnabledOn(const T& element)
{
	if constexpr (HasOptionalAdjacentVertices<T>) {
		return element.isAdjVerticesEnabled();
	}
	else {
		return HasAdjacentVertices<T>;
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_VERTICES_H
