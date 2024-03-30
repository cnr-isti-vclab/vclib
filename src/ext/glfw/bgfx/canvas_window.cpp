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

#include <vclib/ext/glfw/bgfx/canvas_window.h>

#include <iostream>

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

    void* nwh = nullptr;

#if defined(__linux__)
#ifdef VCLIB_RENDER_WITH_WAYLAND
    nwh = (void*) (uintptr_t) glfwGetWaylandWindow(window);
#else
    nwh = (void*) (uintptr_t) glfwGetX11Window(window);
#endif
#elif defined(_WIN32)
    nwh = glfwGetWin32Window(window);
#elif defined(__APPLE__)
    nwh = glfwGetCocoaWindow(window);
#endif
    Canvas::init(nwh, width, height);

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

    // key callback
    glfwSetKeyCallback(
        window,
        [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto* self =
                static_cast<CanvasWindow*>(glfwGetWindowUserPointer(window));
            self->glfwKeyCallback(window, key, scancode, action, mods);
        });

    // mouse position callback
    glfwSetCursorPosCallback(
        window, [](GLFWwindow* window, double xpos, double ypos) {
            auto* self =
                static_cast<CanvasWindow*>(glfwGetWindowUserPointer(window));
            self->glfwCursorPosCallback(window, xpos, ypos);
        });

    // mouse button callback
    glfwSetMouseButtonCallback(
        window, [](GLFWwindow* window, int button, int action, int mods) {
            auto* self =
                static_cast<CanvasWindow*>(glfwGetWindowUserPointer(window));
            self->glfwMouseButtonCallback(window, button, action, mods);
        });

    // scroll callback
    glfwSetScrollCallback(
        window, [](GLFWwindow* window, double xoffset, double yoffset) {
            auto* self =
                static_cast<CanvasWindow*>(glfwGetWindowUserPointer(window));
            self->glfwScrollCallback(window, xoffset, yoffset);
        });
}

void CanvasWindow::glfwWindowSizeCallback(GLFWwindow*, int width, int height)
{
    Canvas::resize(width, height);
    onResize(width, height);
}

void CanvasWindow::glfwKeyCallback(GLFWwindow*, int key, int, int action, int)
{
    Key::Enum k = glfw::fromGLFW((glfw::Key) key);
    // GLFW modifier does not work as expected: modifiers are not updated
    // when a key modifier is released. We have to handle this manually.
    if (action == GLFW_PRESS) {
        if (isModifierKey(k)) {
            KeyModifiers mods                 = modifiers();
            mods.at(KeyModifier::NO_MODIFIER) = false;
            mods.at(keyToModifier(k))         = true;
            setModifiers(mods);
        }
        onKeyPress(k);
    }
    else if (action == GLFW_RELEASE) {
        if (isModifierKey(k)) {
            KeyModifiers mods      = modifiers();
            mods[keyToModifier(k)] = false;
            if (mods.none())
                mods[KeyModifier::NO_MODIFIER] = true;
            setModifiers(mods);
        }
        onKeyRelease(k);
    }
}

void CanvasWindow::glfwMouseButtonCallback(
    GLFWwindow*,
    int button,
    int action,
    int)
{
    glfw::MouseButton btn = (glfw::MouseButton) button;

    if (action == GLFW_PRESS) {
        onMousePress(glfw::fromGLFW(btn));
    }
    else if (action == GLFW_RELEASE) {
        onMouseRelease(glfw::fromGLFW(btn));
    }
}

void CanvasWindow::glfwCursorPosCallback(GLFWwindow*, double xpos, double ypos)
{
    onMouseMove(xpos, ypos);
}

void CanvasWindow::glfwScrollCallback(
    GLFWwindow*,
    double xoffset,
    double yoffset)
{
    onMouseScroll(xoffset, yoffset);
}

// namespace detail

} // namespace vcl::bglfwx
