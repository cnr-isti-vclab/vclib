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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_WEDGE_COLORS_H
#define VCL_MESH_COMPONENTS_CONCEPTS_WEDGE_COLORS_H

#include "component.h"
#include "predicates.h"

#include <vclib/types.h>

namespace vcl::comp {

template<int, typename, bool>
class WedgeColors;

/**
 * @brief A concept that checks whether a type T (that should be a Element) has
 * the WedgeColors component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::comp::WedgeColors, with any template arguments.
 *
 * Note that this concept does not discriminate between the Horizontal
 * WedgeColors component and the vertical OptionalWedgeColors component,
 * therefore it does not guarantee that a template Element type that satisfies
 * this concept provides WedgeColors component at runtime (it is guaranteed
 * only that the proper member functions are available at compile time).
 *
 * @tparam T: The type to be tested for conformity to the HasWedgeColors.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasWedgeColors = ITB::IsDerivedFromSpecializationOfV<T, WedgeColors>;

/**
 * @brief A concept that checks whether a type T (that should be a Element) has
 * the WedgeColors component (inherits from it), and that the component is
 * optional.
 *
 * @tparam T: The type to be tested for conformity to the
 * HasOptionalWedgeColors.
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

#endif // VCL_MESH_COMPONENTS_CONCEPTS_WEDGE_COLORS_H
