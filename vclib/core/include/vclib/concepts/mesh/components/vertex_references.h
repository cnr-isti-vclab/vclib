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
    T                                    obj,
    const T&                             cObj,
    typename T::VertexType               v,
    typename T::VertexIterator           it,
    typename T::ConstVertexIterator      cIt,
    typename T::ConstVertexIndexIterator cIIt,
    typename T::VertexType*              vP,
    const typename T::VertexType*        cVP,
    std::vector<typename T::VertexType*> vecV,
    std::vector<uint>                    vecU) {
    T::VERTEX_NUMBER;
    typename T::VertexReferences;

    typename T::VertexType;
    typename T::VertexIterator;
    typename T::ConstVertexIterator;
    typename T::ConstVertexIndexIterator;

    { cObj.vertexNumber() } -> std::same_as<uint>;
    { obj.vertex(uint()) } -> std::same_as<decltype(vP)>;
    { cObj.vertex(uint()) } -> std::same_as<decltype(cVP)>;
    { cObj.vertexIndex(uint()) } -> std::same_as<uint>;
    { obj.vertexMod(int()) } -> std::same_as<decltype(vP)>;
    { cObj.vertexMod(int()) } -> std::same_as<decltype(cVP)>;
    { cObj.vertexIndexMod(int()) } -> std::same_as<uint>;

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

    { cObj.containsVertex(&v) } -> std::same_as<bool>;
    { cObj.containsVertex(uint()) } -> std::same_as<bool>;
    { cObj.indexOfVertex(&v) } -> std::same_as<uint>;
    { cObj.indexOfVertex(uint()) } -> std::same_as<uint>;
    { cObj.indexOfEdge(&v, &v) } -> std::same_as<uint>;
    { cObj.indexOfEdge(uint(), uint()) } -> std::same_as<uint>;

    { cObj.vertexBegin() } -> std::same_as<decltype(cIt)>;
    { cObj.vertexEnd() } -> std::same_as<decltype(cIt)>;

    { cObj.vertexIndexBegin() } -> std::same_as<decltype(cIIt)>;
    { cObj.vertexIndexEnd() } -> std::same_as<decltype(cIIt)>;

    { obj.vertices() } -> vcl::RangeOf<decltype(vP)>;
    { cObj.vertices() } -> vcl::RangeOf<decltype(cVP)>;
    { cObj.vertexIndices() } -> vcl::RangeOf<uint>;
};

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_VERTEX_REFERENCES_H
