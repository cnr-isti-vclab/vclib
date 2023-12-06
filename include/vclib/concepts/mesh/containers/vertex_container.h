/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_CONCEPTS_MESH_CONTAINERS_VERTEX_CONTAINER_H
#define VCL_CONCEPTS_MESH_CONTAINERS_VERTEX_CONTAINER_H

#include <ranges>
#include <vector>

#include "element_container.h"

namespace vcl {
namespace mesh {

template<typename T>
concept HasVertexContainer = requires (
    T                                 o,
    const T&                          co,
    typename T::VertexType*           v,
    typename T::VertexType::CoordType c) {
    // clang-format off
    typename T::VertexType;
    typename T::VertexIterator;
    typename T::ConstVertexIterator;

    { o.vertex(uint()) } -> std::same_as<typename T::VertexType&>;
    { co.vertex(uint()) } -> std::same_as<const typename T::VertexType&>;

    { co.vertexNumber() } -> std::same_as<uint>;
    { co.vertexContainerSize() } -> std::same_as<uint>;
    { co.deletedVertexNumber() } -> std::same_as<uint>;
    o.deleteVertex(uint());
    o.deleteVertex(v);
    { o.vertexIndexIfCompact(uint()) } -> std::same_as<uint>;
    { o.vertexCompactIndices() } -> std::same_as<std::vector<uint>>;

    { o.addVertex() } -> std::same_as<uint>;
    { o.addVertex(c) } -> std::same_as<uint>;
    { o.addVertices(uint()) } -> std::same_as<uint>;
    { o.addVertices(c, c, c, c) } -> std::same_as<uint>;
    { o.reserveVertices(uint()) } -> std::same_as<void>;
    { o.compactVertices() } -> std::same_as<void>;

    { o.vertexBegin() } -> std::same_as<typename T::VertexIterator>;
    { co.vertexBegin() } -> std::same_as<typename T::ConstVertexIterator>;
    { o.vertexEnd() } -> std::same_as<typename T::VertexIterator>;
    { co.vertexEnd() } -> std::same_as<typename T::ConstVertexIterator>;

    requires std::ranges::range<decltype(o.vertices())>;
    requires std::ranges::range<decltype(co.vertices())>;
    // clang-format on
};

} // namespace mesh

/**
 * @brief HasVertices concepts is satisfied when at least one of its types is
 * (or inherits from) a VertexContainer. It can be used both to check if a Mesh
 * has vertices, or if in a list of types there is a VertexContainer.
 *
 * In the following example, a MyMesh type can be instatiated only if one of its
 * template Args is a VertexContainer:
 * @code{.cpp}
 * template <typename... Args> requires HasVertices<Args...>
 * class MyMesh {
 *     // ...
 * };
 *
 * // ...
 *
 * MyMesh<vcl::VertexContainer<MyVertex>> m1; // ok
 * MyMesh<vcl::FaceContainer<MyFace>> m2; // not ok
 * MyMesh<vcl::VertexContainer<MyVertex>, vcl::FaceContainer<MyFace>> m3; // ok
 * @endcode
 *
 * To check if a type has (inherits from) VertexContainer:
 * @code{.cpp}
 * if constexpr (vcl::HasVertices<MyMeshType>) {
 *     // ...
 * }
 * @endcode
 *
 * @note This concept does not check if a Mesh is a valid Mesh. To do that, use
 * the MeshConcept.
 */
template<typename... Args>
concept HasVertices = (vcl::mesh::HasVertexContainer<Args> || ...);

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_CONTAINERS_VERTEX_CONTAINER_H
