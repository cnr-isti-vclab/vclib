// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_GLFW_DETAIL_WINDOW_MANAGER_NATIVE_H
#define VCL_GLFW_DETAIL_WINDOW_MANAGER_NATIVE_H

#include <GLFW/glfw3.h>

namespace vcl::glfw {

namespace detail {

class WindowManagerNative
{
public:
    static void* winId(GLFWwindow* window);
    static void* displayId();
};

} // namespace detail

/**
 * @brief Utility function to retrieve the display ID used by the GLFW window
 * manager. This function checks whether the system is using X11 or Wayland and
 * retrieves the display ID accordingly.
 *
 * This function is specific for Linux systems and returns a pointer to the
 * display ID. If the system is not using X11 or Wayland, or if the display ID
 * cannot be retrieved, it returns nullptr.
 *
 * @return a pointer to the display ID, or nullptr if the display ID cannot be
 * retrieved.
 */
inline void* getDisplayId()
{
    return detail::WindowManagerNative::displayId();
}

} // namespace vcl::glfw

#endif // VCL_GLFW_DETAIL_WINDOW_MANAGER_NATIVE_H
