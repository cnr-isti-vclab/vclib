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

#include <vector>

#include "component.h"

namespace vcl::comp {

/**
 * @brief HasAdjacentFaces concept is satisfied only if a Element class provides
 * the types and member functions specified in this concept. These types and
 * member functions allow to access to an AdjacentFaces component of a given
 * element.
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
    T                                          o,
    const T&                                   co,
    typename T::AdjacentFaceType               f,
    std::vector<typename T::AdjacentFaceType*> v) {
    // clang-format off
    T::ADJ_FACE_NUMBER;
    typename T::AdjacentFaceType;
    typename T::AdjacentFaceIterator;
    typename T::ConstAdjacentFaceIterator;

    { o.adjFacesNumber() } -> std::same_as<uint>;
    { o.adjFace(uint()) } -> std::same_as<typename T::AdjacentFaceType*>;
    { co.adjFace(uint()) } -> std::same_as<const typename T::AdjacentFaceType*>;
    { o.adjFaceMod(int()) } -> std::same_as<typename T::AdjacentFaceType*>;
    { co.adjFaceMod(int()) } ->
        std::same_as<const typename T::AdjacentFaceType*>;
    { o.setAdjFace(uint(), &f) } -> std::same_as<void>;
    { o.setAdjFaceMod(int(), &f) } -> std::same_as<void>;
    { o.setAdjFaces(v) } -> std::same_as<void>;
    { co.containsAdjFace(&f) } -> std::same_as<bool>;

    { o.findAdjFace(&f) } -> std::same_as<typename T::AdjacentFaceIterator>;
    { co.findAdjFace(&f) } ->
        std::same_as<typename T::ConstAdjacentFaceIterator>;
    { co.indexOfAdjFace(&f) } -> std::same_as<uint>;

    { o.adjFaceBegin() } -> std::same_as<typename T::AdjacentFaceIterator>;
    { o.adjFaceEnd() } -> std::same_as<typename T::AdjacentFaceIterator>;
    { co.adjFaceBegin() } ->
        std::same_as<typename T::ConstAdjacentFaceIterator>;
    { co.adjFaceEnd() } -> std::same_as<typename T::ConstAdjacentFaceIterator>;
    o.adjFaces();
    co.adjFaces();
    // clang-format on
};

/**
 * @brief HasOptionalAdjacentFaces concept is satisfied only if a class
 * satisfies the HasAdjacentFacesComponent concept and the static boolean
 * constant IS_OPTIONAL is set to true.
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
 * adjacent faces must be consisted w.r.t. the number of vertices of the face.
 *
 * This concept is satisfied only if static number of adjacent faces is the same
 * of the static number of vertices.
 */
template<typename T>
concept HasRightNumberOfAdjacentFaces = T::VERTEX_NUMBER == T::ADJ_FACE_NUMBER;

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
