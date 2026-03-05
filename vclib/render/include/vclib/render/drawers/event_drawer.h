/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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
 * By default, the EventDrawer class does not block event propagation to other
 * drawers. This behaviour can be changed by setting the CAN_BLOCK_EVENTS
 * template parameter to true (or by inheriting from the BlockerEventDrawer
 * class).
 *
 * All the event functions of this class return a boolean value.
 * If the EventDrawer class is instantiated with CAN_BLOCK_EVENTS = false
 * (default), the return value of the event functions is ignored.
 * In case of CAN_BLOCK_EVENTS = true (BlockerEventDrawer), if a function
 * returns true, the DerivedRenderApp will block event propagation to other
 * drawers; if a function returns false, the event is propagated to the other
 * drawers.
 *
 * @tparam DerivedRenderApp The type of the derived RenderApp class.
 * @tparam CAN_BLOCK_EVENTS Whether the event functions of this class can block
 * event propagation to other drawers.
 */
template<typename DerivedRenderApp, bool CAN_BLOCK_EVENTS = false>
class EventDrawer : public PlainDrawer<DerivedRenderApp>
{
public:
    static const bool CAN_BLOCK_EVENT_PROPAGATION = CAN_BLOCK_EVENTS;

    EventDrawer() = default;

    EventDrawer(uint, uint) {}

    virtual bool onKeyPress(Key::Enum key, const KeyModifiers& modifiers)
    {
        return false;
    }

    virtual bool onKeyRelease(Key::Enum key, const KeyModifiers& modifiers)
    {
        return false;
    }

    virtual bool onMouseMove(double x, double y, const KeyModifiers& modifiers)
    {
        return false;
    }

    virtual bool onMousePress(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
        return false;
    }

    virtual bool onMouseRelease(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
        return false;
    }

    virtual bool onMouseDoubleClick(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
        return false;
    }

    virtual bool onMouseScroll(
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
        return false;
    }
};

template<typename DerivedRenderApp>
using BlockerEventDrawer = EventDrawer<DerivedRenderApp, true>;

} // namespace vcl

#endif // VCL_RENDER_DRAWERS_EVENT_DRAWER_H
