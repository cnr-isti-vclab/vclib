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

#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/algorithms/mesh/update/transform.h>
#include <vclib/qt/mesh_viewer.h>

#include <QApplication>

class MeshViewerSelectQt : public vcl::qt::MeshViewer
{
public:
    using vcl::qt::MeshViewer::MeshViewer;

    MeshViewerSelectQt(QWidget* parent = nullptr) : vcl::qt::MeshViewer(parent)
    {
        viewer().setOnObjectSelected([this](uint id) {
            drawableObjectVectorTree().setSelectedItem(id);
        });
    }
};

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    MeshViewerSelectQt mv;

    // load and set up a drawable mesh
    auto m = getDrawableMesh<vcl::TriMesh>();

    using enum vcl::MeshRenderInfo::Buffers;

    m.enablePerFaceColor();
    for (auto& f : m.faces()) {
        if (f.index() % 3 == 0)
            f.color() = vcl::Color::Red;
        else if (f.index() % 3 == 1)
            f.color() = vcl::Color::Green;
        else
            f.color() = vcl::Color::Blue;
    }
    m.updateBuffers({TRI_COLORS});

    auto v = std::make_shared<vcl::DrawableObjectVector>();
    v->pushBack(std::move(m));

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::TriMesh> drawable = getDrawableMesh<vcl::TriMesh>();

    drawable.name() = "bimba_scaled";

    // update the mesh to be displayed in the scene
    const auto bb = vcl::boundingBox(drawable);
    vcl::scale(drawable, 0.5f);
    vcl::translate(drawable, vcl::Point3d(bb.size().x(), 0, 0));

    drawable.updateBuffers({VERTICES, VERT_NORMALS});
    v->pushBack(std::move(drawable));

    mv.setDrawableObjectVector(v);

    mv.show();
    mv.showMaximized();

    return app.exec();
}
