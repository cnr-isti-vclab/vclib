// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/glfw/application.h>

#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace vcl::glfw {

namespace detail {

inline void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error: " << error << ": " << description << std::endl;
}

} // namespace detail

Application* Application::s_instance = nullptr;

// Centralized static GLFW callbacks
void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    auto* wm = static_cast<WindowManagerBase*>(glfwGetWindowUserPointer(window));
    if (wm) wm->onGlfwFramebufferSize(width, height);
}

void Application::contentScaleCallback(GLFWwindow* window, float xscale, float yscale) {
    auto* wm = static_cast<WindowManagerBase*>(glfwGetWindowUserPointer(window));
    if (wm) wm->onGlfwContentScale(xscale, yscale);
}

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* wm = static_cast<WindowManagerBase*>(glfwGetWindowUserPointer(window));
    if (wm) wm->onGlfwKey(key, scancode, action, mods);
}

void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto* wm = static_cast<WindowManagerBase*>(glfwGetWindowUserPointer(window));
    if (wm) wm->onGlfwMouseButton(button, action, mods);
}

void Application::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    auto* wm = static_cast<WindowManagerBase*>(glfwGetWindowUserPointer(window));
    if (wm) wm->onGlfwCursorPos(xpos, ypos);
}

void Application::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    auto* wm = static_cast<WindowManagerBase*>(glfwGetWindowUserPointer(window));
    if (wm) wm->onGlfwScroll(xoffset, yoffset);
}

Application* Application::instance() {
    return s_instance;
}

Application::Application(int& /*argc*/, char** /*argv*/) {
    if (s_instance) {
        throw std::runtime_error("vcl::glfw::Application can only be instantiated once!");
    }
    s_instance = this;
    glfwSetErrorCallback(detail::glfwErrorCallback);
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
}

Application::~Application() {
#ifdef VCLIB_RENDER_BACKEND_BGFX
    vcl::Context::shutdown();
#endif
    glfwTerminate();
    s_instance = nullptr;
}

void Application::registerWindow(WindowManagerBase* win) {
    mWindows.insert(win);
    
    GLFWwindow* window = win->glfwWindow();
    glfwSetWindowUserPointer(window, win);
    
    // Set all GLFW callbacks for this window
    glfwSetFramebufferSizeCallback(window, &Application::framebufferSizeCallback);
    glfwSetWindowContentScaleCallback(window, &Application::contentScaleCallback);
    glfwSetKeyCallback(window, &Application::keyCallback);
    glfwSetCursorPosCallback(window, &Application::cursorPosCallback);
    glfwSetMouseButtonCallback(window, &Application::mouseButtonCallback);
    glfwSetScrollCallback(window, &Application::scrollCallback);
}

void Application::unregisterWindow(WindowManagerBase* win) {
    mWindows.erase(win);
}

int Application::exec() {
    while (!mWindows.empty()) {
        glfwPollEvents();
        
        for (auto it = mWindows.begin(); it != mWindows.end(); ) {
            WindowManagerBase* win = *it;
            if (win->shouldClose()) {
                it = mWindows.erase(it);
            } else {
                ++it;
            }
        }
        
        std::for_each(mWindows.begin(), mWindows.end(),
            [](WindowManagerBase* win) {
                win->paint();
        });
    }
    return 0;
}

} // namespace vcl::glfw
