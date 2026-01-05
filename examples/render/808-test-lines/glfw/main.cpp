/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

// imgui drawer must be included before the window manager...
#include <vclib/imgui/imgui_drawer.h>

#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/trackball_viewer_drawer.h>
#include <vclib/render/render_app.h>

#include <imgui.h>

#include "../lines_common.h"

template<typename DerivedRenderApp>
class LinesDrawer : public vcl::TrackBallViewerDrawer<DerivedRenderApp>
{
    using ParentDrawer = vcl::TrackBallViewerDrawer<DerivedRenderApp>;
    // lines
    const vcl::uint                     N_LINES = 4;
    std::shared_ptr<vcl::DrawableLines> mLines;
    int                                 mSelected = 0;

    std::shared_ptr<vcl::DrawableObjectVector> mVec =
        std::make_shared<vcl::DrawableObjectVector>();

    bool mIndexed = false;

public:
    using ParentDrawer::ParentDrawer;

    LinesDrawer(vcl::uint w, vcl::uint h) : ParentDrawer(w, h)
    {
        // initialize the drawable object vector with CPU generated lines
        ParentDrawer::setDrawableObjectVector(mVec);

        mVec->pushBack(std::move(getDrawableLines(N_LINES)));

        mLines = std::dynamic_pointer_cast<vcl::DrawableLines>(mVec->at(0));
    }

    virtual void onDraw(vcl::uint viewId) override
    {
        using enum vcl::Lines::ImplementationType;

        ParentDrawer::onDraw(viewId);

        // TODO: selectable lines
        int selected = mSelected;

        ImGui::Begin("Showing");
        const char* items[] = {"Primitive", "CPU Generated", "GPU Generated"};
        for (int i = 0; i < vcl::toUnderlying(COUNT); ++i) {
            ImGui::RadioButton(items[i], &selected, i);
        }
        ImGui::End();

        if (selected != mSelected) {
            mSelected = selected;
            mLines->setImplementationType(
                static_cast<vcl::Lines::ImplementationType>(mSelected));
        }

        ImGui::Begin("Settings");
        bool indexed = mIndexed;
        ImGui::Checkbox("Indexed", &indexed);

        if (indexed != mIndexed) {
            auto t   = mLines->thickness();
            auto c   = mLines->colorToUse();
            auto i   = mLines->implementationType();
            mIndexed = indexed;
            mVec->clear();
            mVec->pushBack(std::move(getDrawableLines(N_LINES, indexed)));
            mLines = std::dynamic_pointer_cast<vcl::DrawableLines>(mVec->at(0));
            mLines->thickness() = t;
            mLines->setColorToUse(c);
            mLines->setImplementationType(i);
        }

        ImGui::SliderFloat("Thickness", &mLines->thickness(), 1.0f, 100.0f);

        const char* colorToUseItems[] = {"Per Vertex", "Per Edge", "General"};
        int         colorToUse        = vcl::toUnderlying(mLines->colorToUse());
        for (int i = 0; i < 3; ++i) {
            ImGui::RadioButton(colorToUseItems[i], &colorToUse, i);
        }
        mLines->setColorToUse(static_cast<vcl::Lines::ColorToUse>(colorToUse));
        ImGui::End();
    }
};

int main(int argc, char** argv)
{
    using LinesDemo = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        vcl::imgui::ImGuiDrawer,
        LinesDrawer>;

    LinesDemo tw("Test Lines ImGui GLFW");

    tw.fitScene();

    tw.show();

    return 0;
}
