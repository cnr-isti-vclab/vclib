/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#include <vclib/types.h>

#include <vector>

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
    T&&                                             obj,
    typename RemoveRef<T>::VertexType               v,
    typename RemoveRef<T>::VertexType*              vP,
    typename RemoveRef<T>::VertexType&              vR,
    typename RemoveRef<T>::VertexType::PositionType p,
    std::vector<uint>                               vec) {
    typename RemoveRef<T>::VertexType;
    typename RemoveRef<T>::VertexIterator;
    typename RemoveRef<T>::ConstVertexIterator;

    { obj.vertex(uint()) } -> std::convertible_to<decltype(v)>;
    { obj.vertexNumber() } -> std::same_as<uint>;
    { obj.vertexContainerSize() } -> std::same_as<uint>;
    { obj.deletedVertexNumber() } -> std::same_as<uint>;

    { obj.vertexIndexIfCompact(uint()) } -> std::same_as<uint>;
    { obj.vertexCompactIndices() } -> std::same_as<decltype(vec)>;

    { obj.vertexBegin() } -> InputIterator<decltype(v)>;
    { obj.vertexEnd() } -> InputIterator<decltype(v)>;
    { obj.vertices() } -> InputRange<decltype(v)>;
    { obj.vertices(bool()) } -> InputRange<decltype(v)>;

    // non const requirements
    requires IsConst<T> || requires {
        { obj.vertex(uint()) } -> std::same_as<decltype(vR)>;

        { obj.addVertex() } -> std::same_as<uint>;
        { obj.addVertex(p) } -> std::same_as<uint>;
        { obj.addVertices(uint()) } -> std::same_as<uint>;
        { obj.addVertices(p, p, p, p) } -> std::same_as<uint>;
        { obj.clearVertices() } -> std::same_as<void>;
        { obj.resizeVertices(uint()) } -> std::same_as<void>;
        { obj.reserveVertices(uint()) } -> std::same_as<void>;
        { obj.compactVertices() } -> std::same_as<void>;
        { obj.deleteVertex(uint()) } -> std::same_as<void>;
        { obj.deleteVertex(vP) } -> std::same_as<void>;
        { obj.updateVertexIndices(vec) } -> std::same_as<void>;

        { obj.vertexBegin() } -> OutputIterator<decltype(v)>;
        { obj.vertexEnd() } -> OutputIterator<decltype(v)>;
        { obj.vertices() } -> OutputRange<decltype(v)>;
        { obj.vertices(bool()) } -> OutputRange<decltype(v)>;

        { obj.enableAllPerVertexOptionalComponents() } -> std::same_as<void>;
        { obj.disableAllPerVertexOptionalComponents() } -> std::same_as<void>;
    };
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
concept HasVertices = (mesh::HasVertexContainer<Args> || ...);

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_CONTAINERS_VERTEX_CONTAINER_H
