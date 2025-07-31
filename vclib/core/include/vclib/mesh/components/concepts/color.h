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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_COLOR_H
#define VCL_MESH_COMPONENTS_CONCEPTS_COLOR_H

#include "component.h"
#include "predicates.h"

#include <vclib/space/core.h>

namespace vcl::comp {

template<typename, bool>
class Color;

/**
 * @brief A concept that checks whether a type T (that should be a Element or a
 * Mesh) has the Color component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from vcl::comp::Color,
 * with any template arguments.
 *
 * Note that this concept does not discriminate between the Horizontal Color
 * component and the vertical OptionalColor component, therefore it does not
 * guarantee that a template Element type that satisfies this concept provides
 * Color component at runtime (it is guaranteed only that the proper member
 * functions are available at compile time).
 *
 * @tparam T: The type to be tested for conformity to the HasColor.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasColor = TB::IsDerivedFromSpecializationOfV<T, Color>;

/**
 * @brief A concept that checks whether a type T (that should be a Element or a
 * Mesh) has the Color component (inherits from it), and that the component is
 * optional.
 *
 * @tparam T: The type to be tested for conformity to the HasOptionalColor.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalColor =
    HasColor<T> && IsOptionalComponent<typename RemoveRef<T>::Color>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_COLOR_H
