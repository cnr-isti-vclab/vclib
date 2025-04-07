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

#ifndef VCL_RENDER_CONCEPTS_DRAWER_H
#define VCL_RENDER_CONCEPTS_DRAWER_H

#include <vclib/concepts.h>

namespace vcl {

template<typename T>
concept DrawerConcept = requires (T&& obj) {
    // constructors
    RemoveRef<T>();
    RemoveRef<T>(uint(), uint());

    // non const requirements
    requires IsConst<T> || requires {
        { obj.onInit(uint()) } -> std::same_as<void>;
        { obj.onResize(uint(), uint()) } -> std::same_as<void>;
        { obj.onDraw(uint()) } -> std::same_as<void>;
        { obj.onDrawId(uint()) } -> std::same_as<void>;
        { obj.onDrawContent(uint()) } -> std::same_as<void>;
        { obj.onPostDraw() } -> std::same_as<void>;
    };
};

} // namespace vcl

#endif // VCL_RENDER_CONCEPTS_DRAWER_H
