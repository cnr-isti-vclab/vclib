// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "get_drawable_mesh.h"

#include <vclib/qt/viewer_window.h>

int main(int argc, char** argv)
{
    auto app = vcl::qt::qAppl(argc, argv);

    vcl::qt::ViewerWindow tw("Viewer Qt");

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::TriMesh> drawable = getDrawableMesh<vcl::TriMesh>();

    drawable.color() = vcl::Color::Yellow;
    drawable.updateBuffers(
        {vcl::MeshRenderInfo::Buffers::MESH_ADDITIONAL_DATA});

    auto mrs = drawable.renderSettings();
    mrs.setSurface(vcl::MeshRenderInfo::Surface::COLOR_MESH);
    mrs.setSurface(vcl::MeshRenderInfo::Surface::SHADING_FLAT);
    drawable.setRenderSettings(mrs);

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(drawable);

    tw.fitScene();

    tw.show();

    return app.exec();
}
