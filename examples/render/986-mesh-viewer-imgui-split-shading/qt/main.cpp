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

#include <vclib/imgui/mesh_viewer_imgui_drawer.h>

#include <vclib/imgui/imgui_drawer.h>

#include <vclib/qt/widget_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/render_app.h>
#include <vclib/imgui/imgui_stats_drawer.h>

#include "../imgui_split_program_drawer.h"
#include "../program_switcher_drawable_mesh.h"
#include "../get_program_switcher_drawable_mesh.h"

#include <QApplication>

int main(int argc, char** argv)
{
    using ViewerWidget = vcl::RenderApp<
    vcl::qt::WidgetManager,
    vcl::Canvas,
    vcl::imgui::ImGuiDrawer,
    vcl::imgui::MeshViewerDrawerImgui,
    vcl::imgui::ImguiStatsDrawer,
    ImguiSplitProgramDrawer>;

    bool b = false;
    ImguiSplitProgramDrawer<ViewerWidget>::useSwitchProgram = &b;
    ProgramSwitcherDrawableMesh<vcl::TriMesh>::useSwitchProgram = &b;

    QApplication app(argc, argv);

    ViewerWidget tw("Mesh Viewer ImGui Qt");

    // load and set up a drawable mesh
    ProgramSwitcherDrawableMesh<vcl::TriMesh> drawable = 
        getProgramSwitcherDrawableMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj");

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(drawable);

    tw.fitScene();

    tw.show();

    bgfx::reset(tw.width()*tw.dpiScale().x(), tw.height()*tw.dpiScale().y(), BGFX_RESET_NONE);

    return app.exec();
}
