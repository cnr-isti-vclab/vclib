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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_ADJACENT_EDGES_H
#define VCL_CONCEPTS_MESH_COMPONENTS_ADJACENT_EDGES_H

#include "component.h"

#include <vclib/concepts/const_correctness.h>
#include <vclib/concepts/iterators.h>
#include <vclib/concepts/ranges/range.h>

#include <vector>

namespace vcl::comp {

/**
 * @brief HasAdjacentEdges concept is satisfied only if a Element class provides
 * the types and member functions specified in this concept. These types and
 * member functions allow to access to a @ref vcl::comp::AdjacentEdges
 * component of a given element.
 *
 * Note that this concept does not discriminate between the Horizontal
 * AdjacentEdges component and the vertical OptionalAdjacentEdges component,
 * therefore it does not guarantee that a template Element type that satisfies
 * this concept provides AdjacentEdges component at runtime (it is guaranteed
 * only that the proper member functions are available at compile time).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasAdjacentEdges = requires (
    T&&                                                   obj,
    typename RemoveRef<T>::AdjacentEdgeType               e,
    typename RemoveRef<T>::AdjacentEdgeIterator           it,
    typename RemoveRef<T>::ConstAdjacentEdgeIterator      cIt,
    typename RemoveRef<T>::ConstAdjacentEdgeIndexIterator cIIt,
    typename RemoveRef<T>::AdjacentEdgeType*              eP,
    const typename RemoveRef<T>::AdjacentEdgeType*        cEP,
    std::vector<typename RemoveRef<T>::AdjacentEdgeType*> vec) {
    RemoveRef<T>::ADJ_EDGE_NUMBER;
    typename RemoveRef<T>::AdjacentEdgeType;
    typename RemoveRef<T>::AdjacentEdgeIterator;
    typename RemoveRef<T>::ConstAdjacentEdgeIterator;
    typename RemoveRef<T>::ConstAdjacentEdgeIndexIterator;

    { obj.adjEdgesNumber() } -> std::same_as<uint>;

    { obj.adjEdge(uint()) } -> std::convertible_to<decltype(cEP)>;
    { obj.adjEdgeIndex(uint()) } -> std::same_as<uint>;
    { obj.adjEdgeMod(int()) } -> std::convertible_to<decltype(cEP)>;
    { obj.adjEdgeIndexMod(uint()) } -> std::same_as<uint>;

    { obj.containsAdjEdge(&e) } -> std::same_as<bool>;
    { obj.containsAdjEdge(uint()) } -> std::same_as<bool>;
    { obj.indexOfAdjEdge(&e) } -> std::same_as<uint>;
    { obj.indexOfAdjEdge(uint()) } -> std::same_as<uint>;

    { obj.adjEdgeBegin() } -> InputIterator<decltype(cEP)>;
    { obj.adjEdgeEnd() } -> InputIterator<decltype(cEP)>;

    { obj.adjEdgeIndexBegin() } -> InputIterator<uint>;
    { obj.adjEdgeIndexEnd() } -> InputIterator<uint>;

    { obj.adjEdges() } -> InputRange<decltype(cEP)>;
    { obj.adjEdgeIndices() } -> InputRange<uint>;

    // non const requirements
    requires IsConst<T> || requires {
        { obj.adjEdge(uint()) } -> std::same_as<decltype(eP)>;
        { obj.adjEdgeMod(int()) } -> std::same_as<decltype(eP)>;

        { obj.setAdjEdge(uint(), &e) } -> std::same_as<void>;
        { obj.setAdjEdge(uint(), uint()) } -> std::same_as<void>;
        { obj.setAdjEdge(it, &e) } -> std::same_as<void>;
        { obj.setAdjEdge(it, uint()) } -> std::same_as<void>;
        { obj.setAdjEdge(cIt, &e) } -> std::same_as<void>;
        { obj.setAdjEdge(cIt, uint()) } -> std::same_as<void>;
        { obj.setAdjEdge(cIIt, &e) } -> std::same_as<void>;
        { obj.setAdjEdge(cIIt, uint()) } -> std::same_as<void>;
        { obj.setAdjEdgeMod(int(), &e) } -> std::same_as<void>;
        { obj.setAdjEdgeMod(int(), uint()) } -> std::same_as<void>;
        { obj.setAdjEdges(vec) } -> std::same_as<void>;

        // for references components, the iterators returned by begin() and
        // end() are input iterators because they do not allow to modify the
        // content of the container (the only way to do that is by using the set
        // member functions). However, they allow to modify the elements
        // pointed by the iterators (const references components allow to
        // iterate only trough const pointers instead).
        { obj.adjEdgeBegin() } -> InputIterator<decltype(eP)>;
        { obj.adjEdgeEnd() } -> InputIterator<decltype(eP)>;

        { obj.adjEdges() } -> InputRange<decltype(eP)>;
    };
};

/**
 * @brief HasOptionalAdjacentEdges concept is satisfied only if a class
 * satisfies the @ref vcl::comp::HasAdjacentEdges concept and the static boolean
 * constant `IS_OPTIONAL` is set to `true`.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalAdjacentEdges =
    HasAdjacentEdges<T> &&
    IsOptionalComponent<typename RemoveRef<T>::AdjacentEdges>;

/**
 * @private
 * @brief HasRightNumberOfAdjacentEdges concept
 *
 * This concept is designed to be used with Face components, where the number of
 * adjacent edges, if tied to the vertex number, must be consistent w.r.t. the
 * number of vertices of the face.
 *
 * This concept is satisfied only if static number of adjacent edges is the same
 * of the static number of vertices.
 */
template<typename T>
concept HasRightNumberOfAdjacentEdges =
    !comp::IsTiedToVertexNumber<typename RemoveRef<T>::AdjacentEdges> ||
    RemoveRef<T>::VERTEX_NUMBER == RemoveRef<T>::ADJ_EDGE_NUMBER;

/**
 * @private
 * @brief SanityCheckAdjacentEdges concept
 *
 * This concept is designed to be used with Face components, where the number of
 * adjacent edges must be consistent w.r.t. the number of vertices of the face.
 *
 * It is satisfied if:
 * - the component does *not* have adjacent edges, or
 * - in case it has adjacent edges, they have the same number of vertices of the
 * face.
 */
template<typename T>
concept SanityCheckAdjacentEdges =
    !HasAdjacentEdges<T> || HasRightNumberOfAdjacentEdges<T>;

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_ADJACENT_EDGES_H
