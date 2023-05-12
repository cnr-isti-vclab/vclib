/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_CONCEPTS_MESH_CONTAINERS_HALF_EDGE_CONTAINER_H
#define VCL_CONCEPTS_MESH_CONTAINERS_HALF_EDGE_CONTAINER_H

#include "element_container.h"

namespace vcl {
namespace mesh {

template <typename T>
concept HasHalfEdgeContainer = requires(T o)
{
	typename T::HalfEdgeType;
	o.halfEdge(uint());
};

} // namespace vcl::mesh

/**
 * @brief HasHalfEdges concepts is satisfied when at least one of its types is (or inherits from)
 * a HalfEdgeContainer. It can be used both to check if a Mesh has half edges, or if in a list of
 * types there is a HalfEdgeContainer.
 *
 * In the following example, a MyMesh type can be instatiated only if one of its template Args is a
 * HalfEdgeContainer:
 * @code{.cpp}
 * template <typename... Args> requires HasHalfEdges<Args...>
 * class MyMesh {
 *     // ...
 * };
 *
 * // ...
 *
 * MyMesh<vcl::VertexContainer<MyVertex>> m1; // not ok
 * MyMesh<vcl::HalfEdgeContainer<MyHalfEdge>> m2; // ok
 * MyMesh<vcl::VertexContainer<MyVertex>, vcl::HalfEdgeContainer<MyHalfEdge>> m3; // ok
 * @endcode
 *
 * To check if a type has (inherits from) HalfEdgeContainer:
 * @code{.cpp}
 * if constexpr (vcl::HasHalfEdges<MyMeshType>) {
 *     // ...
 * }
 * @endcode
 */
template<typename... Args>
concept HasHalfEdges = (vcl::mesh::HasHalfEdgeContainer<Args> | ...);

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_CONTAINERS_HALF_EDGE_CONTAINER_H
