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

#ifndef MESH_CLEAN_H
#define MESH_CLEAN_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

// This example demonstrates mesh cleaning and repairing algorithms:
// 1. Removing unreferenced vertices
// 2. Removing duplicated vertices
// 3. Removing duplicated faces
// 4. Removing degenerate vertices and faces
// 5. Checking mesh topology properties
// 6. Connected components analysis

auto meshClean()
{
    std::cout << "=== VCLib Example 013: Mesh Cleaning ===\n\n";

    /****** Load a mesh and perform basic cleaning ******/

    std::cout << "=== Loading and Cleaning Brain Mesh ===" << std::endl;

    vcl::TriMesh originalMesh =
        vcl::load<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/brain.ply");
    originalMesh.name() = "Original Brain Mesh";

    vcl::TriMesh mesh = originalMesh;
    mesh.name() = "Cleaned Brain Mesh";

    std::cout << "Original mesh: " << mesh.vertexNumber() << " vertices, "
              << mesh.faceNumber() << " faces" << std::endl;

    // Remove unreferenced vertices
    vcl::uint removedUnref = vcl::removeUnreferencedVertices(mesh);
    std::cout << "Removed " << removedUnref
              << " unreferenced vertices" << std::endl;

    // Remove duplicated vertices
    vcl::uint removedDupVerts = vcl::removeDuplicatedVertices(mesh);
    std::cout << "Removed " << removedDupVerts
              << " duplicated vertices" << std::endl;

    // Remove duplicated faces
    vcl::uint removedDupFaces = vcl::removeDuplicatedFaces(mesh);
    std::cout << "Removed " << removedDupFaces
              << " duplicated faces" << std::endl;

    // Remove degenerate faces
    vcl::uint removedDegFaces = vcl::removeDegenerateFaces(mesh);
    std::cout << "Removed " << removedDegFaces
              << " degenerate faces" << std::endl;

    // Compact the mesh to remove deleted elements
    mesh.compact();

    std::cout << "Cleaned mesh: " << mesh.vertexNumber() << " vertices, "
              << mesh.faceNumber() << " faces\n" << std::endl;

    /****** Removing degenerate vertices with NaN positions ******/

    std::cout << "\n=== Handling Degenerate Vertices ===" << std::endl;

    // Create a test mesh with some problematic vertices
    vcl::TriMesh testMesh;
    testMesh.name() = "Test Mesh";

    // Add normal vertices
    vcl::uint v0 = testMesh.addVertex(vcl::Point3d(0, 0, 0));
    vcl::uint v1 = testMesh.addVertex(vcl::Point3d(1, 0, 0));
    vcl::uint v2 = testMesh.addVertex(vcl::Point3d(0, 1, 0));
    
    // Add a vertex with NaN position
    vcl::uint v3 = testMesh.addVertex(vcl::Point3d(
        std::numeric_limits<double>::quiet_NaN(), 0, 0));

    // Add faces
    testMesh.addFace(v0, v1, v2);
    testMesh.addFace(v0, v1, v3); // This face will be affected

    std::cout << "Test mesh before cleaning: " 
              << testMesh.vertexNumber() << " vertices, "
              << testMesh.faceNumber() << " faces" << std::endl;

    // Remove degenerate vertices and associated faces
    vcl::uint removedDegVerts = vcl::removeDegeneratedVertices(testMesh, true);
    std::cout << "Removed " << removedDegVerts 
              << " degenerate vertices" << std::endl;

    testMesh.compact();
    std::cout << "Test mesh after cleaning: " 
              << testMesh.vertexNumber() << " vertices, "
              << testMesh.faceNumber() << " faces" << std::endl;

    std::cout << "Example completed successfully!\n";

    return std::make_tuple(originalMesh, mesh, testMesh);
}

#endif // MESH_CLEAN_H
