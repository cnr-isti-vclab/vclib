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

#ifndef VCL_MESH_CONCEPTS_COMPONENTS_WEDGE_TEX_COORDS_H
#define VCL_MESH_CONCEPTS_COMPONENTS_WEDGE_TEX_COORDS_H

#include "component.h"

#include <vclib/concepts/space/tex_coord.h>

#include <vclib/types.h>

#include <vector>

namespace vcl::comp {

/**
 * @brief HasWedgeTexCoords concept is satisfied only if a Element class
 * provides the types and member functions specified in this concept. These
 * types and member functions allow to access to an @ref
 * vcl::comp::WedgeTexCoords component of a given element.
 *
 * Note that this concept does not discriminate between the Horizontal
 * WedgeTexCoords component and the vertical OptionalWedgeTexCoords component,
 * therefore it does not guarantee that a template Element type that satisfies
 * this concept provides WedgeTexCoords component at runtime (it is guaranteed
 * only that the proper member functions are available at compile time).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasWedgeTexCoords = requires (
    T&&                                                   obj,
    typename RemoveRef<T>::WedgeTexCoordType              t,
    std::vector<typename RemoveRef<T>::WedgeTexCoordType> vec) {
    RemoveRef<T>::WEDGE_TEX_COORD_NUMBER;
    typename RemoveRef<T>::WedgeTexCoordType;
    typename RemoveRef<T>::WedgeTexCoordsIterator;
    typename RemoveRef<T>::ConstWedgeTexCoordsIterator;

    { obj.wedgeTexCoord(uint()) } -> TexCoordConcept;
    { obj.wedgeTexCoordMod(int()) } -> TexCoordConcept;

    { obj.textureIndex() } -> std::convertible_to<ushort>;

    { obj.wedgeTexCoordBegin() } -> InputIterator<decltype(t)>;
    { obj.wedgeTexCoordEnd() } -> InputIterator<decltype(t)>;

    { obj.wedgeTexCoords() } -> InputRange<decltype(t)>;

    // non const requirements
    requires IsConst<T> || requires {
        { obj.setWedgeTexCoord(uint(), t) } -> std::same_as<void>;
        { obj.setWedgeTexCoords(vec) } -> std::same_as<void>;

        { obj.textureIndex() } -> std::same_as<ushort&>;

        { obj.wedgeTexCoordBegin() } -> OutputIterator<decltype(t)>;
        { obj.wedgeTexCoordEnd() } -> OutputIterator<decltype(t)>;

        { obj.wedgeTexCoords() } -> OutputRange<decltype(t)>;
    };
};

/**
 * @brief HasOptionalWedgeTexCoords concept is satisfied only if a class
 * satisfied the @ref vcl::comp::HasWedgeTexCoords and has the static
 * boolean constant `IS_OPTIONAL` is set to `true`.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalWedgeTexCoords =
    HasWedgeTexCoords<T> &&
    IsOptionalComponent<typename RemoveRef<T>::WedgeTexCoords>;

/**
 * @private
 * @brief HasRightNumberOfWedgeTexCoords concept
 *
 * This concept is designed to be used with Face components, where the number of
 * wedge texcoords must be consistent w.r.t. the number of vertices of the face.
 *
 * This concept is satisfied only if static number of wedge texcoords is the
 * same of the static number of vertices.
 */
template<typename T>
concept HasRightNumberOfWedgeTexCoords =
    RemoveRef<T>::VERTEX_NUMBER == RemoveRef<T>::WEDGE_TEX_COORD_NUMBER;

/**
 * @private
 * @brief SanityCheckWedgeTexCoords concept
 *
 * This concept is designed to be used with Face components, where the number of
 * wedge texcoords must be consistent w.r.t. the number of vertices of the face.
 *
 * It is satisfied if:
 * - the component does *not* have wedge texcoords;
 * - in case it has wedge texcoords, they have the same number of vertices of
 * the face.
 */
template<typename T>
concept SanityCheckWedgeTexCoords =
    !HasWedgeTexCoords<T> || HasRightNumberOfWedgeTexCoords<T>;

} // namespace vcl::comp

#endif // VCL_MESH_CONCEPTS_COMPONENTS_WEDGE_TEX_COORDS_H
