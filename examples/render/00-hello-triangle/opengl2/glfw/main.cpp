// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../hello_triangle_drawer.h"

// may include qt - must be included before glfw...
#include <vclib/render/canvas.h>

#include <vclib/glfw/window_manager.h>
#include <vclib/render/render_app.h>

int main(int argc, char** argv)
{
    using WindowGLFW = vcl::
        RenderApp<vcl::glfw::WindowManager, vcl::Canvas, HelloTriangleDrawer>;

    WindowGLFW tw("Hello Triangle with GLFW");

    tw.show();

    return 0;
}
