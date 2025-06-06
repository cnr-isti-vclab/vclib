/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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
