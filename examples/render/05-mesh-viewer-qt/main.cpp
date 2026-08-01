// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "get_drawable_mesh.h"

#include <vclib/render/mesh_viewer.h>

int main(int argc, char** argv)
{
    auto app = vcl::qt::qAppl(argc, argv);

    vcl::qt::MeshViewer mv;

    vcl::pushDefaultEditors(mv);

    // load and set up a drawable mesh
    auto mesh = getDrawableMesh<vcl::TriMesh>();

    auto mesh_scaled = mesh;

    using enum vcl::MeshRenderInfo::Buffers;

    // to test the per-vertex and per-face color rendering, we set both of them
    mesh.enablePerVertexColor();
    mesh.enablePerFaceColor();

    for (auto& f : mesh.faces()) {
        f.vertex(0)->color() = vcl::Color::Red;
        f.vertex(1)->color() = vcl::Color::Green;
        f.vertex(2)->color() = vcl::Color::Blue;
        if (f.index() % 3 == 0)
            f.color() = vcl::Color::Red;
        else if (f.index() % 3 == 1)
            f.color() = vcl::Color::Green;
        else
            f.color() = vcl::Color::Blue;
    }
    mesh.updateBuffers({VERT_COLORS, TRI_COLORS, WIREFRAME});

    mv.pushDrawableObject(std::move(mesh));

    // load and set up a drawable mesh
    mesh_scaled.name() = "bimba_scaled";

    // update the mesh to be displayed in the scene
    const auto bb = vcl::boundingBox(mesh_scaled);

    vcl::Matrix44d rot         = vcl::Matrix44d::Identity();
    vcl::Matrix44d scale       = vcl::Matrix44d::Identity();
    vcl::Matrix44d translation = vcl::Matrix44d::Identity();

    vcl::setTransformMatrixRotation(
        rot, vcl::Point3d(0, 1, 0), vcl::toRad(90.0));

    vcl::setTransformMatrixScale(scale, 0.5);
    vcl::setTransformMatrixTranslation(
        translation, vcl::Point3d(bb.size().x(), 0, 0));

    mesh_scaled.transformMatrix() = translation * scale * rot;

    mesh_scaled.updateBuffers({MESH_ADDITIONAL_DATA});
    mv.pushDrawableObject(std::move(mesh_scaled));

    mv.fitScene();
    mv.show();
    mv.showMaximized();

    return app.exec();
}
