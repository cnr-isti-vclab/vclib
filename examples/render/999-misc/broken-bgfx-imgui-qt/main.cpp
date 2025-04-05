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

#include <vclib/imgui/imgui_drawer.h>
#include <vclib/imgui/imgui_stats_drawer.h>
#include <vclib/qt/viewer_widget.h>

#include <QApplication>
#include <QFileDialog>

template<typename Der>
class ViewerDrawer : public vcl::TrackBallViewerDrawer<Der>
{
public:
    using ParentViewer = vcl::TrackBallViewerDrawer<Der>;
    using ParentViewer::ParentViewer;

    void onMousePress(
        vcl::MouseButton::Enum   button,
        double                   x,
        double                   y,
        const vcl::KeyModifiers& modifiers) override
    {
        vcl::TrackBallViewerDrawer<Der>::onMousePress(button, x, y, modifiers);

        if (button == vcl::MouseButton::RIGHT) {
            QFileDialog::getOpenFileName(
                nullptr, QObject::tr("Open Document"), QDir::currentPath());
        }
    }
};

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

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
