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

#include <vclib/concepts/ranges/range.h>

#include "component.h"

namespace vcl::comp {

/**
 * @brief HasAdjacentVertices concept is satisfied only if a Element class
 * provides the types and member functions specified in this concept. These
 * types and member functions allow to access to an @ref
 * vcl::comp::AdjacentVertices component of a given element.
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
    T                                            obj,
    const T&                                     cObj,
    typename T::AdjacentVertexType               v,
    typename T::AdjacentVertexIterator           it,
    typename T::ConstAdjacentVertexIterator      cIt,
    typename T::ConstAdjacentVertexIndexIterator cIIt,
    typename T::AdjacentVertexType*              vP,
    const typename T::AdjacentVertexType*        cVP,
    std::vector<typename T::AdjacentVertexType*> vec) {
    typename T::AdjacentVertexType;
    typename T::AdjacentVertexIterator;
    typename T::ConstAdjacentVertexIterator;
    typename T::ConstAdjacentVertexIndexIterator;

    { obj.adjVerticesNumber() } -> std::same_as<uint>;
    { obj.adjVertex(uint()) } -> std::same_as<decltype(vP)>;
    { cObj.adjVertex(uint()) } -> std::same_as<decltype(cVP)>;
    { cObj.adjVertexIndex(uint()) } -> std::same_as<uint>;
    { obj.adjVertexMod(int()) } -> std::same_as<decltype(vP)>;
    { cObj.adjVertexMod(int()) } -> std::same_as<decltype(cVP)>;
    { cObj.adjVertexIndexMod(uint()) } -> std::same_as<uint>;

    { obj.setAdjVertex(uint(), &v) } -> std::same_as<void>;
    { obj.setAdjVertex(uint(), uint()) } -> std::same_as<void>;
    { obj.setAdjVertex(it, &v) } -> std::same_as<void>;
    { obj.setAdjVertex(it, uint()) } -> std::same_as<void>;
    { obj.setAdjVertex(cIt, &v) } -> std::same_as<void>;
    { obj.setAdjVertex(cIt, uint()) } -> std::same_as<void>;
    { obj.setAdjVertex(cIIt, &v) } -> std::same_as<void>;
    { obj.setAdjVertex(cIIt, uint()) } -> std::same_as<void>;
    { obj.setAdjVertexMod(int(), &v) } -> std::same_as<void>;
    { obj.setAdjVertexMod(int(), uint()) } -> std::same_as<void>;
    { obj.setAdjVertices(vec) } -> std::same_as<void>;

    { cObj.containsAdjVertex(&v) } -> std::same_as<bool>;
    { cObj.containsAdjVertex(uint()) } -> std::same_as<bool>;
    { cObj.indexOfAdjVertex(&v) } -> std::same_as<uint>;
    { cObj.indexOfAdjVertex(uint()) } -> std::same_as<uint>;

    { obj.adjVertexBegin() } -> std::same_as<decltype(it)>;
    { obj.adjVertexEnd() } -> std::same_as<decltype(it)>;

    { cObj.adjVertexBegin() } -> std::same_as<decltype(cIt)>;
    { cObj.adjVertexEnd() } -> std::same_as<decltype(cIt)>;

    { cObj.adjVertexIndexBegin() } -> std::same_as<decltype(cIIt)>;
    { cObj.adjVertexIndexEnd() } -> std::same_as<decltype(cIIt)>;

    { obj.adjVertices() } -> vcl::RangeOf<decltype(vP)>;
    { cObj.adjVertices() } -> vcl::RangeOf<decltype(cVP)>;
    { cObj.adjVertexIndices() } -> vcl::RangeOf<uint>;
};

/**
 * @brief HasOptionalAdjacentVertices concept is satisfied only if a class
 * satisfies the @ref vcl::comp::HasAdjacentVertices concept and has the static
 * boolean constant `IS_OPTIONAL` is set to `true`.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalAdjacentVertices =
    HasAdjacentVertices<T> && IsOptionalComponent<typename T::AdjacentVertices>;

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_ADJACENT_VERTICES_H
