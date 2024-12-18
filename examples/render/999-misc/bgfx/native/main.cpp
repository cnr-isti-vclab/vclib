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

/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */
#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <stdio.h>
#if BX_PLATFORM_LINUX
#ifdef VCLIB_RENDER_WITH_WAYLAND
#include <wayland-egl.h>
#define GLFW_EXPOSE_NATIVE_WAYLAND
#else
#define GLFW_EXPOSE_NATIVE_X11
#endif
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include "logo.h"
#include <GLFW/glfw3native.h>

static bool s_showStats = false;

static void glfw_errorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

static void glfw_keyCallback(GLFWwindow*, int key, int, int action, int)
{
    if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
        s_showStats = !s_showStats;
}

int main(int, char**)
{
    // Create a GLFW window without an OpenGL context.
    glfwSetErrorCallback(glfw_errorCallback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window =
        glfwCreateWindow(1024, 768, "helloworld", nullptr, nullptr);
    if (!window)
        return 1;
    glfwSetKeyCallback(window, glfw_keyCallback);
    // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create
    // a render thread. Most graphics APIs must be used on the same thread that
    // created the window.
    bgfx::renderFrame();
    // Initialize bgfx using the native window handle and window resolution.
    bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#ifdef VCLIB_RENDER_WITH_WAYLAND
    // Get Native Window Handle - For Wayland
    struct wl_surface* surface =
        (struct wl_surface*) glfwGetWaylandWindow(window);
    if (!surface) {
        glfwTerminate();
        return EXIT_FAILURE;
    }
    wl_egl_window* winImpl = wl_egl_window_create(surface, 1024, 768);
    glfwSetWindowUserPointer(window, (void*) (uintptr_t) winImpl);
    init.platformData.ndt = glfwGetWaylandDisplay();
    init.platformData.nwh = (void*) (uintptr_t) winImpl;
#else
    init.platformData.ndt = glfwGetX11Display();
    init.platformData.nwh = (void*) (uintptr_t) glfwGetX11Window(window);
#endif
#elif BX_PLATFORM_OSX
    init.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
    init.platformData.nwh = glfwGetWin32Window(window);
#endif
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    width *= 2;
    height *= 2;
#ifdef VCLIB_RENDER_WITH_WAYLAND
    wl_egl_window_resize(winImpl, width, height, 0, 0);
#endif
    init.resolution.width  = (uint32_t) width;
    init.resolution.height = (uint32_t) height;
    init.resolution.reset  = BGFX_RESET_VSYNC;
    if (!bgfx::init(init))
        return 1;
    // Set view 0 to the same dimensions as the window and to clear the color
    // buffer.
    const bgfx::ViewId kClearView = 0;
    bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
    bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        // Handle window resize.
        int oldWidth = width, oldHeight = height;
        glfwGetWindowSize(window, &width, &height);
        width *= 2;
        height *= 2;
        if (width != oldWidth || height != oldHeight) {
#ifdef VCLIB_RENDER_WITH_WAYLAND
            wl_egl_window_resize(winImpl, width, height, 0, 0);
#endif
            bgfx::reset((uint32_t) width, (uint32_t) height, BGFX_RESET_VSYNC);
            bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
        }
        // This dummy draw call is here to make sure that view 0 is cleared if
        // no other draw calls are submitted to view 0.
        bgfx::touch(kClearView);
        // Use debug font to print information about this example.
        bgfx::dbgTextClear();
        bgfx::dbgTextImage(
            bx::max<uint16_t>(uint16_t(width / 2 / 8), 20) - 20,
            bx::max<uint16_t>(uint16_t(height / 2 / 16), 6) - 6,
            40,
            12,
            s_logo,
            160);
        bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
        bgfx::dbgTextPrintf(
            0,
            1,
            0x0f,
            "Color can be changed with ANSI "
            "\x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b["
            "0m code too.");
        bgfx::dbgTextPrintf(
            80,
            1,
            0x0f,
            "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    "
            "\x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
        bgfx::dbgTextPrintf(
            80,
            2,
            0x0f,
            "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    "
            "\x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
        const bgfx::Stats* stats = bgfx::getStats();
        bgfx::dbgTextPrintf(
            0,
            2,
            0x0f,
            "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in "
            "characters.",
            stats->width,
            stats->height,
            stats->textWidth,
            stats->textHeight);
        // Enable stats or debug text.
        bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);
        // Advance to next frame. Process submitted rendering primitives.
        bgfx::frame();
    }
    bgfx::shutdown();
    glfwTerminate();
    return 0;
}
