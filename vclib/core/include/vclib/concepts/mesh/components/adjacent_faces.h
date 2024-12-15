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

#include <vclib/concepts/const_correctness.h>
#include <vclib/concepts/iterators.h>
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
    T&&                                                   obj,
    typename RemoveRef<T>::AdjacentFaceType               f,
    typename RemoveRef<T>::AdjacentFaceIterator           it,
    typename RemoveRef<T>::ConstAdjacentFaceIterator      cIt,
    typename RemoveRef<T>::ConstAdjacentFaceIndexIterator cIIt,
    typename RemoveRef<T>::AdjacentFaceType*              fP,
    const typename RemoveRef<T>::AdjacentFaceType*        cFP,
    std::vector<typename RemoveRef<T>::AdjacentFaceType*> vec) {
    RemoveRef<T>::ADJ_FACE_NUMBER;
    typename RemoveRef<T>::AdjacentFaceType;
    typename RemoveRef<T>::AdjacentFaceIterator;
    typename RemoveRef<T>::ConstAdjacentFaceIterator;
    typename RemoveRef<T>::ConstAdjacentFaceIndexIterator;

    { obj.adjFacesNumber() } -> std::same_as<uint>;

    { obj.adjFace(uint()) } -> std::convertible_to<decltype(cFP)>;
    { obj.adjFaceIndex(uint()) } -> std::same_as<uint>;
    { obj.adjFaceMod(int()) } -> std::convertible_to<decltype(cFP)>;
    { obj.adjFaceIndexMod(uint()) } -> std::same_as<uint>;

    { obj.containsAdjFace(&f) } -> std::same_as<bool>;
    { obj.containsAdjFace(uint()) } -> std::same_as<bool>;
    { obj.indexOfAdjFace(&f) } -> std::same_as<uint>;
    { obj.indexOfAdjFace(uint()) } -> std::same_as<uint>;

    { obj.adjFaceBegin() } -> InputIterator<decltype(cFP)>;
    { obj.adjFaceEnd() } -> InputIterator<decltype(cFP)>;

    { obj.adjFaceIndexBegin() } -> InputIterator<uint>;
    { obj.adjFaceIndexEnd() } -> InputIterator<uint>;

    { obj.adjFaces() } -> InputRange<decltype(cFP)>;
    { obj.adjFaceIndices() } -> InputRange<uint>;

    // non const requirements
    requires vcl::IsConst<T> || requires {
        { obj.adjFace(uint()) } -> std::same_as<decltype(fP)>;
        { obj.adjFaceMod(int()) } -> std::same_as<decltype(fP)>;

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

        // for references components, the iterators returned by begin() and
        // end() are input iterators because they do not allow to modify the
        // content of the container (the only way to do that is by using the set
        // member functions). However, they allow to modify the elements
        // pointed by the iterators (const references components allow to
        // iterate only trough const pointers instead).
        { obj.adjFaceBegin() } -> InputIterator<decltype(fP)>;
        { obj.adjFaceEnd() } -> InputIterator<decltype(fP)>;

        { obj.adjFaces() } -> InputRange<decltype(fP)>;
    };
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
    HasAdjacentFaces<T> &&
    IsOptionalComponent<typename RemoveRef<T>::AdjacentFaces>;

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
    !comp::IsTiedToVertexNumber<typename RemoveRef<T>::AdjacentFaces> ||
    RemoveRef<T>::VERTEX_NUMBER == RemoveRef<T>::ADJ_FACE_NUMBER;

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
