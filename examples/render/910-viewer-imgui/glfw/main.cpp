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

// imgui drawer must be included before the window manager...
#include <vclib/imgui/imgui_drawer.h>

#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/render_app.h>
#include <vclib/bgfx/drawable/lines/drawable_polylines.h>
#include <imgui.h>

template<typename DerivedRenderApp>
class DemoImGuiDrawer : public vcl::imgui::ImGuiDrawer<DerivedRenderApp>
{
    using ParentDrawer = vcl::imgui::ImGuiDrawer<DerivedRenderApp>;

public:
    using ParentDrawer::ParentDrawer;

    virtual void onDraw(vcl::uint viewId) override
    {
        // draw the scene
        ParentDrawer::onDraw(viewId);

        if (!ParentDrawer::isWindowMinimized()) {
            // imgui demo window
            ImGui::ShowDemoWindow();
        }
    }
};

int main(int argc, char** argv)
{
    using ImGuiDemo = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        DemoImGuiDrawer,
        vcl::ViewerDrawer>;

    ImGuiDemo tw("Viewer ImGui GLFW");

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::TriMesh> drawable = getDrawableMesh<vcl::TriMesh>();
    
    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(drawable);

    std::vector<vcl::lines::LinesVertex> points = {
        vcl::lines::LinesVertex(0.0, 0.0f, -2.0f, vcl::lines::LinesVertex::COLOR(0.0, 1.0, 0.0, 1.0)),
        vcl::lines::LinesVertex(0.5, 0.5f, 0.0f, vcl::lines::LinesVertex::COLOR(1.0, 1.0, 0.0, 1.0)),
        vcl::lines::LinesVertex(1.0f, -1.0f, 0.0f, vcl::lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
        vcl::lines::LinesVertex(1.0f, 1.0f, 0.25f, vcl::lines::LinesVertex::COLOR(0.0, 1.0, 0.0, 1.0)),
        vcl::lines::LinesVertex(1.5f, -1.0f, 0.0f, vcl::lines::LinesVertex::COLOR(1.0, 0.0, 0.5, 1.0)),
        vcl::lines::LinesVertex(1.5f, 0.5f, 0.5f, vcl::lines::LinesVertex::COLOR(0.0, 1.0, 1.0, 1.0)),
    }; 

    // auto line = vcl::lines::DrawableLines::create(points, vcl::lines::LinesTypes::TEXTURE_BASED);
    // line->getSettings().setColorToUse(vcl::lines::ColorToUse::PER_VERTEX_COLOR); 
    // tw.pushDrawableObject(*line.get());   

    // auto polyline = vcl::lines::DrawablePolylines::create(points, vcl::lines::LinesTypes::TEXTURE_BASED);
    // polyline->getSettings().setColorToUse(vcl::lines::ColorToUse::PER_VERTEX_COLOR); 
    // tw.pushDrawableObject(*polyline.get());    

    tw.fitScene();

    tw.show();

    return 0;
}
