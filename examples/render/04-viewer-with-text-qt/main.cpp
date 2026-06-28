// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "get_drawable_mesh.h"

#include <vclib/qt/viewer_widget.h>

#include <vclib/bgfx/drawable/drawable_box3.h>
#include <vclib/bgfx/drawers/text_drawer.h>
#include <vclib/qt/widget_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/trackball_viewer_drawer.h>
#include <vclib/render/render_app.h>

int main(int argc, char** argv)
{
    using ViewerWidget = vcl::RenderApp<
        vcl::qt::WidgetManager,
        vcl::Canvas,
        vcl::TrackBallViewerDrawer,
        vcl::TextDrawer>;

    auto app = vcl::qt::qAppl(argc, argv);

    ViewerWidget tw("Viewer Qt");

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::TriMesh> m =
        getDrawableMesh<vcl::TriMesh>("greek_helmet.obj");

    vcl::updateBoundingBox(m);

    vcl::DrawableBox3 db(m.boundingBox(), vcl::Color::Red, 3.0f);
    db.name() = "Bounding Box";

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(m);
    tw.pushDrawableObject(db);

    tw.enableText();

    tw.setTextFont(vcl::VclFont::DROID_SANS, 20);
    tw.appendStaticText({5, 5}, "Vertices: " + std::to_string(m.vertexCount()));
    tw.appendStaticText({5, 30}, "Faces: " + std::to_string(m.faceCount()));

    tw.fitScene();

    tw.show();

    return app.exec();
}
