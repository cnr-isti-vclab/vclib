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

#include <vclib/ext/glfw/bgfx/canvas_window.h>

#include <iostream>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3native.h>

namespace vcl::bglfwx {

namespace detail {

inline void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error: " << error << ": " << description << std::endl;
}

} // namespace detail

CanvasWindow::CanvasWindow(uint width, uint height)
{
    glfwSetErrorCallback(detail::glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(width, height, "VCL", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    void* ndt = nullptr;
    void* nwh = nullptr;

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    ndt = glfwGetX11Display();
    nwh = (void*) (uintptr_t) glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
    nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
    nwh = glfwGetWin32Window(window);
#endif
    Canvas::init(nwh, width, height, ndt);

    glfwSetWindowUserPointer(window, this);

    glfwSetWindowSizeCallback(
        window, [](GLFWwindow* window, int width, int height) {
            auto* self =
                static_cast<CanvasWindow*>(glfwGetWindowUserPointer(window));
            self->glfwWindowSizeCallback(window, width, height);
        });
}

CanvasWindow::~CanvasWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

uint CanvasWindow::width() const
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return width;
}

uint CanvasWindow::height() const
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return height;
}

void CanvasWindow::show()
{
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        frame();
    }
}

void CanvasWindow::glfwWindowSizeCallback(GLFWwindow*, int width, int height)
{
    Canvas::resize(width, height);
    this->onResize(width, height);
}

// namespace detail

} // namespace vcl::bglfwx
