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

#ifndef EVENT_DRAWER_H
#define EVENT_DRAWER_H

#include "plain_drawer.h"

#include <vclib/render/input.h>

namespace vcl {

namespace detail {

/**
 * @brief The EventDrawer class is a base class for all drawers that handle
 * events.
 *
 * The EventDrawer class is a CRTP class that is templated on the derived
 * renderer class. It provides the interface for handling events produced by the
 * DerivedRenderer class.
 *
 * The EventDrawer class is specialized for two cases:
 * - CAN_BLOCK_EVENT_PROPAGATION = false: The drawer cannot request to block the
 *   event propagation. All the event functions do not return any value. @see
 *   EventDrawer<DerivedRenderer, false>.
 * - CAN_BLOCK_EVENT_PROPAGATION = true: The drawer can request to block the
 *   event propagation. All the event functions return a boolean value that
 *   indicates if the event propagation is blocked. @see
 *   EventDrawer<DerivedRenderer, true>.
 *
 * @tparam DerivedRenderer The type of the derived renderer class.
 * @tparam CAN_BLOCK_EVENT_PROPAGATION: A boolean flag that indicates if the
 * drawer can request to block the event propagation.
 */
template<typename, bool CAN_BLOCK_EVENT_PROPAGATION = false>
class EventDrawer
{
};

/**
 * @brief Default specialization of the EventDrawer class for the case where the
 * drawer cannot request to block the event propagation.
 *
 * @tparam DerivedRenderer The type of the derived renderer class.
 */
template<typename DerivedRenderer>
class EventDrawer<DerivedRenderer, false> : public PlainDrawer<DerivedRenderer>
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

/**
 * @brief Specialization of the EventDrawer class for the case where the drawer
 * can request to block the event propagation.
 *
 * A drawer that inherits from this class can request to block the event
 * propagation, and (if requested) the event will not be propagated to the other
 * drawer arguments of the Renderer class.
 *
 * All the event functions of this class return a boolean value. If the function
 * returns true, the DerivedRenderer will block event propagation to other
 * drawers; otherwise, the event is propagated to the other drawers.
 *
 * @tparam DerivedRenderer The type of the derived renderer class.
 */
template<typename DerivedRenderer>
class EventDrawer<DerivedRenderer, true> : public PlainDrawer<DerivedRenderer>
{
public:
    static const bool CAN_BLOCK_EVENT_PROPAGATION = true;

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

} // namespace detail

template<typename DerivedRenderer>
using EventDrawer = detail::EventDrawer<DerivedRenderer, false>;

template<typename DerivedRenderer>
using BlockerEventDrawer = detail::EventDrawer<DerivedRenderer, true>;

} // namespace vcl

#endif // EVENT_DRAWER_H
