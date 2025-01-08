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

#ifndef VCL_GLFW_EVENT_MANAGER_WINDOW_H
#define VCL_GLFW_EVENT_MANAGER_WINDOW_H

#include <vclib/render/interfaces/event_manager_i.h>
#include <vclib/space/core/point.h>

#include <GLFW/glfw3.h>

namespace vcl::glfw {

class EventManagerWindow : public virtual vcl::EventManagerI
{
    std::string mTitle;

protected:
    GLFWwindow* mWindow = nullptr;
    float       mScaleX = 1.0f; // content scaling
    float       mScaleY = 1.0f;

public:
    EventManagerWindow(
        const std::string& windowTitle,
        uint               width  = 1024,
        uint               height = 768);

    virtual ~EventManagerWindow() = default;

    const std::string& windowTitle() const;

    void setWindowTitle(const std::string& title);

    uint width() const;

    uint height() const;

    float contentScaleX() const;

    float contentScaleY() const;

protected:
    void* winId();

    void* displayId();

    // callbacks
    virtual void glfwFramebufferSizeCallback(
        GLFWwindow*,
        int width,
        int height);

    virtual void glfwContentScaleCallback(
        GLFWwindow*,
        float xscale,
        float yscale);

    virtual void glfwKeyCallback(
        GLFWwindow*,
        int key,
        int,
        int action,
        int mods);

    virtual void glfwMouseButtonCallback(
        GLFWwindow* win,
        int         button,
        int         action,
        int         mods);

    virtual void glfwCursorPosCallback(GLFWwindow*, double xpos, double ypos);

    virtual void glfwScrollCallback(
        GLFWwindow*,
        double xoffset,
        double yoffset);

private:
    void setCallbacks();

    // double click management
    static constexpr int NO_BUTTON = GLFW_MOUSE_BUTTON_LAST + 1;

    // delta time between two clicks
    static constexpr double DOUBLE_CLICK_TIME_SECS   = 0.25;
    static constexpr double DOUBLE_CLICK_DIST_PIXELS = 4.0; // pixels?

    double  mLastPressedTime   = 0.0;
    int     mLastPressedButton = NO_BUTTON;
    Point2d mLastPressedPos    = {0, 0};
};

} // namespace vcl::glfw

#endif // VCL_GLFW_EVENT_MANAGER_WINDOW_H
