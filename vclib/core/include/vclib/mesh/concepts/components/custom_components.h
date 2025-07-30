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

#ifndef VCL_MESH_CONCEPTS_COMPONENTS_CUSTOM_COMPONENTS_H
#define VCL_MESH_CONCEPTS_COMPONENTS_CUSTOM_COMPONENTS_H

#include <vclib/types.h>

#include <string>

namespace vcl::comp {

/**
 * @brief HasCustomComponents concept is satisfied only if a Element class
 * provides the types and member functions specified in this concept. These
 * types and member functions allow to access to a @ref
 * vcl::comp::CustomComponents component of a given element.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasCustomComponents =
    requires (T&& obj, std::string str, std::vector<std::string> vStr) {
        { obj.hasCustomComponent(str) } -> std::same_as<bool>;
        {
            obj.template isCustomComponentOfType<int>(str)
        } -> std::same_as<bool>;
        { obj.customComponentType(str) } -> std::same_as<std::type_index>;
        {
            obj.template customComponentNamesOfType<int>()
        } -> std::same_as<decltype(vStr)>;

        { obj.template customComponent<int>(str) } -> std::convertible_to<int>;

        // non const requirements
        requires IsConst<T> || requires {
            { obj.template customComponent<int>(str) } -> std::same_as<int&>;
        };
    };

} // namespace vcl::comp

#endif // VCL_MESH_CONCEPTS_COMPONENTS_CUSTOM_COMPONENTS_H
