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

#include <vclib/gui/native_window_handle.h>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <X11/Xlib.h>
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
    LPWSTR ptr = wtext;

    WNDCLASS wc      = {};
    wc.lpfnWndProc   = DefWindowProc;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.lpszClassName = L"MyWindowClass";

    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(
        0,
        L"MyWindowClass",
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
    Display* dspl = XOpenDisplay(NULL);
    if (!dspl) {
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

    XStoreName(dspl, window, title);

    if (!hidden)
        XMapWindow(dspl, window);

    XSync(dspl, False);

    display = (void*) dspl;

    return (void*) window;
#endif
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
#else
    XDestroyWindow((Display*) display, (Window) window);
#endif
}

} // namespace vcl
