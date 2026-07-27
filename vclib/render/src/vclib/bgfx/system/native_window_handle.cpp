// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/system/native_window_handle.h>

#include <cassert>
#include <iostream>

namespace vcl {

#ifdef __APPLE__
void* createWindow(
    const char* title,
    int         width,
    int         height,
    void*&      display,
    bool        hidden)
{
    (void) display;

    return detail::cretateCocoaWindow(title, width, height, hidden);
}

void* createWindow(const char* title, int width, int height, bool hidden)
{
    void* display = nullptr;
    return createWindow(title, width, height, display, hidden);
}

void closeWindow(void* window, void* display)
{
    detail::closeCocoaWindow(window);
}
#endif // __APPLE__

} // namespace vcl
