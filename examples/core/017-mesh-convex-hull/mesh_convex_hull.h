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
        vcl::load<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
    inputMesh.name() = "Input Mesh";

    std::cout << "Input mesh loaded: " << inputMesh.vertexNumber() 
              << " vertices, " << inputMesh.faceNumber() << " faces\n";

    // Create logger for timing information
    vcl::ConsoleLogger log;
    log.enablePrintTimer();

    // Compute convex hull from vertex positions
    std::cout << "\nComputing convex hull...\n";
    vcl::TriMesh hullMesh = vcl::convexHull<vcl::TriMesh>(
        inputMesh.vertices() | vcl::views::positions, log);
    hullMesh.name() = "Convex Hull";

    std::cout << "Convex hull computed: " << hullMesh.vertexNumber() 
              << " vertices, " << hullMesh.faceNumber() << " faces\n";

    // Also demonstrate computing hull from a simple point set
    std::cout << "\nComputing convex hull from point set...\n";
    
    std::vector<vcl::Point3d> points = {
        {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1},
        {1, 1, 0}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1},
        {0.5, 0.5, 0.5}  // internal point
    };

    vcl::TriMesh cubeHull = vcl::convexHull<vcl::TriMesh>(points, log);
    cubeHull.name() = "Cube Convex Hull";

    std::cout << "Cube convex hull: " << cubeHull.vertexNumber() 
              << " vertices, " << cubeHull.faceNumber() << " faces\n";

    return std::make_tuple(inputMesh, hullMesh, cubeHull);
}

#endif // MESH_CONVEX_HULL_H
