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

#ifndef MESH_NORMALS_H
#define MESH_NORMALS_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

// This example demonstrates how to:
// 1. Calculate face and vertex normals
// 2. Use different weighting algorithms for vertex normals
// 3. Manipulate normals with transformations

auto meshNormals()
{
    std::cout << "=== VCLib Example 005: Mesh Normals ===\n\n";

    // ========================================
    // PART 1: LOADING AND BASIC NORMAL CALCULATION
    // ========================================

    std::cout << "PART 1: Loading Mesh and Basic Normal Calculation\n";
    std::cout << "--------------------------------------------------\n";

    // Load a sample mesh
    vcl::TriMesh mesh;
    vcl::load(mesh, VCLIB_EXAMPLE_MESHES_PATH "/bimba_simplified.obj");

    std::cout << "Loaded mesh: " << mesh.vertexNumber() << " vertices, "
              << mesh.faceNumber() << " faces\n\n";

    // ========================================
    // PART 2: FACE NORMALS
    // ========================================

    std::cout << "PART 2: Face Normals\n";
    std::cout << "--------------------\n";

    // Calculate face normals
    vcl::updatePerFaceNormals(mesh);

    std::cout << "Face normals calculated for all " << mesh.faceNumber()
              << " faces\n";

    // Show normals for first few faces
    std::cout << "\nFirst 3 face normals:\n";
    int faceCount = 0;
    for (const auto& face : mesh.faces()) {
        if (faceCount >= 3)
            break;
        std::cout << "  Face " << face.index() << ": (" << face.normal().x()
                  << ", " << face.normal().y() << ", " << face.normal().z()
                  << ")\n";
        faceCount++;
    }

    // ========================================
    // PART 3: VERTEX NORMALS - DIFFERENT ALGORITHMS
    // ========================================

    std::cout << "\n\nPART 3: Vertex Normals - Different Algorithms\n";
    std::cout << "----------------------------------------------\n";

    // 3.1 Basic vertex normals (area weighted)
    std::cout << "\n3.1 Basic Vertex Normals (area weighted):\n";
    vcl::updatePerVertexNormals(mesh);

    std::cout << "Basic vertex normals calculated\n";
    std::cout << "First vertex normal: (" << mesh.vertex(0).normal().x() << ", "
              << mesh.vertex(0).normal().y() << ", "
              << mesh.vertex(0).normal().z() << ")\n";

    // Store this result for comparison
    auto basicNormal = mesh.vertex(0).normal();

    // 3.2 Vertex normals from face normals (uniform weight)
    std::cout << "\n3.2 Vertex Normals from Face Normals (uniform weight):\n";
    vcl::updatePerVertexNormalsFromFaceNormals(mesh);

    auto uniformNormal = mesh.vertex(0).normal();
    std::cout << "Uniform weighted normal: (" << uniformNormal.x() << ", "
              << uniformNormal.y() << ", " << uniformNormal.z() << ")\n";

    // 3.3 Angle weighted vertex normals
    std::cout << "\n3.3 Angle Weighted Vertex Normals:\n";
    vcl::updatePerVertexNormalsAngleWeighted(mesh);

    auto angleNormal = mesh.vertex(0).normal();
    std::cout << "Angle weighted normal: (" << angleNormal.x() << ", "
              << angleNormal.y() << ", " << angleNormal.z() << ")\n";

    // 3.4 Nelson-Max weighted vertex normals
    std::cout << "\n3.4 Nelson-Max Weighted Vertex Normals:\n";
    vcl::updatePerVertexNormalsNelsonMaxWeighted(mesh);

    auto nelsonMaxNormal = mesh.vertex(0).normal();
    std::cout << "Nelson-Max weighted normal: (" << nelsonMaxNormal.x() << ", "
              << nelsonMaxNormal.y() << ", " << nelsonMaxNormal.z() << ")\n";

    // Compare the different methods
    std::cout << "\nComparison of different weighting methods for vertex 0:\n";
    std::cout << "  Basic (area):     " << basicNormal.norm()
              << " (magnitude)\n";
    std::cout << "  Uniform:          " << uniformNormal.norm()
              << " (magnitude)\n";
    std::cout << "  Angle weighted:   " << angleNormal.norm()
              << " (magnitude)\n";
    std::cout << "  Nelson-Max:       " << nelsonMaxNormal.norm()
              << " (magnitude)\n";

    // ========================================
    // PART 4: NORMAL MANIPULATION
    // ========================================

    std::cout << "\n\nPART 4: Normal Manipulation\n";
    std::cout << "----------------------------\n";

    // Create a transformation matrix to flip normals
    std::cout << "4.1 Flipping Normals:\n";
    vcl::Matrix44d flipMatrix = vcl::Matrix44d::Identity();
    flipMatrix.diagonal() << -1, -1, -1, 1; // Flip X, Y, Z components

    // Store original normal for comparison
    auto originalFaceNormal = mesh.face(0).normal();
    std::cout << "Original face 0 normal: (" << originalFaceNormal.x() << ", "
              << originalFaceNormal.y() << ", " << originalFaceNormal.z()
              << ")\n";

    // Apply transformation to face normals
    vcl::multiplyPerFaceNormalsByMatrix(mesh, flipMatrix);

    auto flippedFaceNormal = mesh.face(0).normal();
    std::cout << "Flipped face 0 normal:  (" << flippedFaceNormal.x() << ", "
              << flippedFaceNormal.y() << ", " << flippedFaceNormal.z()
              << ")\n";

    // Apply transformation to vertex normals
    auto originalVertexNormal = mesh.vertex(0).normal();
    std::cout << "\nOriginal vertex 0 normal: (" << originalVertexNormal.x()
              << ", " << originalVertexNormal.y() << ", "
              << originalVertexNormal.z() << ")\n";

    vcl::multiplyPerVertexNormalsByMatrix(mesh, flipMatrix);

    auto flippedVertexNormal = mesh.vertex(0).normal();
    std::cout << "Flipped vertex 0 normal:  (" << flippedVertexNormal.x()
              << ", " << flippedVertexNormal.y() << ", "
              << flippedVertexNormal.z() << ")\n";

    // ========================================
    // PART 5: WORKING WITH DIFFERENT MESH TYPES
    // ========================================

    std::cout << "\n\nPART 5: Working with Different Mesh Types\n";
    std::cout << "------------------------------------------\n";

    // Load and process a polygon mesh
    vcl::PolyMesh polyMesh;
    vcl::load(polyMesh, VCLIB_EXAMPLE_MESHES_PATH "/cube_poly.ply");

    std::cout << "Loaded polygon mesh: " << polyMesh.vertexNumber()
              << " vertices, " << polyMesh.faceNumber() << " faces\n";

    // Calculate normals for polygon mesh
    vcl::updatePerFaceNormals(polyMesh);
    vcl::updatePerVertexNormals(polyMesh);

    std::cout << "Calculated normals for polygon mesh\n";
    std::cout << "First polygon face normal: (" << polyMesh.face(0).normal().x()
              << ", " << polyMesh.face(0).normal().y() << ", "
              << polyMesh.face(0).normal().z() << ")\n";

    // ========================================
    // SUMMARY
    // ========================================

    std::cout << "\n\n=== EXAMPLE SUMMARY ===\n";
    std::cout << "\nIn this example we learned:\n\n";

    std::cout << "1. FACE NORMALS:\n";
    std::cout
        << "   - updatePerFaceNormals(): calculates normals for all faces\n";
    std::cout << "   - Face normals are perpendicular to the face surface\n\n";

    std::cout << "2. VERTEX NORMALS - DIFFERENT WEIGHTING:\n";
    std::cout << "   - updatePerVertexNormals(): area-weighted average\n";
    std::cout
        << "   - updatePerVertexNormalsFromFaceNormals(): uniform weight\n";
    std::cout << "   - updatePerVertexNormalsAngleWeighted(): angle-weighted\n";
    std::cout << "   - updatePerVertexNormalsNelsonMaxWeighted(): Nelson-Max "
                 "method\n\n";

    std::cout << "3. NORMAL MANIPULATION:\n";
    std::cout
        << "   - multiplyPerFaceNormalsByMatrix(): transform face normals\n";
    std::cout << "   - multiplyPerVertexNormalsByMatrix(): transform vertex "
                 "normals\n";
    std::cout << "   - Useful for flipping, rotating, or scaling normals\n\n";

    std::cout << "4. MESH TYPE COMPATIBILITY:\n";
    std::cout
        << "   - All normal algorithms work with both TriMesh and PolyMesh\n";
    std::cout << "   - Polygon faces are handled automatically\n\n";

    std::cout << "Normal calculation is essential for:\n";
    std::cout << "- Proper lighting and shading in rendering\n";
    std::cout << "- Surface analysis and geometry processing\n";
    std::cout << "- Smooth surface reconstruction\n";
    std::cout << "- Geometric computations requiring surface orientation\n\n";

    std::cout << "Example completed successfully!\n";

    // Restore proper normals before returning
    vcl::updatePerFaceNormals(mesh);
    vcl::updatePerVertexNormals(mesh);

    return std::make_tuple(mesh, polyMesh);
}

#endif // MESH_NORMALS_H
