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

#ifndef VCL_MESH_CONCEPTS_COMPONENTS_NORMAL_H
#define VCL_MESH_CONCEPTS_COMPONENTS_NORMAL_H

#include "component.h"

#include <vclib/space/core.h>

namespace vcl::comp {

/**
 * @brief HasNormal concept is satisfied only if a Element class provides the
 * types and member functions specified in this concept. These types and member
 * functions allow to access to a @ref vcl::comp::Normal component of a given
 * element.
 *
 * Note that this concept does not discriminate between the Horizontal Normal
 * component and the vertical OptionalNormal component, therefore it does not
 * guarantee that a template Element type that satisfies this concept provides
 * Normal component at runtime (it is guaranteed only that the proper member
 * functions are available at compile time).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasNormal = requires (T&& obj) {
    typename RemoveRef<T>::NormalType;
    { obj.normal() } -> PointConcept;
};

/**
 * @brief HasOptionalNormal concept is satisfied only if a class satisfies the
 * @ref vcl::comp::HasNormal concept and the static boolean constant
 * `IS_OPTIONAL` is set to `true`.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalNormal =
    HasNormal<T> && IsOptionalComponent<typename RemoveRef<T>::Normal>;

} // namespace vcl::comp

#endif // VCL_MESH_CONCEPTS_COMPONENTS_NORMAL_H
