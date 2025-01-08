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

#include <vclib/glfw/event_manager_window.h>

#include <vclib/glfw/input.h>

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

#include <iostream>

namespace vcl::glfw {

namespace detail {

void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error: " << error << ": " << description << std::endl;
}

} // namespace detail

EventManagerWindow::EventManagerWindow(
    const std::string& windowTitle,
    uint               width,
    uint               height) : mTitle(windowTitle)
{
    glfwSetErrorCallback(detail::glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

#if defined(VCLIB_RENDER_BACKEND_BGFX)
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
#if defined(__APPLE__)
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
#endif
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
#endif

    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    mWindow =
        glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
    if (!mWindow) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
    glfwMakeContextCurrent(mWindow);
#endif

    // get content scale (e.g. for macOS retina displays)
    glfwGetWindowContentScale(mWindow, &mScaleX, &mScaleY);

    glfwSetWindowUserPointer(mWindow, this);
    setCallbacks();
}

const std::string& EventManagerWindow::windowTitle() const
{
    return mTitle;
}

void EventManagerWindow::setWindowTitle(const std::string& title)
{
    mTitle = title;
    glfwSetWindowTitle(mWindow, mTitle.c_str());
}

uint EventManagerWindow::width() const
{
    int width, height;
    glfwGetWindowSize(mWindow, &width, &height);
    return width;
}

uint EventManagerWindow::height() const
{
    int width, height;
    glfwGetWindowSize(mWindow, &width, &height);
    return height;
}

float EventManagerWindow::contentScaleX() const
{
    return mScaleX;
}

float EventManagerWindow::contentScaleY() const
{
    return mScaleY;
}

void* EventManagerWindow::winId()
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

    return nwh;
}

void* EventManagerWindow::displayId()
{
    void* ndt = nullptr;
#ifdef __linux__
#ifdef VCLIB_RENDER_WITH_WAYLAND
    ndt = (void*) (uintptr_t) glfwGetWaylandDisplay();
#else
    ndt = (void*) (uintptr_t) glfwGetX11Display();
#endif
#endif
    return ndt;
}

void EventManagerWindow::glfwFramebufferSizeCallback(
    GLFWwindow*,
    int width,
    int height)
{
    onResize(width, height);
}

void EventManagerWindow::glfwContentScaleCallback(
    GLFWwindow*,
    float xscale,
    float yscale)
{
    mScaleX = xscale;
    mScaleY = yscale;

    int width, height;
    glfwGetFramebufferSize(mWindow, &width, &height);
    onResize(width, height);
}

static int fixKeyboardMods(int key, int action, int mods)
{
    switch (key) {
    case GLFW_KEY_LEFT_SHIFT:
    case GLFW_KEY_RIGHT_SHIFT:
        return (action == GLFW_PRESS) ? mods | GLFW_MOD_SHIFT :
                                        mods & (~GLFW_MOD_SHIFT);
    case GLFW_KEY_LEFT_CONTROL:
    case GLFW_KEY_RIGHT_CONTROL:
        return (action == GLFW_PRESS) ? mods | GLFW_MOD_CONTROL :
                                        mods & (~GLFW_MOD_CONTROL);
    case GLFW_KEY_LEFT_ALT:
    case GLFW_KEY_RIGHT_ALT:
        return (action == GLFW_PRESS) ? mods | GLFW_MOD_ALT :
                                        mods & (~GLFW_MOD_ALT);
    case GLFW_KEY_LEFT_SUPER:
    case GLFW_KEY_RIGHT_SUPER:
        return (action == GLFW_PRESS) ? mods | GLFW_MOD_SUPER :
                                        mods & (~GLFW_MOD_SUPER);
    default: break;
    }

    return mods;
}

