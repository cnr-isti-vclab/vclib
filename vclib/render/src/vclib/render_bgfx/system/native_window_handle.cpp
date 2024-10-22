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

#include <vclib/render_bgfx/system/native_window_handle.h>

#include <cassert>
#include <iostream>

#ifdef _WIN32
#ifndef UNICODE
#define VCLIB_DEF_UNICODE
#define UNICODE
#endif
#include <tchar.h>
#include <windows.h>
#ifdef VCLIB_DEF_UNICODE
#undef UNICODE
#undef VCLIB_DEF_UNICODE
#endif
#elif __linux__
#ifdef VCLIB_RENDER_WITH_WAYLAND
#include <wayland-client.h>
#include <wayland-egl.h>
#else
#include <X11/Xlib.h>
#endif
#endif

namespace vcl {

void* createWindow(
    const char* title,
    int         width,
    int         height,
    void*&      display,
    bool        hidden)
{
#ifdef _WIN32
    (void) display;

    wchar_t wtext[256];
    size_t  sz;
    mbstowcs_s(&sz, wtext, title, strlen(title) + 1); // Plus null
    LPCWSTR ptr = wtext;

    WNDCLASS wc      = {};
    wc.lpfnWndProc   = DefWindowProc;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.lpszClassName = _T("MyWindowClass");

    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(
        0,
        _T("MyWindowClass"),
        ptr,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL);

    if (hidden)
        ShowWindow(hWnd, SW_HIDE);

    return (void*) hWnd;
#elif __APPLE__
    (void) display;

    return detail::cretateCocoaWindow(title, width, height, hidden);

#else

#ifdef VCLIB_RENDER_WITH_WAYLAND
    wl_display* dspl = wl_display_connect(NULL);
    if (!dspl) {
        std::cerr << "Failed to connect to Wayland display." << std::endl;
        assert(0);
        return nullptr;
    }
    display = dspl;
    // create wayland surface
    wl_surface* surface = wl_compositor_create_surface(
        static_cast<wl_compositor*>(wl_registry_bind(
            wl_display_get_registry(dspl), 1, &wl_compositor_interface, 1)));
    if (!surface) {
        std::cerr << "Failed to create Wayland surface." << std::endl;
        assert(0);
        return nullptr;
    }
    wl_egl_window* winImpl = wl_egl_window_create(surface, 1, 1);
    if (!hidden) {
        wl_surface_commit(surface);
        wl_display_roundtrip(dspl);
    }
    return (void*) winImpl;
#else  // X11
    Display* dspl = XOpenDisplay(NULL);
    if (!dspl) {
        std::cerr << "Failed to Open X display." << std::endl;
        assert(0);
        return nullptr;
    }

    int screen = DefaultScreen(dspl);

    Window window = XCreateSimpleWindow(
        dspl,
        RootWindow(dspl, screen),
        0,
        0,
        width,
        height,
        0,
        BlackPixel(dspl, screen),
        WhitePixel(dspl, screen));

    if (!window) {
        std::cerr << "Failed to create X window." << std::endl;
        assert(0);
        return nullptr;
    }

    XStoreName(dspl, window, title);

    if (!hidden)
        XMapWindow(dspl, window);

    XSync(dspl, False);

    display = (void*) dspl;

    return (void*) window;
#endif // VCLIB_RENDER_WITH_WAYLAND
#endif // _WIN32
}

void* createWindow(const char* title, int width, int height, bool hidden)
{
    void* display = nullptr;
    return createWindow(title, width, height, display, hidden);
}

void closeWindow(void* window, void* display)
{
#ifdef _WIN32
    DestroyWindow((HWND) window);
#elif __APPLE__
    detail::closeCocoaWindow(window);
#else // linux

#ifdef VCLIB_RENDER_WITH_WAYLAND
    wl_display_disconnect((wl_display*) display);
#else  // X11
    XDestroyWindow((Display*) display, (Window) window);
#endif // VCLIB_RENDER_WITH_WAYLAND
#endif
}

} // namespace vcl
