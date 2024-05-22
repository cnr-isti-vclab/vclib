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

#include <vclib/ext/glfw/canvas_window.h>

#if defined(__linux__)
#ifdef VCLIB_RENDER_WITH_WAYLAND
#define GLFW_EXPOSE_NATIVE_WAYLAND
#else
#define GLFW_EXPOSE_NATIVE_X11
#endif
#elif defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3native.h>

namespace vcl::glfw {

CanvasWindow::CanvasWindow(
    const std::string& windowTitle,
    uint               width,
    uint               height) :
        EventManagerWindow(windowTitle, width, height)
{
    void* nwh = nullptr;

#if defined(__linux__)
#ifdef VCLIB_RENDER_WITH_WAYLAND
    nwh = (void*) (uintptr_t) glfwGetWaylandWindow(mWindow);
#else
    nwh = (void*) (uintptr_t) glfwGetX11Window(mWindow);
#endif
#elif defined(_WIN32)
    nwh = glfwGetWin32Window(mWindow);
#elif defined(__APPLE__)
    nwh = glfwGetCocoaWindow(mWindow);
#endif
    Canvas::init(nwh, width, height);

    glfwSetWindowUserPointer(mWindow, this);

    setCallbacks();
}

CanvasWindow::CanvasWindow(uint width, uint height) :
        vcl::glfw::CanvasWindow("GLFW Canvas", width, height)
{
}

CanvasWindow::~CanvasWindow()
{
}

void CanvasWindow::show()
{
    while (!glfwWindowShouldClose(mWindow)) {
        glfwPollEvents();
        frame();
    }
}

void CanvasWindow::setCallbacks()
{
    glfwSetWindowSizeCallback(
        mWindow, [](GLFWwindow* window, int width, int height) {
            auto* self =
                static_cast<CanvasWindow*>(glfwGetWindowUserPointer(window));
            self->glfwWindowSizeCallback(window, width, height);
        });
}

void CanvasWindow::glfwWindowSizeCallback(GLFWwindow*, int width, int height)
{
    Canvas::resize(width, height);
    onResize(width, height);
}

} // namespace vcl::glfw
