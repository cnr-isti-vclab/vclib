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

#if defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#elif defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3native.h>

#include <vclib/ext/glfw/gui/input.h>

namespace vcl::bglfwx {

namespace detail {

void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error: " << error << ": " << description << std::endl;
}

} // namespace detail

CanvasWindow::CanvasWindow(
    const std::string& windowTitle,
    uint               width,
    uint               height) :
        title(windowTitle)
{
    glfwSetErrorCallback(detail::glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window =
        glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    void* ndt = nullptr;
    void* nwh = nullptr;

#if defined(__linux__)
    ndt = glfwGetX11Display();
    nwh = (void*) (uintptr_t) glfwGetX11Window(window);
#elif defined(_WIN32)
    nwh = glfwGetWin32Window(window);
#elif defined(__APPLE__)
    nwh = glfwGetCocoaWindow(window);
#endif
    Canvas::init(nwh, width, height, ndt);

    glfwSetWindowUserPointer(window, this);

    setCallbacks();
}

CanvasWindow::CanvasWindow(uint width, uint height) :
        vcl::bglfwx::CanvasWindow("GLFW Canvas", width, height)
{
}

CanvasWindow::~CanvasWindow()
{
}

const std::string& CanvasWindow::windowTitle() const
{
    return title;
}

void CanvasWindow::setWindowTitle(const std::string& wTitle)
{
    title = wTitle;
    glfwSetWindowTitle(window, title.c_str());
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

void CanvasWindow::setCallbacks()
{
    glfwSetWindowSizeCallback(
        window, [](GLFWwindow* window, int width, int height) {
            auto* self =
                static_cast<CanvasWindow*>(glfwGetWindowUserPointer(window));
            self->glfwWindowSizeCallback(window, width, height);
        });

    // key callback lambda
    auto keyCB =
        [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto* self = static_cast<CanvasWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwKeyCallback(window, key, scancode, action, mods);
        };

    glfwSetKeyCallback(window, keyCB);

           // mouse position callback
    glfwSetCursorPosCallback(
        window, [](GLFWwindow* window, double xpos, double ypos) {
            auto* self = static_cast<CanvasWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwCursorPosCallback(window, xpos, ypos);
        });

           // mouse button callback
    glfwSetMouseButtonCallback(
        window, [](GLFWwindow* window, int button, int action, int mods) {
            auto* self = static_cast<CanvasWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwMouseButtonCallback(window, button, action, mods);
        });

           // scroll callback
    glfwSetScrollCallback(
        window, [](GLFWwindow* window, double xoffset, double yoffset) {
            auto* self = static_cast<CanvasWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwScrollCallback(window, xoffset, yoffset);
        });
}

void CanvasWindow::glfwWindowSizeCallback(GLFWwindow*, int width, int height)
{
    Canvas::resize(width, height);
    onResize(width, height);
}

void CanvasWindow::glfwKeyCallback(
    GLFWwindow*,
    int key,
    int,
    int action,
    int mods)
{
    KeyModifiers modifiers = glfw::fromGLFW((glfw::KeyboardModifiers) mods);
    Key::Enum k = glfw::fromGLFW((glfw::Key) key);
    if (action == GLFW_PRESS) {
        onKeyPress(k, modifiers);
    } else if (action == GLFW_RELEASE) {
        onKeyRelease(k, modifiers);
    }
}

void CanvasWindow::glfwMouseButtonCallback(
    GLFWwindow*,
    int button,
    int action,
    int mods)
{
    glfw::MouseButton btn = (glfw::MouseButton) button;

    if (action == GLFW_PRESS) {
        onMousePress(glfw::fromGLFW(btn));
    } else if (action == GLFW_RELEASE) {
        onMouseRelease(glfw::fromGLFW(btn));
    }
}

void CanvasWindow::glfwCursorPosCallback(GLFWwindow*, double xpos, double ypos)
{
    onMouseMove(xpos, ypos);
}

void CanvasWindow::glfwScrollCallback(GLFWwindow*, double xoffset, double yoffset)
{
    onMouseScroll(xoffset, yoffset);
}

// namespace detail

} // namespace vcl::bglfwx
