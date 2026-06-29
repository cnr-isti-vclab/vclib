// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_CONCEPTS_BLOCKER_EVENT_DRAWER_H
#define VCL_RENDER_CONCEPTS_BLOCKER_EVENT_DRAWER_H

#include "event_drawer.h"

#include <vclib/render/input.h>

namespace vcl {

template<typename T>
concept BlockerEventDrawerConcept =
    EventDrawerConcept<T> && requires (T&& obj) {
        requires RemoveRef<T>::CAN_BLOCK_EVENT_PROPAGATION == true;

        // non const requirements
        requires IsConst<T> || requires {
            {
                obj.onKeyPress(Key::Enum(), KeyModifiers())
            } -> std::same_as<bool>;
            {
                obj.onKeyRelease(Key::Enum(), KeyModifiers())
            } -> std::same_as<bool>;
            {
                obj.onMouseMove(double(), double(), KeyModifiers())
            } -> std::same_as<bool>;
            {
                obj.onMousePress(
                    MouseButton::Enum(), double(), double(), KeyModifiers())
            } -> std::same_as<bool>;
            {
                obj.onMouseRelease(
                    MouseButton::Enum(), double(), double(), KeyModifiers())
            } -> std::same_as<bool>;
            {
                obj.onMouseDoubleClick(
                    MouseButton::Enum(), double(), double(), KeyModifiers())
            } -> std::same_as<bool>;
            {
                obj.onMouseScroll(double(), double(), KeyModifiers())
            } -> std::same_as<bool>;
        };
    };

} // namespace vcl

#endif // VCL_RENDER_CONCEPTS_BLOCKER_EVENT_DRAWER_H
