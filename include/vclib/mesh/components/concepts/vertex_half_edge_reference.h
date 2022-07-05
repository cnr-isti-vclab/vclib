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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_VERTEX_HALF_EDGE_REFERENCE_H
#define VCL_MESH_COMPONENTS_CONCEPTS_VERTEX_HALF_EDGE_REFERENCE_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/**
 * @brief HasVertexHalfEdgeReference concept
 *
 * This concept is satisfied only if a class has the following member functions:
 * - halfEdge()
 */
template<typename T>
concept HasVertexHalfEdgeReference = requires(T v) // requires that an object of type T has the following members
{
	v.halfEdge();
};

/* Detector to check if a class has VertexHalfEdgeReference */

template<typename T>
bool constexpr hasVertexHalfEdgeReference()
{
	return HasVertexHalfEdgeReference<T>;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_VERTEX_HALF_EDGE_REFERENCE_H
