/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

/*!
 * \file main.cpp
 * \brief VCLib Standalone Template Project
 *
 * This standalone template project demonstrates how to create basic mesh structures
 * using VCLib when included via FetchContent.
 *
 * It creates:
 * - An icosahedron mesh
 * - A simple point cloud
 * - A polygonal mesh
 *
 * Build instructions:
 * \code
 *   mkdir build && cd build
 *   cmake .. -DCMAKE_BUILD_TYPE=Release
 *   cmake --build . --config Release
 * \endcode
 */

#include <iostream>
#include <vclib/meshes.h>
#include <vclib/algorithms.h>
#include <vclib/version.h>

int main()
{
    std::cout << "=== VCLib Standalone Example 001: Basic Mesh Creation ===" << std::endl;
    std::cout << "VCLib Version: " << VCLIB_VERSION_STRING << std::endl;
    std::cout << "  Major: " << VCLIB_VERSION_MAJOR << std::endl;
    std::cout << "  Minor: " << VCLIB_VERSION_MINOR << std::endl;
    std::cout << "  Patch: " << VCLIB_VERSION_PATCH << std::endl;
    std::cout << std::endl;

    // Create an icosahedron
    std::cout << "--- Creating Icosahedron ---" << std::endl;
    vcl::TriMesh icosahedron = vcl::createIcosahedron<vcl::TriMesh>();

    std::cout << "  Vertices: " << icosahedron.vertexCount() << std::endl;
    std::cout << "  Faces: " << icosahedron.faceCount() << std::endl;

    // Verify icosahedron properties (V = 12, E = 30, F = 20)
    if (icosahedron.vertexCount() == 12 && icosahedron.faceCount() == 20) {
        std::cout << "  ✓ Icosahedron created successfully!" << std::endl;
    } else {
        std::cout << "  ✗ Unexpected icosahedron properties!" << std::endl;
        return 1;
    }
    std::cout << std::endl;

    // Create a simple point cloud mesh
    std::cout << "--- Creating Point Cloud ---" << std::endl;
    vcl::PointCloud pointCloud;
    pointCloud.addVertex({0.0, 0.0, 0.0});
    pointCloud.addVertex({1.0, 0.0, 0.0});
    pointCloud.addVertex({0.0, 1.0, 0.0});
    pointCloud.addVertex({0.0, 0.0, 1.0});

    std::cout << "  Points: " << pointCloud.vertexCount() << std::endl;
    std::cout << "  ✓ Point cloud created!" << std::endl;
    std::cout << std::endl;

    // Create a poly mesh (polygonal mesh)
    std::cout << "--- Creating PolyMesh ---" << std::endl;
    vcl::PolyMesh polyMesh;

    auto v0 = polyMesh.addVertex({0.0, 0.0, 0.0});
    auto v1 = polyMesh.addVertex({1.0, 0.0, 0.0});
    auto v2 = polyMesh.addVertex({1.0, 1.0, 0.0});
    auto v3 = polyMesh.addVertex({0.0, 1.0, 0.0});

    // Add a quad face
    std::vector<vcl::uint> faceVertices = {v0, v1, v2, v3};
    polyMesh.addFace(faceVertices);

    std::cout << "  Vertices: " << polyMesh.vertexCount() << std::endl;
    std::cout << "  Faces: " << polyMesh.faceCount() << std::endl;
    std::cout << "  Face has " << polyMesh.faces().front().vertexCount()
              << " vertices" << std::endl;
    std::cout << "  ✓ PolyMesh created!" << std::endl;
    std::cout << std::endl;

    std::cout << "=== Example completed successfully! ===" << std::endl;

    return 0;
}
