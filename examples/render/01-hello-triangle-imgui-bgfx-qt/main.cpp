// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../01-hello-triangle-imgui-common/demo_imgui_drawer.h"
#include "../01-hello-triangle-imgui-bgfx-common/hello_triangle_drawer.h"

#include <vclib/imgui/imgui_stats_drawer.h>

#include <vclib/qt/widget_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/render_app.h>

int main(int argc, char** argv)
{
    auto app = vcl::qt::qAppl(argc, argv);

    using WidgetQt = vcl::RenderApp<
        vcl::qt::WidgetManager, // The WindowManager: Qt
        vcl::Canvas,            // The default Canvas
        DemoImGuiDrawer,        // A Drawer that draws the ImGui demo window
        HelloTriangleDrawer,    // The Drawer that draws the triangle
        vcl::imgui::ImguiStatsDrawer>; // A Drawer that draws the ImGui stats
                                       // window

    WidgetQt tw("Hello Triangle ImGui with Qt");

    tw.show();

    return app.exec();
}
