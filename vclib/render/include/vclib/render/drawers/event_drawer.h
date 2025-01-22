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

#ifndef VCL_RENDER_DRAWERS_EVENT_DRAWER_H
#define VCL_RENDER_DRAWERS_EVENT_DRAWER_H

#include "plain_drawer.h"

#include <vclib/render/input.h>

namespace vcl {

/**
 * @brief The EventDrawer class is a base class for drawers that can handle
 * events.
 *
 * The EventDrawer class is a CRTP class that is templated on the derived
 * RenderApp class. It provides the interface for handling events produced by
 * the DerivedRenderApp class.
 *
 * @tparam DerivedRenderApp The type of the derived RenderApp class.
 */
template<typename DerivedRenderApp>
class EventDrawer : public PlainDrawer<DerivedRenderApp>
{
public:
    static const bool CAN_BLOCK_EVENT_PROPAGATION = false;

    EventDrawer() = default;

    EventDrawer(uint, uint) {}

    virtual void onKeyPress(Key::Enum key, const KeyModifiers& modifiers) {}

    virtual void onKeyRelease(Key::Enum key, const KeyModifiers& modifiers) {}

    virtual void onMouseMove(double x, double y, const KeyModifiers& modifiers)
    {
    }

    virtual void onMousePress(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
    }

    virtual void onMouseRelease(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
    }

    virtual void onMouseDoubleClick(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
    }

    virtual void onMouseScroll(
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
    }
};

} // namespace vcl

#endif // VCL_RENDER_DRAWERS_EVENT_DRAWER_H
