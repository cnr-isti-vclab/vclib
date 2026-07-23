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

    void* displayId = vcl::getDisplayId();

    // Initialize the VCL context with a null window and the display ID
    // (will be nullptr if not running on Linux with X11 or Wayland)
    auto& inst = vcl::Context::instance(nullptr, displayId);

    std::cerr << "Headless: " << inst.isHeadless() << "\n";

    assert(inst.isHeadless() == true);

    vcl::MeshViewer viewer;

    vcl::pushDefaultEditors(viewer);

    std::vector<vcl::TriEdgeMesh> meshes =
        vcl::loadMeshes<vcl::TriEdgeMesh>(
            VCLIB_EXAMPLE_MESHES_PATH
            "/gltf/CesiumMilkTruck/CesiumMilkTruck.gltf");
    showOnMeshViewer(argc, argv, viewer, std::move(meshes));

    return app.exec();
}
