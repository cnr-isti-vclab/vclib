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

#ifndef BLOCKER_EVENT_DRAWER_H
#define BLOCKER_EVENT_DRAWER_H

#include "plain_drawer.h"

#include <vclib/render/input.h>

namespace vcl {

/**
 * @brief Special EventDrawer class for the case where the drawer can request to
 * block the event propagation.
 *
 * A drawer that inherits from this class can request to block the event
 * propagation, and (if requested) the event will not be propagated to the other
 * drawer arguments of the RenderApp class.
 *
 * All the event functions of this class return a boolean value. If the function
 * returns true, the DerivedRenderApp will block event propagation to other
 * drawers; otherwise, the event is propagated to the other drawers.
 *
 * @tparam DerivedRenderApp The type of the derived RenderApp class.
 */
template<typename DerivedRenderApp>
class BlockerEventDrawer : public PlainDrawer<DerivedRenderApp>
{
public:
    static const bool CAN_BLOCK_EVENT_PROPAGATION = true;

    BlockerEventDrawer() = default;

    BlockerEventDrawer(uint, uint) {}

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

} // namespace vcl

#endif // BLOCKER_EVENT_DRAWER_H