void EventManagerWindow::glfwKeyCallback(
    GLFWwindow*,
    int key,
    int,
    int action,
    int mods)
{
#if defined GLFW_EXPOSE_NATIVE_X11
    // Fix modifiers on X11
    // maybe it will be fixed https://github.com/glfw/glfw/issues/1630
    mods = fixKeyboardMods(key, action, mods);
#endif

    // GLFW modifiers are always set
    setModifiers(glfw::fromGLFW((glfw::KeyboardModifiers) mods));

    vcl::Key::Enum k = glfw::fromGLFW((glfw::Key) key);

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        onKeyPress(k);
    }
    else if (action == GLFW_RELEASE) {
        onKeyRelease(k);
    }
}

void EventManagerWindow::glfwMouseButtonCallback(
    GLFWwindow* win,
    int         button,
    int         action,
    int         mods)
{
    glfw::MouseButton btn = (glfw::MouseButton) button;

    setModifiers(glfw::fromGLFW((glfw::KeyboardModifiers) mods));

    if (action == GLFW_PRESS) {
        // handle double click
        const double timeSeconds = glfwGetTime();
        Point2d      pos;
        Point2f      scale;
        glfwGetCursorPos(win, &pos.x(), &pos.y());
#ifdef __APPLE__
        // only macOS has coherent coordinates with content scale
        pos.x() *= contentScaleX();
        pos.y() *= contentScaleY();
#endif
        if (timeSeconds - mLastPressedTime < DOUBLE_CLICK_TIME_SECS &&
            button == mLastPressedButton &&
            (mLastPressedPos - pos).norm() < DOUBLE_CLICK_DIST_PIXELS) {
            mLastPressedTime   = 0.0;
            mLastPressedButton = NO_BUTTON;
            onMouseDoubleClick(glfw::fromGLFW(btn), pos.x(), pos.y());
        }
        else {
            mLastPressedTime   = timeSeconds;
            mLastPressedButton = button;
            mLastPressedPos    = pos;
            onMousePress(glfw::fromGLFW(btn));
        }
    }
    else if (action == GLFW_RELEASE) {
        onMouseRelease(glfw::fromGLFW(btn));
    }
}

void EventManagerWindow::glfwCursorPosCallback(
    GLFWwindow*,
    double xpos,
    double ypos)
{
#ifdef __APPLE__
    // only macOS has coherent coordinates with content scale
    xpos *= contentScaleX();
    ypos *= contentScaleY();
#endif
    onMouseMove(xpos, ypos);
}

void EventManagerWindow::glfwScrollCallback(
    GLFWwindow*,
    double xoffset,
    double yoffset)
{
    // This is ok for macOS
    // TODO: check other platforms
    // TODO: check if content scale must be used
    const double ToPixelFactor = 10;
    onMouseScroll(xoffset * ToPixelFactor, yoffset * ToPixelFactor);
}

void EventManagerWindow::setCallbacks()
{
    // framebuffer size callback
    glfwSetFramebufferSizeCallback(
        mWindow, [](GLFWwindow* window, int width, int height) {
            auto* self = static_cast<EventManagerWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwFramebufferSizeCallback(window, width, height);
        });

    // content scale callback
    glfwSetWindowContentScaleCallback(
        mWindow, [](GLFWwindow* window, float xscale, float yscale) {
            auto* self = static_cast<EventManagerWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwContentScaleCallback(window, xscale, yscale);
        });

    // key callback
    glfwSetKeyCallback(
        mWindow,
        [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto* self = static_cast<EventManagerWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwKeyCallback(window, key, scancode, action, mods);
        });

    // mouse position callback
    glfwSetCursorPosCallback(
        mWindow, [](GLFWwindow* window, double xpos, double ypos) {
            auto* self = static_cast<EventManagerWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwCursorPosCallback(window, xpos, ypos);
        });

    // mouse button callback
    glfwSetMouseButtonCallback(
        mWindow, [](GLFWwindow* window, int button, int action, int mods) {
            auto* self = static_cast<EventManagerWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwMouseButtonCallback(window, button, action, mods);
        });

    // scroll callback
    glfwSetScrollCallback(
        mWindow, [](GLFWwindow* window, double xoffset, double yoffset) {
            auto* self = static_cast<EventManagerWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwScrollCallback(window, xoffset, yoffset);
        });
}

} // namespace vcl::glfw
