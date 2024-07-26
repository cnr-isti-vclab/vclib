/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_CONCEPTS_MESH_CONTAINERS_EDGE_CONTAINER_H
#define VCL_CONCEPTS_MESH_CONTAINERS_EDGE_CONTAINER_H

#include <ranges>
#include <vector>

#include "element_container.h"

namespace vcl {
namespace mesh {

/**
 * @brief The HasEdgeContainer concept is satisfied only if a container class
 * provides the types and member functions specified in this concept. These
 * types and member functions allow to access to a @ref vcl::mesh::EdgeContainer
 * container of a given mesh.
 * 
 * @ingroup containers_concepts
 */
template<typename T>
concept HasEdgeContainer =
    requires (T o, const T& co, typename T::EdgeType* e) {
        // clang-format off
        typename T::EdgeType;
        typename T::EdgeIterator;
        typename T::ConstEdgeIterator;

        { o.edge(uint()) } -> std::same_as<typename T::EdgeType&>;
        { co.edge(uint()) } -> std::same_as<const typename T::EdgeType&>;

        { co.edgeNumber() } -> std::same_as<uint>;
        { co.edgeContainerSize() } -> std::same_as<uint>;
        { co.deletedEdgeNumber() } -> std::same_as<uint>;
        o.deleteEdge(uint());
        o.deleteEdge(e);
        { o.edgeIndexIfCompact(uint()) } -> std::same_as<uint>;
        { o.edgeCompactIndices() } -> std::same_as<std::vector<uint>>;

        { o.addEdge() } -> std::same_as<uint>;
        { o.addEdges(uint()) } -> std::same_as<uint>;
        { o.reserveEdges(uint()) } -> std::same_as<void>;
        { o.compactEdges() } -> std::same_as<void>;

        { o.edgeBegin() } -> std::same_as<typename T::EdgeIterator>;
        { co.edgeBegin() } -> std::same_as<typename T::ConstEdgeIterator>;
        { o.edgeEnd() } -> std::same_as<typename T::EdgeIterator>;
        { co.edgeEnd() } -> std::same_as<typename T::ConstEdgeIterator>;

        requires std::ranges::range<decltype(o.edges())>;
        requires std::ranges::range<decltype(co.edges())>;
        // clang-format on
    };

} // namespace mesh

/**
 * @brief HasEdges concepts is satisfied when at least one of its
 * template types is (or inherits from) a @ref vcl::mesh::EdgeContainer. It can
 * be used both to check if a Mesh has edges, or if in a list of types there is
 * a EdgeContainer.
 *
 * In the following example, a MyMesh type can be instantiated only if one of
 * its template Args is a EdgeContainer:
 * @code{.cpp}
 * template <typename... Args> requires HasEdges<Args...>
 * class MyMesh {
 *     // ...
 * };
 *
 * // ...
 *
 * MyMesh<vcl::VertexContainer<MyVertex>> m1; // not ok
 * MyMesh<vcl::EdgeContainer<MyEdge>> m2; // ok
 * MyMesh<vcl::VertexContainer<MyVertex>, vcl::EdgeContainer<MyEdge>> m3; // ok
 * @endcode
 *
 * To check if a type has (inherits from) EdgeContainer:
 * @code{.cpp}
 * if constexpr (vcl::HasEdges<MyMeshType>) {
 *     // ...
 * }
 * @endcode
 *
 * @note This concept does not check if a Mesh is a valid EdgeMesh.
 * To do that, use the @ref vcl::EdgeMeshConcept.
 * 
 * @ingroup containers_concepts
 */
template<typename... Args>
concept HasEdges = (vcl::mesh::HasEdgeContainer<Args> || ...);

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_CONTAINERS_EDGE_CONTAINER_H
