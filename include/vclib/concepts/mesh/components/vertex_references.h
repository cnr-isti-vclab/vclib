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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_VERTEX_REFERENCES_H
#define VCL_CONCEPTS_MESH_COMPONENTS_VERTEX_REFERENCES_H

#include <vector>

#include <vclib/concepts/ranges/range.h>

namespace vcl::comp {

/**
 * @brief HasVertexReferences concept is satisfied only if a Element class
 * provides the types and member functions specified in this concept. These
 * types and member functions allow to access to a @ref
 * vcl::comp::VertexReferences component of a given element.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasVertexReferences = requires (
    T                                    o,
    const T&                             co,
    typename T::VertexType               v,
    std::vector<typename T::VertexType*> vecv,
    std::vector<uint>                    vecu) {
    // clang-format off
    T::VERTEX_NUMBER;
    typename T::VertexReferences;

    typename T::VertexType;
    typename T::VertexIterator;
    typename T::ConstVertexIterator;
    typename T::ConstVertexIndexIterator;

    { co.vertexNumber() } -> std::same_as<uint>;
    { o.vertex(uint()) } -> std::same_as<typename T::VertexType*>;
    { co.vertex(uint()) } -> std::same_as<const typename T::VertexType*>;
    { co.vertexIndex(uint()) } -> std::same_as<uint>;
    { o.vertexMod(int()) } -> std::same_as<typename T::VertexType*>;
    { co.vertexMod(int()) } -> std::same_as<const typename T::VertexType*>;
    { co.vertexIndexMod(int()) } -> std::same_as<uint>;

    { o.setVertex(uint(), &v) } -> std::same_as<void>;
    { o.setVertex(uint(), uint()) } -> std::same_as<void>;
    { o.setVertex(typename T::VertexIterator(), &v) } -> std::same_as<void>;
    { o.setVertex(typename T::VertexIterator(), uint()) } -> std::same_as<void>;
    { o.setVertex(typename T::ConstVertexIterator(), &v) } ->
        std::same_as<void>;
    { o.setVertex(typename T::ConstVertexIterator(), uint()) } ->
        std::same_as<void>;
    { o.setVertex(typename T::ConstVertexIndexIterator(), &v) } ->
        std::same_as<void>;
    { o.setVertex(typename T::ConstVertexIndexIterator(), uint()) } ->
        std::same_as<void>;
    { o.setVertexMod(int(), &v) } -> std::same_as<void>;
    { o.setVertexMod(int(), uint()) } -> std::same_as<void>;
    { o.setVertices(vecv) } -> std::same_as<void>;
    { o.setVertices(vecu) } -> std::same_as<void>;

    { co.containsVertex(&v) } -> std::same_as<bool>;
    { co.containsVertex(uint()) }-> std::same_as<bool>;
    { co.indexOfVertex(&v) } -> std::same_as<uint>;
    { co.indexOfVertex(uint()) } -> std::same_as<uint>;
    { co.indexOfEdge(&v, &v) } -> std::same_as<uint>;
    { co.indexOfEdge(uint(), uint()) } -> std::same_as<uint>;

    { co.vertexBegin() } -> std::same_as<typename T::ConstVertexIterator>;
    { co.vertexEnd() } -> std::same_as<typename T::ConstVertexIterator>;

    { co.vertexIndexBegin() } ->
        std::same_as<typename T::ConstVertexIndexIterator>;
    { co.vertexIndexEnd() } ->
        std::same_as<typename T::ConstVertexIndexIterator>;

    { o.vertices() } -> vcl::RangeOf<typename T::VertexType*>;
    { co.vertices() } -> vcl::RangeOf<const typename T::VertexType*>;
    { co.vertexIndices() } -> vcl::RangeOf<uint>;
    // clang-format on
};

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_VERTEX_REFERENCES_H
