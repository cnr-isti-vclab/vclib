// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <iostream>

#include <vclib/glfw/viewer_window.h>

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/render/drawable/drawable_mesh.h>

int main(int argc, char** argv)
{
    std::cout << "=== VCLib Standalone Render Example: Mesh Viewer ==="
              << std::endl;

    // Create the GLFW viewer window
    vcl::glfw::ViewerWindow window("VCLib Standalone Mesh Viewer");

    // Load or generate a mesh
    // We create a standard icosahedron
    std::cout << "Generating icosahedron mesh..." << std::endl;
    vcl::TriMesh mesh = vcl::createIcosahedron<vcl::TriMesh>();

    // It's usually a good idea to compute per-vertex and per-face normals
    // so the renderer can properly shade the mesh.
    vcl::updatePerVertexAndFaceNormals(mesh);

    // Convert the mesh into a DrawableMesh which the viewer can render
    auto drawableMesh = vcl::makeDrawable(std::move(mesh));

    // Optionally set some color or drawing properties
    drawableMesh.color() = vcl::Color::LightGray;

    // Add the drawable object to the scene
    window.pushDrawableObject(std::move(drawableMesh));

    // Automatically fit the camera to show the whole scene
    window.fitScene();

    // Show the window and start the render loop
    std::cout << "Starting viewer..." << std::endl;
    window.show();

    return 0;
}
