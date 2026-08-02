// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_GLFW_APPLICATION_H
#define VCL_GLFW_APPLICATION_H

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/context.h>
#endif

#include <GLFW/glfw3.h>

namespace vcl::glfw {

/**
 * @brief Application class for GLFW backend.
 *
 * This class initializes GLFW upon construction and terminates it upon
 * destruction. If the BGFX render backend is enabled, it also shuts down
 * the rendering context properly before destroying the application.
 */
class Application
{
public:
    Application(int& /*argc*/, char** /*argv*/) { glfwInit(); }

    ~Application()
    {
#ifdef VCLIB_RENDER_BACKEND_BGFX
        vcl::Context::shutdown();
#endif
        glfwTerminate();
    }

    int exec() { return 0; }
};

} // namespace vcl::glfw

#endif // VCL_GLFW_APPLICATION_H
