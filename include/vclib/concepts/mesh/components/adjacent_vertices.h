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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_ADJACENT_VERTICES_H
#define VCL_CONCEPTS_MESH_COMPONENTS_ADJACENT_VERTICES_H

#include <vector>

#include "component.h"

namespace vcl::comp {

/**
 * @brief HasAdjacentVertices concept is satisfied only if a Element class
 * provides the types and member functions specified in this concept. These
 * types and member functions allow to access to an AdjacentVertices component
 * of a given element.
 *
 * Note that this concept does not discriminate between the Horizontal
 * AdjacentVertices component and the vertical OptionalAdjacentVertices
 * component, therefore it does not guarantee that a template Element type that
 * satisfies this concept provides AdjacentVertices component at runtime (it is
 * guaranteed only that the proper member functions are available at compile
 * time).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasAdjacentVertices = requires (
    T                                            o,
    const T&                                     co,
    typename T::AdjacentVertexType               v,
    std::vector<typename T::AdjacentVertexType*> vec) {
    // clang-format off
    typename T::AdjacentVertexType;
    typename T::ConstAdjacentVertexIterator;
    typename T::ConstAdjacentVertexIndexIterator;

    { o.adjVerticesNumber() } -> std::same_as<uint>;
    { o.adjVertex(uint()) } -> std::same_as<typename T::AdjacentVertexType*>;
    { co.adjVertex(uint()) } ->
        std::same_as<const typename T::AdjacentVertexType*>;
    { co.adjVertexIndex(uint()) } -> std::same_as<uint>;
    { o.adjVertexMod(int()) } -> std::same_as<typename T::AdjacentVertexType*>;
    { co.adjVertexMod(int()) } ->
        std::same_as<const typename T::AdjacentVertexType*>;
    { co.adjVertexIndexMod(uint()) } -> std::same_as<uint>;

    { o.setAdjVertex(uint(), &v) } -> std::same_as<void>;
    { o.setAdjVertex(uint(), uint()) } -> std::same_as<void>;
    { o.setAdjVertex(typename T::ConstAdjacentVertexIterator(), &v) } ->
        std::same_as<void>;
    { o.setAdjVertex(typename T::ConstAdjacentVertexIterator(), uint()) } ->
        std::same_as<void>;
    { o.setAdjVertex(typename T::ConstAdjacentVertexIndexIterator(), &v) } ->
        std::same_as<void>;
    { o.setAdjVertex(typename T::ConstAdjacentVertexIndexIterator(), uint()) } ->
        std::same_as<void>;
    { o.setAdjVertexMod(int(), &v) } -> std::same_as<void>;
    { o.setAdjVertexMod(int(), uint()) } -> std::same_as<void>;
    { o.setAdjVertices(vec) } -> std::same_as<void>;

    { co.containsAdjVertex(&v) } -> std::same_as<bool>;
    { co.containsAdjVertex(uint()) } -> std::same_as<bool>;
    { co.indexOfAdjVertex(&v) } -> std::same_as<uint>;
    { co.indexOfAdjVertex(uint()) } -> std::same_as<uint>;

    { co.adjVertexBegin() } ->
        std::same_as<typename T::ConstAdjacentVertexIterator>;
    { co.adjVertexEnd() } ->
        std::same_as<typename T::ConstAdjacentVertexIterator>;

    { co.adjVertexIndexBegin() } ->
        std::same_as<typename T::ConstAdjacentVertexIndexIterator>;
    { co.adjVertexIndexEnd() } ->
        std::same_as<typename T::ConstAdjacentVertexIndexIterator>;

    co.adjVertices();
    co.adjVertexIndices();
    // clang-format on
};

/**
 * @brief HasOptionalAdjacentVertices concept is satisfied only if a class
 * satisfies the HasAdjacentVertices concept and has the static boolean constant
 * IS_OPTIONAL is set to true.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalAdjacentVertices =
    HasAdjacentVertices<T> &&
    IsOptionalComponent<typename T::AdjacentVertexPointers>;

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_ADJACENT_VERTICES_H
