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

    // Note: This example tests mixing a purely headless context initialization
    // with a physical GUI window created afterwards.
    // While this works on Windows and Linux, it will result in a black screen
    // on macOS. When the primary context is initialized as headless 
    // (nwh=nullptr, 0x0 resolution), the BGFX Metal backend does not set up 
    // the necessary queues for screen presentation. Consequently, any secondary 
    // window (like the MeshViewer below) will fail to update its CAMetalLayer.
    // For pure offscreen rendering on macOS (no GUI), headless mode works fine.
    vcl::MeshViewer viewer;

    vcl::pushDefaultEditors(viewer);

    std::vector<vcl::TriEdgeMesh> meshes =
        vcl::loadMeshes<vcl::TriEdgeMesh>(
            VCLIB_EXAMPLE_MESHES_PATH
            "/gltf/CesiumMilkTruck/CesiumMilkTruck.gltf");
    showOnMeshViewer(argc, argv, viewer, std::move(meshes));

    return app.exec();
}
