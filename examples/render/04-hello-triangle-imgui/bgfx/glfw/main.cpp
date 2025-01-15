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

#include "../../demo_imgui_drawer.h"
#include "../../imgui_stats_drawer.h"
#include "../hello_triangle_drawer.h"

#include <vclib/render/canvas.h>
#include <vclib/glfw/window_manager.h>
#include <vclib/render/render_app.h>

/**
 * This Demo is a simple example of how to use the ImGuiDrawer to draw the ImGui
 * demo window.
 *
 * The example is the same as 00-hello-triangle, but with the addition of the
 * ImGui demo window.
 */
int main(int argc, char** argv)
{
    // The window is created with the RenderApp class:
    using WindowGLFW = vcl::RenderApp<
        vcl::glfw::WindowManager, // The WindowManager: GLFW
        vcl::Canvas,              // The default Canvas
        DemoImGuiDrawer,          // A Drawer that draws the ImGui demo window
        HelloTriangleDrawer,     // The Drawer that draws the triangle
        ImguiStatsDrawer>;          // A Drawer that draws the ImGui stats window
    WindowGLFW tw("Hello Triangle ImGui with GLFW");

    tw.show();

    return 0;
}
