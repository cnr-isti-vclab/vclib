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

#include "get_drawable_mesh.h"

#include <vclib/imgui/imgui_drawer.h>

#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/algorithms/mesh/update/transform.h>
#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/render_app.h>

#include "../../automation_imgui_drawer.h"
#include "../../rotation_automation_action.h"
#include "../../benchmark_drawer.h"
#include "../../time_limited_automation_action.h"
#include "../../per_frame_rotation_automation_action.h"
#include "../../scale_automation_action.h"

#include <imgui.h>

int main(int argc, char** argv)
{
    using ImguiMeshViewer = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        vcl::imgui::ImGuiDrawer,
        vcl::imgui::MeshViewerDrawerImgui,
        BenchmarkDrawer>;

    vcl::Context::setResetFlags(BGFX_RESET_NONE);

    ImguiMeshViewer tw("ImGui Mesh Viewer GLFW");

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::TriMesh> drawable = getDrawableMesh<vcl::TriMesh>();

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(drawable);

    tw.setRepeatTimes(BENCHMARK_DRAWER_REPEAT_FOREVER);
    tw.fitScene();

    tw.show();

    return 0;
}
