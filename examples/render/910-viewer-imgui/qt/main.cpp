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

#include <vclib/qt_imgui/viewer_widget_imgui.h>

#include <imgui.h>

#include <QApplication>

class ImguiDemo : public vcl::qt::ViewerWidgetImgui
{
public:
    ImguiDemo(const std::string& windowTitle) :
            vcl::qt::ViewerWidgetImgui(windowTitle)
    {
    }

    void draw() override
    {
        // imgui demo window
        ImGui::ShowDemoWindow();

        // draw the scene
        ViewerWidgetImgui::draw();
    }
};

int main(int argc, char** argv)
{
    // FIXME #2 - Crash when closing window on Ubuntu

    QApplication app(argc, argv);

    ImguiDemo tw("Viewer ImGui Qt");

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::TriMesh> drawable = getDrawableMesh<vcl::TriMesh>();

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(drawable);

    tw.fitScene();

    tw.show();

    return app.exec();
}
