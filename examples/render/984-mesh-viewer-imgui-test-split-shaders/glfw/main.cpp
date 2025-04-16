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

#include "get_drawable_mesh.h"

#include <vclib/imgui/mesh_viewer_imgui_drawer.h>

#include <vclib/imgui/imgui_drawer.h>

#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/algorithms/mesh/update/transform.h>
#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/render_app.h>
#include <vclib/imgui/imgui_stats_drawer.h>

#include <imgui.h>

#include "../imgui_split_program_drawer.h"

int main(int argc, char** argv)
{
    using namespace vcl;

    using ImguiMeshViewer = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        vcl::imgui::ImGuiDrawer,
        vcl::imgui::MeshViewerDrawerImgui,
        vcl::imgui::ImguiStatsDrawer,
        ImguiSplitProgramDrawer>;

    vcl::Context::setResetFlags(BGFX_RESET_NONE);

    ImguiMeshViewer tw("ImGui Mesh Viewer GLFW");

    // get control of the DrawableObjectVector of the viewer
    std::shared_ptr<DrawableObjectVector> vec =
        std::make_shared<DrawableObjectVector>();
    tw.setDrawableObjectVector(vec);

    // load and set up a drawable mesh
    DrawableMesh<TriMesh> drawable = getDrawableMesh<TriMesh>("bimba.obj");

    // add the drawable mesh to the scene
    tw.pushDrawableObject(std::move(drawable));

    auto setShaderFunction =
        [&vec](DrawableMesh<TriMesh>::SurfaceProgramsType type) {
            for (auto& obj : *vec) {
                auto* mesh = dynamic_cast<DrawableMesh<TriMesh>*>(obj.get());
                if (mesh) {
                    mesh->setSurfaceProgramType(type);
                }
            }
        };

    tw.setSurfaceProgramChangerFn(setShaderFunction);

    tw.fitScene();

    tw.show();

    return 0;
}
