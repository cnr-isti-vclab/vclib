// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/glfw/detail/window_manager_native.h>

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
#include <cstdint>

namespace vcl::glfw::detail {

void* WindowManagerNative::winId(GLFWwindow* window)
{
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

    return nwh;
}

void* WindowManagerNative::displayId()
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

} // namespace vcl::glfw::detail
