// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "get_drawable_mesh.h"

#include <vclib/imgui/imgui_drawer.h>
#include <vclib/imgui/imgui_stats_drawer.h>
#include <vclib/qt/widget_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/trackball_viewer_drawer.h>
#include <vclib/render/render_app.h>

int main(int argc, char** argv)
{
    auto app = vcl::qt::qAppl(argc, argv);

    using ImGuiDemo = vcl::RenderApp<
        vcl::qt::WidgetManager,
        vcl::Canvas,
        vcl::imgui::ImGuiDrawer,
        vcl::TrackBallViewerDrawer,
        vcl::imgui::ImguiStatsDrawer>;

    ImGuiDemo tw("Viewer ImGui Qt");

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::TriMesh> drawable = getDrawableMesh<vcl::TriMesh>();

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(drawable);

    tw.fitScene();

    tw.show();

    return app.exec();
}
