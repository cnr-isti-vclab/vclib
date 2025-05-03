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
#include <vclib/render/drawers/trackball_viewer_drawer.h>
#include <vclib/render/render_app.h>

#include <imgui.h>

#include "../lines_common.h"

template<typename DerivedRenderApp>
class LinesImGuiDrawer : public vcl::imgui::ImGuiDrawer<DerivedRenderApp>
{
    using ParentDrawer = vcl::imgui::ImGuiDrawer<DerivedRenderApp>;
    std::shared_ptr<vcl::DrawableObjectVector> mLines;
    const vcl::uint N_LINES = 100;
    int mSelcted = 0;

public:
    using ParentDrawer::ParentDrawer;

    LinesImGuiDrawer(vcl::uint, vcl::uint) 
    {
        mLines = std::make_shared<vcl::DrawableObjectVector>(getDrawableLines(N_LINES));
    }

    virtual void onDraw(vcl::uint viewId) override
    {
        // draw the scene
        ParentDrawer::onDraw(viewId);

        if (!ParentDrawer::isWindowMinimized()) {
            ImGui::Begin("Showing");

            const char* items[] = { "CPU Generated" , "GPU Generated", "Instancing Based", "Indirect Based", "Texture Based"};

            for (int i = 0; i < mLines->size(); i++)
                ImGui::RadioButton(items[i], &mSelcted, i);
            ImGui::End();
        }

        mLines->at(mSelcted)->draw(viewId);
    }
};

int main(int argc, char** argv)
{
    using ImGuiDemo = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        LinesImGuiDrawer,
        vcl::TrackBallViewerDrawer>;

    ImGuiDemo tw("Test Lines ImGui GLFW");

    tw.fitScene();

    tw.show();

    return 0;
}
