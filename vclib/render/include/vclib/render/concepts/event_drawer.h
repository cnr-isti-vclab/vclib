// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
