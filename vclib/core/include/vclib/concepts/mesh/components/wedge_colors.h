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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_WEDGE_COLORS_H
#define VCL_CONCEPTS_MESH_COMPONENTS_WEDGE_COLORS_H

#include "component.h"

#include <vclib/concepts/ranges/range.h>
#include <vclib/concepts/space/color.h>

#include <vector>

namespace vcl::comp {

/**
 * @brief HasWedgeColors concept is satisfied only if a Element class provides
 * the types and member functions specified in this concept. These types and
 * member functions allow to access to an @ref vcl::comp::WedgeColors component
 * of a given element.
 *
 * Note that this concept does not discriminate between the Horizontal
 * WedgeColors component and the vertical OptionalWedgeColors component,
 * therefore it does not guarantee that a template Element type that satisfies
 * this concept provides WedgeColors component at runtime (it is guaranteed only
 * that the proper member functions are available at compile time).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasWedgeColors = requires (
    T&&                                                obj,
    typename RemoveRef<T>::WedgeColorType              c,
    std::vector<typename RemoveRef<T>::WedgeColorType> vec) {
    RemoveRef<T>::WEDGE_COLOR_NUMBER;
    typename RemoveRef<T>::WedgeColorType;
    typename RemoveRef<T>::WedgeColorsIterator;
    typename RemoveRef<T>::ConstWedgeColorsIterator;

    { obj.wedgeColor(uint()) } -> ColorConcept;
    { obj.wedgeColorMod(int()) } -> ColorConcept;

    { obj.wedgeColorBegin() } -> std::input_iterator;
    { obj.wedgeColorEnd() } -> std::input_iterator;

    { obj.wedgeColors() } -> vcl::RangeOf<decltype(c)>;

    // non const requirements
    requires vcl::IsConst<T> || requires {
        { obj.setWedgeColor(uint(), c) } -> std::same_as<void>;
        { obj.setWedgeColors(vec) } -> std::same_as<void>;

        { obj.wedgeColorBegin() } -> std::output_iterator<decltype(c)>;
        { obj.wedgeColorEnd() } -> std::output_iterator<decltype(c)>;
    };
};

/**
 * @brief HasOptionalWedgeColors concept is satisfied only if a class satisfies
 * the @ref vcl::comp::HasWedgeColors concept and has the static boolean
 * constant `IS_OPTIONAL` is set to `true`.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalWedgeColors =
    HasWedgeColors<T> &&
    IsOptionalComponent<typename RemoveRef<T>::WedgeColors>;

/**
 * @private
 * @brief HasRightNumberOfWedgeColors concept
 *
 * This concept is designed to be used with Face components, where the number of
 * wedge colors must be consistent w.r.t. the number of vertices of the face.
 *
 * This concept is satisfied only if static number of wedge colors is the same
 * of the static number of vertices.
 */
template<typename T>
concept HasRightNumberOfWedgeColors = T::VERTEX_NUMBER == T::WEDGE_COLOR_NUMBER;

/**
 * @private
 * @brief SanityCheckWedgeColors concept
 *
 * This concept is designed to be used with Face components, where the number of
 * wedge colors must be consistent w.r.t. the number of vertices of the face.
 *
 * It is satisfied if:
 * - the component does *not* have wedge colors;
 * - in case it has wedge colors, they have the same number of vertices of the
 * face.
 */
template<typename T>
concept SanityCheckWedgeColors =
    !HasWedgeColors<T> || HasRightNumberOfWedgeColors<T>;

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_WEDGE_COLORS_H
