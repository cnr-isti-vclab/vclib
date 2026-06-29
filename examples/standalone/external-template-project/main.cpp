// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

/*!
 * \file main.cpp
 * \brief VCLib Standalone External Template Project
 *
 * This standalone template project demonstrates how to use the external module
 * of VCLib when included via FetchContent.
 *
 * It creates a simple polygonal cube mesh and uses Embree to find the
 * intersection of a ray with the mesh.
 *
 * Build instructions:
 * \code
 *   mkdir build && cd build
 *   cmake .. -DCMAKE_BUILD_TYPE=Release
 *   cmake --build . --config Release
 * \endcode
 */

#include <iostream>
#include <vclib/embree/scene.h>
#include <vclib/meshes.h>
#include <vclib/version.h>

int main()
{
    std::cout << "=== VCLib Standalone External Example ===" << std::endl;
    std::cout << "VCLib Version: " << VCLIB_VERSION_STRING << std::endl;
    std::cout << std::endl;

    // Create a simple cube as a poly mesh
    vcl::PolyMesh m;

    auto v0 = m.addVertex({-1, -1, -1});
    auto v1 = m.addVertex({1, -1, -1});
    auto v2 = m.addVertex({1, 1, -1});
    auto v3 = m.addVertex({-1, 1, -1});
    auto v4 = m.addVertex({-1, -1, 1});
    auto v5 = m.addVertex({1, -1, 1});
    auto v6 = m.addVertex({1, 1, 1});
    auto v7 = m.addVertex({-1, 1, 1});

    m.addFace(v0, v1, v2, v3); // back
    m.addFace(v4, v5, v6, v7); // front
    m.addFace(v0, v1, v5, v4); // bottom
    m.addFace(v3, v2, v6, v7); // top
    m.addFace(v0, v3, v7, v4); // left
    m.addFace(v1, v2, v6, v5); // right

    std::cout << "--- Cube created ---" << std::endl;
    std::cout << "  Vertices: " << m.vertexCount() << std::endl;
    std::cout << "  Faces: " << m.faceCount() << std::endl;
    std::cout << std::endl;

    // Use Embree to compute ray intersection
    std::cout << "--- Initializing Embree Scene ---" << std::endl;
    vcl::embree::Scene scene(m);

    // Cast a ray from outside the cube pointing to the center
    vcl::Point3d origin(0, 0, 5);
    vcl::Point3d direction(0, 0, -1);

    std::cout << "Casting ray from " << origin << " along " << direction
              << "..." << std::endl;

    auto [faceId, barCoords, tId, dist] =
        scene.firstFaceIntersectedByRay(vcl::Ray(origin, direction));

    if (faceId != vcl::UINT_NULL) {
        std::cout << "  ✓ Hit successful!" << std::endl;
        std::cout << "  Face ID: " << faceId << std::endl;
        std::cout << "  Distance: " << dist << std::endl;
    }
    else {
        std::cout << "  ✗ Missed the cube!" << std::endl;
        return 1;
    }

    std::cout << std::endl;
    std::cout << "=== Example completed successfully! ===" << std::endl;

    return 0;
}
