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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_GUI_EVENT_MANAGER_INTERFACE_H
#define VCL_GUI_EVENT_MANAGER_INTERFACE_H

#include "input.h"

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
public:
    virtual ~EventManagerInterface() = default;

    /**
     * @brief Called when a key is pressed.
     * @param key The key that was pressed.
     */
    virtual void onKeyPress(Key key, KeyModifiers modifiers) {};

    /**
     * @brief Called when a key is released.
     * @param key The key that was released.
     */
    virtual void onKeyRelease(Key key, KeyModifiers modifiers) {};

    /**
     * @brief Called when the mouse moves.
     * @param x The x coordinate of the mouse.
     * @param y The y coordinate of the mouse.
     */
    virtual void onMouseMove(double x, double y) {};

    /**
     * @brief Called when a mouse button is pressed.
     * @param button The button that was pressed.
     * @param x The x coordinate of the mouse.
     * @param y The y coordinate of the mouse.
     */
    virtual void onMousePress(MouseButton button, double x, double y) {};

    /**
     * @brief Called when a mouse button is released.
     * @param button The button that was released.
     * @param x The x coordinate of the mouse.
     * @param y The y coordinate of the mouse.
     */
    virtual void onMouseRelease(MouseButton button, double x, double y) {};

    /**
     * @brief Called when the mouse wheel is scrolled.
     * @param dx The horizontal scroll amount.
     * @param dy The vertical scroll amount.
     */
    virtual void onMouseScroll(double dx, double dy) {};
};

} // namespace vcl

#endif // VCL_GUI_EVENT_MANAGER_INTERFACE_H
