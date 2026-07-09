// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

// imgui drawer must be included before the window manager...
#include <vclib/imgui/imgui_drawer.h>

#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/render_app.h>

#include <imgui/imgui.h>

#include "../808-test-lines-common/lines_common.h"

template<typename DerivedRenderApp>
class LinesDrawer : public vcl::ViewerDrawer<DerivedRenderApp>
{
    using ParentDrawer = vcl::ViewerDrawer<DerivedRenderApp>;
    // lines
    const vcl::uint                     N_LINES = 8;
    std::shared_ptr<vcl::DrawableLines> mLines;
    int                                 mSelected = 0;

    std::shared_ptr<vcl::DrawableObjectVector> mVec =
        std::make_shared<vcl::DrawableObjectVector>();

    bool mIndexed = false;

public:
    using ParentDrawer::ParentDrawer;

    LinesDrawer() = default;

    LinesDrawer(vcl::uint w, vcl::uint h) : ParentDrawer(w, h)
    {
        // initialize the drawable object vector with CPU generated lines
        ParentDrawer::setDrawableObjectVector(mVec);

        mVec->pushBack(std::move(getDrawableLines(N_LINES)));

        mLines = std::dynamic_pointer_cast<vcl::DrawableLines>(mVec->at(0));
    }

    virtual void onDraw(uint viewId) override
    {
        using enum vcl::Lines::ImplementationType;

        ParentDrawer::onDraw(viewId);

        // TODO: selectable lines
        int selected = mSelected;

        ImGui::Begin("Showing");
        const char* items[] = {"Primitive", "CPU Generated"};
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

        bool shadingPerVertex = mLines->shadingPerVertex();
        ImGui::Checkbox("Shading per Vertex", &shadingPerVertex);
        mLines->setShading(shadingPerVertex);

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
