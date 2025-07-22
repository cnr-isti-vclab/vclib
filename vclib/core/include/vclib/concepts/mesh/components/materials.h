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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_MATERIALS_H
#define VCL_CONCEPTS_MESH_COMPONENTS_MATERIALS_H

#include <vclib/concepts/space/material.h>

namespace vcl::comp {

template<typename T>
concept HasMaterials =
    requires (T&& obj) {

        { obj.materialNumber() } -> std::same_as<uint>;
        { obj.meshBasePath() } -> std::convertible_to<std::string>;

        { obj.material(uint()) } -> MaterialConcept;

        // non const requirements
        requires IsConst<T> || requires {
            { obj.meshBasePath() } -> std::same_as<std::string&>;
        };
    };

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_MATERIALS_H
