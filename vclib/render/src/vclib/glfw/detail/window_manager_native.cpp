// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/glfw/detail/window_manager_native.h>

#if defined(__linux__)
#define GLFW_EXPOSE_NATIVE_WAYLAND
#define GLFW_EXPOSE_NATIVE_X11
#elif defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3native.h>
#include <cstdint>

namespace vcl::glfw::detail {

void* WindowManagerNative::winId(GLFWwindow* window)
{
    void* nwh = nullptr;

#if defined(__linux__)
    int platform = glfwGetPlatform();
    if (platform == GLFW_PLATFORM_WAYLAND) {
        nwh = (void*) (uintptr_t) glfwGetWaylandWindow(window);
    }
    else if (platform == GLFW_PLATFORM_X11) {
        nwh = (void*) (uintptr_t) glfwGetX11Window(window);
    }
#elif defined(_WIN32)
    nwh = glfwGetWin32Window(window);
#elif defined(__APPLE__)
    nwh = glfwGetCocoaWindow(window);
#endif

    return nwh;
}

void* WindowManagerNative::displayId()
{
    void* ndt = nullptr;
#ifdef __linux__
    int platform = glfwGetPlatform();
    if (platform == GLFW_PLATFORM_WAYLAND) {
        ndt = (void*) (uintptr_t) glfwGetWaylandDisplay();
    }
    else if (platform == GLFW_PLATFORM_X11) {
        ndt = (void*) (uintptr_t) glfwGetX11Display();
    }
#endif
    return ndt;
}

vcl::NativeWindowHandleType WindowManagerNative::handleType()
{
#ifdef __linux__
    int platform = glfwGetPlatform();
    if (platform == GLFW_PLATFORM_WAYLAND) {
        return vcl::NativeWindowHandleType::WAYLAND;
    }
#endif
    return vcl::NativeWindowHandleType::DEFAULT;
}

} // namespace vcl::glfw::detail
