// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../01-hello-triangle-imgui-common/demo_imgui_drawer.h"
#include "../01-hello-triangle-imgui-opengl2-common/hello_triangle_drawer.h"

#include <vclib/imgui/imgui_stats_drawer.h>

#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
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
        HelloTriangleDrawer,      // The Drawer that draws the triangle
        vcl::imgui::ImguiStatsDrawer>; // A Drawer that draws the ImGui stats
                                       // window

    WindowGLFW tw("Hello Triangle ImGui with GLFW");

    tw.show();

    return 0;
}
