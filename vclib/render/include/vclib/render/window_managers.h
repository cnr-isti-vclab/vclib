// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_WINDOW_MANAGERS_H
#define VCL_RENDER_WINDOW_MANAGERS_H

namespace vcl {

/**
 * @brief The WindowManagerId struct enumerates the window managers that can be
 * used to create and manage a window and its events.
 *
 * The window managers are identified by an unsigned integer value, that
 * corresponds to a specific window manager implementation.
 */
struct WindowManagerId
{
    enum Enum {
        GLFW_WINDOW,
        QT_WIDGET,
        QT_WINDOW,
        // Additional window managers here

        WINDOW_MANAGER_COUNT
    };
};

} // namespace vcl

#endif // VCL_RENDER_WINDOW_MANAGERS_H
