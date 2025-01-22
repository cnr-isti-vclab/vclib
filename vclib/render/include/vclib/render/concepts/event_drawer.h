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

#ifndef VCL_RENDER_CONCEPTS_EVENT_DRAWER_H
#define VCL_RENDER_CONCEPTS_EVENT_DRAWER_H

#include "drawer.h"

#include <vclib/render/input.h>

namespace vcl {

template<typename T>
concept EventDrawerConcept = DrawerConcept<T> && requires (T&& obj) {
    RemoveRef<T>::CAN_BLOCK_EVENT_PROPAGATION;

    // non const requirements
    requires IsConst<T> || requires {
        // note: we don't check return type on purpose here
        // as BlockerEventDrawers can return a boolean value, while
        // most EventDrawers do not return anything

        obj.onKeyPress(Key::Enum(), KeyModifiers());
        obj.onKeyRelease(Key::Enum(), KeyModifiers());
        obj.onMouseMove(double(), double(), KeyModifiers());
        obj.onMousePress(
            MouseButton::Enum(), double(), double(), KeyModifiers());
        obj.onMouseRelease(
            MouseButton::Enum(), double(), double(), KeyModifiers());
        obj.onMouseDoubleClick(
            MouseButton::Enum(), double(), double(), KeyModifiers());
        obj.onMouseScroll(double(), double(), KeyModifiers());
    };
};

} // namespace vcl

#endif // VCL_RENDER_CONCEPTS_EVENT_DRAWER_H
