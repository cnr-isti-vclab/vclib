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

#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/algorithms/mesh/update/transform.h>
#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/render_app.h>
#include <vclib/imgui/imgui_stats_drawer.h>

#include <imgui.h>

#include "../get_program_switcher_drawable_mesh.h"
#include "../imgui_switch_program_drawer.h"
#include "../program_switcher_drawable_mesh.h"

int main(int argc, char** argv)
{
    using ImguiMeshViewer = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        vcl::imgui::ImGuiDrawer,
        vcl::imgui::MeshViewerDrawerImgui,
        vcl::imgui::ImguiStatsDrawer,
        ImguiSwitchProgramDrawer>;

    bool b = false;
    ImguiSwitchProgramDrawer<ImguiMeshViewer>::useSwitchProgram = &b;
    ProgramSwitcherDrawableMesh<vcl::TriMesh>::useSwitchProgram = &b;

    ImguiMeshViewer tw("ImGui Mesh Viewer GLFW");

    bgfx::reset(tw.width()*tw.dpiScale().x(), tw.height()*tw.dpiScale().y(), BGFX_RESET_NONE);

    // load and set up a drawable mesh
    ProgramSwitcherDrawableMesh<vcl::TriMesh> drawable = 
        getProgramSwitcherDrawableMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj");

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(drawable);

    // update the mesh to be displayed in the scene
    const auto bb = vcl::boundingBox(drawable);
    vcl::scale(drawable, 0.5f);
    vcl::translate(drawable, vcl::Point3d(bb.size().x(), 0, 0));

    tw.pushDrawableObject(drawable);

    tw.fitScene();

    tw.show();

    return 0;
}
