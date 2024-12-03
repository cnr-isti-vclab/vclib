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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_ADJACENT_FACES_H
#define VCL_CONCEPTS_MESH_COMPONENTS_ADJACENT_FACES_H

#include "component.h"

#include <vclib/concepts/ranges/range.h>

#include <vector>

namespace vcl::comp {

/**
 * @brief HasAdjacentFaces concept is satisfied only if a Element class provides
 * the types and member functions specified in this concept. These types and
 * member functions allow to access to a @ref vcl::comp::AdjacentFaces
 * component of a given element.
 *
 * Note that this concept does not discriminate between the Horizontal
 * AdjacentFaces component and the vertical OptionalAdjacentFaces component,
 * therefore it does not guarantee that a template Element type that satisfies
 * this concept provides AdjacentFaces component at runtime (it is guaranteed
 * only that the proper member functions are available at compile time).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasAdjacentFaces = requires (
    T                                          obj,
    const T&                                   cObj,
    typename T::AdjacentFaceType               f,
    typename T::AdjacentFaceIterator           it,
    typename T::ConstAdjacentFaceIterator      cIt,
    typename T::ConstAdjacentFaceIndexIterator cIIt,
    typename T::AdjacentFaceType*              fP,
    const typename T::AdjacentFaceType*        cFP,
    std::vector<typename T::AdjacentFaceType*> vec) {
    T::ADJ_FACE_NUMBER;
    typename T::AdjacentFaceType;
    typename T::AdjacentFaceIterator;
    typename T::ConstAdjacentFaceIterator;
    typename T::ConstAdjacentFaceIndexIterator;

    { obj.adjFacesNumber() } -> std::same_as<uint>;
    { obj.adjFace(uint()) } -> std::same_as<decltype(fP)>;
    { cObj.adjFace(uint()) } -> std::same_as<decltype(cFP)>;
    { cObj.adjFaceIndex(uint()) } -> std::same_as<uint>;
    { obj.adjFaceMod(int()) } -> std::same_as<decltype(fP)>;
    { cObj.adjFaceMod(int()) } -> std::same_as<decltype(cFP)>;
    { cObj.adjFaceIndexMod(uint()) } -> std::same_as<uint>;

    { obj.setAdjFace(uint(), &f) } -> std::same_as<void>;
    { obj.setAdjFace(uint(), uint()) } -> std::same_as<void>;
    { obj.setAdjFace(it, &f) } -> std::same_as<void>;
    { obj.setAdjFace(it, uint()) } -> std::same_as<void>;
    { obj.setAdjFace(cIt, &f) } -> std::same_as<void>;
    { obj.setAdjFace(cIt, uint()) } -> std::same_as<void>;
    { obj.setAdjFace(cIIt, &f) } -> std::same_as<void>;
    { obj.setAdjFace(cIIt, uint()) } -> std::same_as<void>;
    { obj.setAdjFaceMod(int(), &f) } -> std::same_as<void>;
    { obj.setAdjFaceMod(int(), uint()) } -> std::same_as<void>;
    { obj.setAdjFaces(vec) } -> std::same_as<void>;

    { cObj.containsAdjFace(&f) } -> std::same_as<bool>;
    { cObj.containsAdjFace(uint()) } -> std::same_as<bool>;
    { cObj.indexOfAdjFace(&f) } -> std::same_as<uint>;
    { cObj.indexOfAdjFace(uint()) } -> std::same_as<uint>;

    { obj.adjFaceBegin() } -> std::same_as<decltype(it)>;
    { obj.adjFaceEnd() } -> std::same_as<decltype(it)>;

    { cObj.adjFaceBegin() } -> std::same_as<decltype(cIt)>;
    { cObj.adjFaceEnd() } -> std::same_as<decltype(cIt)>;

    { cObj.adjFaceIndexBegin() } -> std::same_as<decltype(cIIt)>;
    { cObj.adjFaceIndexEnd() } -> std::same_as<decltype(cIIt)>;

    { obj.adjFaces() } -> vcl::RangeOf<decltype(fP)>;
    { cObj.adjFaces() } -> vcl::RangeOf<decltype(cFP)>;
    { cObj.adjFaceIndices() } -> vcl::RangeOf<uint>;
};

/**
 * @brief HasOptionalAdjacentFaces concept is satisfied only if a class
 * satisfies the @ref vcl::comp::HasAdjacentFaces concept and the
 * static boolean constant `IS_OPTIONAL` is set to `true`.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalAdjacentFaces =
    HasAdjacentFaces<T> && IsOptionalComponent<typename T::AdjacentFaces>;

/**
 * @private
 * @brief HasRightNumberOfAdjacentFaces concept
 *
 * This concept is designed to be used with Face components, where the number of
 * adjacent faces, if tied to the vertex number, must be consisted w.r.t. the
 * number of vertices of the face.
 *
 * This concept is satisfied only if static number of adjacent faces is the same
 * of the static number of vertices.
 */
template<typename T>
concept HasRightNumberOfAdjacentFaces =
    !comp::IsTiedToVertexNumber<typename T::AdjacentFaces> ||
    T::VERTEX_NUMBER == T::ADJ_FACE_NUMBER;

/**
 * @private
 * @brief SanityCheckAdjacentFaces concept
 *
 * This concept is designed to be used with Face components, where the number of
 * adjacent faces must be consistent w.r.t. the number of vertices of the face.
 *
 * It is satisfied if:
 * - the component does *not* have adjacent faces;
 * - in case it has adjacent faces, they have the same number of vertices of the
 * face.
 */
template<typename T>
concept SanityCheckAdjacentFaces =
    !HasAdjacentFaces<T> || HasRightNumberOfAdjacentFaces<T>;

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_ADJACENT_FACES_H
