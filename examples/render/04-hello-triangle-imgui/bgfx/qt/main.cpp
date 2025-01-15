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
#include <vclib/qt/widget_manager.h>
#include <vclib/render/render_app.h>

#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    // FIXME #2: crash on linux when closing: derived()->update() in imgui_drawer.h

    using WidgetQt = vcl::RenderApp<
        vcl::qt::WidgetManager,   // The WindowManager: Qt
        vcl::Canvas,              // The default Canvas
        DemoImGuiDrawer,          // A Drawer that draws the ImGui demo window
        HelloTriangleDrawer,      // The Drawer that draws the triangle
        ImguiStatsDrawer>;        // A Drawer that draws the ImGui stats window

    WidgetQt tw("Hello Triangle ImGui with Qt");

    tw.show();

    return app.exec();
}
