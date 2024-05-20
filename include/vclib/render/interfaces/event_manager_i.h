/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_RENDER_INTERFACES_EVENT_MANAGER_I_H
#define VCL_RENDER_INTERFACES_EVENT_MANAGER_I_H

#include <vclib/render/input.h>

namespace vcl {

/**
 * @brief The EventManagerInterface class provides a common interface for event
 * managers.
 *
 * All the widgets/window classes of the vclib-render module will use this
 * interface and call the methods of the event manager to handle the input
 * events.
 *
 * Therefore, the user that implements their custom window/widget class will
 * have to provide an implementation of this interface to handle the input
 * events.
 */
class EventManagerInterface
{
    KeyModifiers mMods = {KeyModifier::NO_MODIFIER};

public:
    virtual ~EventManagerInterface() = default;

    /**
     * @brief Called when the window is updated.
     *
     * You should reimplement this method if you need to update some state
     * when the window is updated.
     */
    virtual void update() {}

protected:
    void setModifiers(KeyModifiers modifiers) { mMods = modifiers; }

    const KeyModifiers& modifiers() const { return mMods; }

    /**
     * @brief Called when the window is resized.
     * @param[in] w: The new width of the window.
     * @param[in] h: The new height of the window.
     */
    virtual void onResize(unsigned int w, unsigned int h) {}

    /**
     * @brief Called when a key is pressed.
     * @param[in] key: The key that was pressed.
     */
    virtual void onKeyPress(Key::Enum key) {}

    /**
     * @brief Called when a key is released.
     * @param[in] key: The key that was released.
     */
    virtual void onKeyRelease(Key::Enum key) {}

    /**
     * @brief Called when the mouse moves.
     * @param[in] x: The x coordinate of the mouse.
     * @param[in] y: The y coordinate of the mouse.
     */
    virtual void onMouseMove(double x, double y) {}

    /**
     * @brief Called when a mouse button is pressed.
     * @param[in] button: The button that was pressed.
     */
    virtual void onMousePress(MouseButton::Enum button) {}

    /**
     * @brief Called when a mouse button is released.
     * @param[in] button: The button that was released.
     */
    virtual void onMouseRelease(MouseButton::Enum button) {}

    /**
     * @brief Called when the mouse wheel is scrolled.
     * @param[in] dx: The horizontal scroll amount.
     * @param[in] dy: The vertical scroll amount.
     */
    virtual void onMouseScroll(double dx, double dy) {}
};

} // namespace vcl

#endif // VCL_RENDER_INTERFACES_EVENT_MANAGER_I_H
