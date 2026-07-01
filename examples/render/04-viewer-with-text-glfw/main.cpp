// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "get_drawable_mesh.h"

#include <vclib/bgfx/drawers/text_drawer.h>
#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/render_app.h>

int main(int argc, char** argv)
{
    using ViewerWindow = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        vcl::ViewerDrawer,
        vcl::TextDrawer>;

    ViewerWindow tw("Viewer GLFW");

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::PolyMesh> m =
        getDrawableMesh<vcl::PolyMesh>("greek_helmet.obj");

    auto mrs = m.renderSettings();
    mrs.setSurface(vcl::MeshRenderInfo::Surface::SHADING_FLAT);
    m.setRenderSettings(mrs);

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(m);

    tw.enableText();

    tw.setTextFont(vcl::VclFont::DROID_SANS, 20);
    tw.appendStaticText({5, 5}, "Vertices: " + std::to_string(m.vertexCount()));
    tw.appendStaticText({5, 30}, "Faces: " + std::to_string(m.faceCount()));

    tw.fitScene();

    tw.show();

    return 0;
}
