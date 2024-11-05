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

#ifndef VCL_CONCEPTS_MESH_CONTAINERS_VERTEX_CONTAINER_H
#define VCL_CONCEPTS_MESH_CONTAINERS_VERTEX_CONTAINER_H

#include <ranges>
#include <vector>

#include <vclib/concepts/ranges/range.h>

#include "element_container.h"

namespace vcl {
namespace mesh {

/**
 * @brief The HasVertexContainer concept is satisfied only if a container class
 * provides the types and member functions specified in this concept. These
 * types and member functions allow to access to a @ref
 * vcl::mesh::VertexContainer container of a given mesh.
 *
 * @ingroup containers_concepts
 */
template<typename T>
concept HasVertexContainer = requires (
    T                                 obj,
    const T&                          cObj,
    typename T::VertexType*           vP,
    typename T::VertexType::CoordType c) {
    typename T::VertexType;
    typename T::VertexIterator;
    typename T::ConstVertexIterator;

    { obj.vertex(uint()) } -> std::same_as<typename T::VertexType&>;
    { cObj.vertex(uint()) } -> std::same_as<const typename T::VertexType&>;

    { cObj.vertexNumber() } -> std::same_as<uint>;
    { cObj.vertexContainerSize() } -> std::same_as<uint>;
    { cObj.deletedVertexNumber() } -> std::same_as<uint>;
    { obj.deleteVertex(uint()) } -> std::same_as<void>;
    { obj.deleteVertex(vP) } -> std::same_as<void>;
    { obj.vertexIndexIfCompact(uint()) } -> std::same_as<uint>;
    { obj.vertexCompactIndices() } -> std::same_as<std::vector<uint>>;

    { obj.addVertex() } -> std::same_as<uint>;
    { obj.addVertex(c) } -> std::same_as<uint>;
    { obj.addVertices(uint()) } -> std::same_as<uint>;
    { obj.addVertices(c, c, c, c) } -> std::same_as<uint>;
    { obj.reserveVertices(uint()) } -> std::same_as<void>;
    { obj.compactVertices() } -> std::same_as<void>;

    { obj.vertexBegin() } -> std::same_as<typename T::VertexIterator>;
    { cObj.vertexBegin() } -> std::same_as<typename T::ConstVertexIterator>;
    { obj.vertexEnd() } -> std::same_as<typename T::VertexIterator>;
    { cObj.vertexEnd() } -> std::same_as<typename T::ConstVertexIterator>;

    { obj.vertices() } -> vcl::RangeOf<typename T::VertexType>;
    { cObj.vertices() } -> vcl::RangeOf<typename T::VertexType>;
};

} // namespace mesh

/**
 * @brief HasVertices concepts is satisfied when at least one of its types is
 * (or inherits from) a @ref vcl::mesh::VertexContainer. It can be used both to
 * check if a Mesh has vertices, or if in a list of types there is a
 * VertexContainer.
 *
 * In the following example, a MyMesh type can be instantiated only if one of
 * its template Args is a VertexContainer:
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
 *
 * @ingroup containers_concepts
 */
template<typename... Args>
concept HasVertices = (vcl::mesh::HasVertexContainer<Args> || ...);

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_CONTAINERS_VERTEX_CONTAINER_H
