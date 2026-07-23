// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <get_drawable_mesh.h>

#include <vclib/render/mesh_viewer.h>

int main(int argc, char** argv)
{
    vcl::Application app(argc, argv);

    vcl::TriEdgeMesh mesh =
        vcl::loadMesh<vcl::TriEdgeMesh>(
            VCLIB_EXAMPLE_MESHES_PATH
            "/gltf/CesiumMilkTruck/CesiumMilkTruck.gltf");

    auto dm = vcl::makeDrawable(std::move(mesh));

    std::cerr << "Headless: " << vcl::Context::instance().isHeadless() << "\n";

    vcl::MeshViewer viewer;

    vcl::pushDefaultEditors(viewer);

    showOnMeshViewer(argc, argv, viewer, std::move(dm));

    return app.exec();
}
