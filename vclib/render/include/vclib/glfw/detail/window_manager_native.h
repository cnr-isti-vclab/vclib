// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_GLFW_WINDOW_MANAGER_NATIVE_H
#define VCL_GLFW_WINDOW_MANAGER_NATIVE_H

#include <GLFW/glfw3.h>

namespace vcl::glfw::detail {

class WindowManagerNative
{
public:
    static void* winId(GLFWwindow* window);
    static void* displayId();
};

} // namespace vcl::glfw::detail

#endif // VCL_GLFW_WINDOW_MANAGER_NATIVE_H
