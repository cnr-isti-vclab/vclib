// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MESH_CONVEX_HULL_H
#define MESH_CONVEX_HULL_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

// This example demonstrates how to compute the convex hull of a point set
// using the incremental convex hull algorithm provided by VCLib.

auto meshConvexHull()
{
    std::cout << "=== VCLib Example 017: Convex Hull ===\n\n";

    // Load an input mesh
    vcl::TriMesh inputMesh =
        vcl::loadMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
    inputMesh.name() = "Input Mesh";

    std::cout << "Input mesh loaded: " << inputMesh.vertexCount()
              << " vertices, " << inputMesh.faceCount() << " faces\n";

    // Create logger for timing information
    vcl::ConsoleLogger log;
    log.enablePrintTimer();

    // Compute convex hull from vertex positions
    std::cout << "\nComputing convex hull...\n";
    vcl::TriMesh hullMesh = vcl::convexHull<vcl::TriMesh>(
        inputMesh.vertices() | vcl::views::positions, log);
    hullMesh.name() = "Convex Hull";

    std::cout << "Convex hull computed: " << hullMesh.vertexCount()
              << " vertices, " << hullMesh.faceCount() << " faces\n";

    // Also demonstrate computing hull from a simple point set
    std::cout << "\nComputing convex hull from point set...\n";

    std::vector<vcl::Point3d> points = {
        {0,   0,   0  },
        {1,   0,   0  },
        {0,   1,   0  },
        {0,   0,   1  },
        {1,   1,   0  },
        {1,   0,   1  },
        {0,   1,   1  },
        {1,   1,   1  },
        {0.5, 0.5, 0.5}  // internal point
    };

    vcl::TriMesh cubeHull = vcl::convexHull<vcl::TriMesh>(points, log);
    cubeHull.name()       = "Cube Convex Hull";

    std::cout << "Cube convex hull: " << cubeHull.vertexCount() << " vertices, "
              << cubeHull.faceCount() << " faces\n";

    return std::make_tuple(inputMesh, hullMesh, cubeHull);
}

#endif // MESH_CONVEX_HULL_H
