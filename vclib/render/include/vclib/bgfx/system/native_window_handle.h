// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_SYSTEM_NATIVE_WINDOW_HANDLE_H
#define VCL_BGFX_SYSTEM_NATIVE_WINDOW_HANDLE_H

namespace vcl {

#ifdef __APPLE__
namespace detail {

void* cretateCocoaWindow(
    const char* title,
    int         width,
    int         height,
    bool        hidden = false);

void closeCocoaWindow(void* window);

} // namespace detail
#endif // __APPLE__

void* createWindow(
    const char* title,
    int         width,
    int         height,
    void*&      display,
    bool        hidden = false);

void* createWindow(
    const char* title,
    int         width,
    int         height,
    bool        hidden = false);

void closeWindow(void* window, void* display = nullptr);

} // namespace vcl

#endif // VCL_BGFX_SYSTEM_NATIVE_WINDOW_HANDLE_H
