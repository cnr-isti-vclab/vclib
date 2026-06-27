// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/imgui/imgui_drawer.h>
#include <vclib/imgui/imgui_stats_drawer.h>
#include <vclib/qt/viewer_widget.h>

#include <QFileDialog>

template<typename Der>
class ViewerDrawer : public vcl::TrackBallViewerDrawer<Der>
{
public:
    using Base = vcl::TrackBallViewerDrawer<Der>;
    using Base::Base;

    bool onMousePress(
        vcl::MouseButton::Enum   button,
        double                   x,
        double                   y,
        const vcl::KeyModifiers& modifiers) override
    {
        bool block = Base::onMousePress(button, x, y, modifiers);

        if (!block && button == vcl::MouseButton::RIGHT) {
            QFileDialog::getOpenFileName(
                nullptr, QObject::tr("Open Document"), QDir::currentPath());
        }
        return block;
    }
};

int main(int argc, char** argv)
{
    auto app = vcl::qt::qAppl(argc, argv);

    // vcl::Context::setResetFlags(BGFX_RESET_NONE);

    using Viewer = vcl::RenderApp<
        vcl::qt::WidgetManager,
        vcl::Canvas,
        vcl::imgui::ImGuiDrawer,
        vcl::imgui::ImguiStatsDrawer,
        ViewerDrawer>;

    Viewer viewer("Viewer with ImGui and Stats");

    viewer.show();

    return app.exec();
}
