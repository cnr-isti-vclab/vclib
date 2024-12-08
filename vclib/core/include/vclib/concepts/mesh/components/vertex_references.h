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

#include <vclib/concepts/const_correctness.h>
#include <vclib/concepts/ranges/range.h>

#include <vector>

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
    T&&                                             obj,
    typename RemoveRef<T>::VertexType               v,
    typename RemoveRef<T>::VertexIterator           it,
    typename RemoveRef<T>::ConstVertexIterator      cIt,
    typename RemoveRef<T>::ConstVertexIndexIterator cIIt,
    typename RemoveRef<T>::VertexType*              vP,
    const typename RemoveRef<T>::VertexType*        cVP,
    std::vector<typename RemoveRef<T>::VertexType*> vecV,
    std::vector<uint>                               vecU) {
    RemoveRef<T>::VERTEX_NUMBER;

    typename RemoveRef<T>::VertexType;
    typename RemoveRef<T>::VertexIterator;
    typename RemoveRef<T>::ConstVertexIterator;
    typename RemoveRef<T>::ConstVertexIndexIterator;

    { obj.vertexNumber() } -> std::same_as<uint>;

    { obj.vertex(uint()) } -> std::convertible_to<decltype(cVP)>;
    { obj.vertexIndex(uint()) } -> std::same_as<uint>;
    { obj.vertexMod(int()) } -> std::convertible_to<decltype(cVP)>;
    { obj.vertexIndexMod(int()) } -> std::same_as<uint>;

    { obj.containsVertex(&v) } -> std::same_as<bool>;
    { obj.containsVertex(uint()) } -> std::same_as<bool>;
    { obj.indexOfVertex(&v) } -> std::same_as<uint>;
    { obj.indexOfVertex(uint()) } -> std::same_as<uint>;
    { obj.indexOfEdge(&v, &v) } -> std::same_as<uint>;
    { obj.indexOfEdge(uint(), uint()) } -> std::same_as<uint>;

    { obj.vertexBegin() } -> std::input_iterator;
    { obj.vertexEnd() } -> std::input_iterator;

    { obj.vertexIndexBegin() } -> std::input_iterator;
    { obj.vertexIndexEnd() } -> std::input_iterator;

    { obj.vertexIndices() } -> vcl::RangeOf<uint>;

    // const requirements
    requires !vcl::IsConst<T> || requires {
        { obj.vertices() } -> vcl::RangeOf<decltype(cVP)>;
    };

    // non const requirements
    requires vcl::IsConst<T> || requires {
        { obj.vertex(uint()) } -> std::same_as<decltype(vP)>;
        { obj.vertexMod(int()) } -> std::same_as<decltype(vP)>;

        { obj.setVertex(uint(), &v) } -> std::same_as<void>;
        { obj.setVertex(uint(), uint()) } -> std::same_as<void>;
        { obj.setVertex(it, &v) } -> std::same_as<void>;
        { obj.setVertex(it, uint()) } -> std::same_as<void>;
        { obj.setVertex(cIt, &v) } -> std::same_as<void>;
        { obj.setVertex(cIt, uint()) } -> std::same_as<void>;
        { obj.setVertex(cIIt, &v) } -> std::same_as<void>;
        { obj.setVertex(cIIt, uint()) } -> std::same_as<void>;
        { obj.setVertexMod(int(), &v) } -> std::same_as<void>;
        { obj.setVertexMod(int(), uint()) } -> std::same_as<void>;
        { obj.setVertices(vecV) } -> std::same_as<void>;
        { obj.setVertices(vecU) } -> std::same_as<void>;

        // { obj.vertexBegin() } -> std::output_iterator<decltype(vP)>;
        // { obj.vertexEnd() } -> std::output_iterator<decltype(vP)>;

        { obj.vertices() } -> vcl::RangeOf<decltype(vP)>;
    };
};

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_VERTEX_REFERENCES_H
