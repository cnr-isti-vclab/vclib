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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_MARK_H
#define VCL_MESH_COMPONENTS_CONCEPTS_MARK_H

#include "component.h"

#include <vclib/types.h>

namespace vcl::comp {

/**
 * @brief HasMark concept is satisfied only if a Element/Mesh class provides the
 * types and member functions specified in this concept. These types and member
 * functions allow to access to a @ref vcl::comp::Mark component of a given
 * element/mesh.
 *
 * Note that this concept does not discriminate between the Horizontal Mark
 * component and the vertical OptionalMark component, therefore it does not
 * guarantee that a template Element type that satisfies this concept provides
 * Mark component at runtime (it is guaranteed only that the proper member
 * functions are available at compile time).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasMark = requires (T&& obj) {
    { obj.mark() } -> std::same_as<int>;

    // non const requirements
    requires IsConst<T> || requires {
        { obj.resetMark() } -> std::same_as<void>;
        { obj.incrementMark() } -> std::same_as<void>;
        { obj.decrementMark() } -> std::same_as<void>;
    };
};

/**
 * @brief HasOptionalMark concept is satisfied only if a class satisfies the
 * @ref vcl::comp::HasMark concept and and the static boolean constant
 * `IS_OPTIONAL` is set to `true`.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalMark =
    HasMark<T> && IsOptionalComponent<typename RemoveRef<T>::Mark>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_MARK_H
