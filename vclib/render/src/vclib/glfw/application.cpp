/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#include <vclib/glfw/application.h>
#include <algorithm>
#include <iostream>

namespace vcl::glfw {

namespace detail {

inline void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error: " << error << ": " << description << std::endl;
}

} // namespace detail

// Centralized static GLFW callbacks
void GLFWApplication::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    auto* wm = static_cast<WindowManagerBase*>(glfwGetWindowUserPointer(window));
    if (wm) wm->onGlfwFramebufferSize(width, height);
}

void GLFWApplication::contentScaleCallback(GLFWwindow* window, float xscale, float yscale) {
    auto* wm = static_cast<WindowManagerBase*>(glfwGetWindowUserPointer(window));
    if (wm) wm->onGlfwContentScale(xscale, yscale);
}

void GLFWApplication::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* wm = static_cast<WindowManagerBase*>(glfwGetWindowUserPointer(window));
    if (wm) wm->onGlfwKey(key, scancode, action, mods);
}

void GLFWApplication::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto* wm = static_cast<WindowManagerBase*>(glfwGetWindowUserPointer(window));
    if (wm) wm->onGlfwMouseButton(button, action, mods);
}

void GLFWApplication::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    auto* wm = static_cast<WindowManagerBase*>(glfwGetWindowUserPointer(window));
    if (wm) wm->onGlfwCursorPos(xpos, ypos);
}

void GLFWApplication::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    auto* wm = static_cast<WindowManagerBase*>(glfwGetWindowUserPointer(window));
    if (wm) wm->onGlfwScroll(xoffset, yoffset);
}

GLFWApplication& GLFWApplication::instance() {
    static GLFWApplication app;
    return app;
}

GLFWApplication::GLFWApplication() {
    glfwSetErrorCallback(detail::glfwErrorCallback);
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    mInitialized = true;
}

GLFWApplication::~GLFWApplication() {
    if (mInitialized) {
        glfwTerminate();
        mInitialized = false;
    }
}

void GLFWApplication::registerWindow(WindowManagerBase* win) {
    mWindows.insert(win);
    
    GLFWwindow* window = win->glfwWindow();
    glfwSetWindowUserPointer(window, win);
    
    // Set all GLFW callbacks for this window
    glfwSetFramebufferSizeCallback(window, &GLFWApplication::framebufferSizeCallback);
    glfwSetWindowContentScaleCallback(window, &GLFWApplication::contentScaleCallback);
    glfwSetKeyCallback(window, &GLFWApplication::keyCallback);
    glfwSetCursorPosCallback(window, &GLFWApplication::cursorPosCallback);
    glfwSetMouseButtonCallback(window, &GLFWApplication::mouseButtonCallback);
    glfwSetScrollCallback(window, &GLFWApplication::scrollCallback);
}

void GLFWApplication::unregisterWindow(WindowManagerBase* win) {
    mWindows.erase(win);
}

size_t GLFWApplication::windowCount() const {
    return mWindows.size();
}

int GLFWApplication::exec() {
    return instance().executeMainLoop();
}

int GLFWApplication::executeMainLoop() {
    while (!mWindows.empty()) {
        glfwPollEvents();
        std::erase_if(mWindows, [](WindowManagerBase* win) {
            return win->shouldClose();
        });
        std::for_each(mWindows.begin(), mWindows.end(),
            [](WindowManagerBase* win) {
                win->paint();
        });
    }
    return 0;
}

} // namespace vcl::glfw
