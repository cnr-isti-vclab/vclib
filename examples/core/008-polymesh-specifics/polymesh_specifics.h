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

#ifndef POLYMESH_SPECIFICS_H
#define POLYMESH_SPECIFICS_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

// This example demonstrates how to:
// 1. Understand differences between TriMesh and PolyMesh
// 2. Work with polygonal faces of variable size
// 3. Use PolyMesh-specific operations and features

auto polymeshSpecifics()
{
    std::cout << "=== VCLib Example 008: PolyMesh Specifics ===\n\n";

    // ========================================
    // PART 1: CREATING A POLYMESH FROM SCRATCH
    // ========================================

    std::cout << "PART 1: Creating a PolyMesh from Scratch\n";
    std::cout << "----------------------------------------\n";

    vcl::PolyMesh polyMesh;
    polyMesh.name() = "Hexagon Pyramid PolyMesh";

    // Add vertices for a hexagon
    std::cout << "Creating a hexagonal face:\n";

    // Add 6 vertices in a circle
    for (int i = 0; i < 6; ++i) {
        double angle = i * 2.0 * M_PI / 6.0;
        double x     = std::cos(angle);
        double y     = std::sin(angle);
        polyMesh.addVertex(vcl::Point3d(x, y, 0.0));
    }

    std::cout << "Added " << polyMesh.vertexNumber() << " vertices\n";

    // Create a hexagonal face
    vcl::uint fid     = polyMesh.addFace();
    auto&     hexFace = polyMesh.face(fid);

    // Add vertices to the face one by one
    for (vcl::uint i = 0; i < 6; ++i) {
        hexFace.pushVertex(i);
        // or:
        // hexFace.pushVertex(&polyMesh.vertex(i));
    }

    std::cout << "Created hexagonal face with " << hexFace.vertexNumber()
              << " vertices\n";

    // Add a center vertex (shifted on z) and create triangular faces
    vcl::uint centerVertex = polyMesh.addVertex(vcl::Point3d(0.0, 0.0, -1.0));

    // Create triangular faces from center to each edge of hexagon
    for (vcl::uint i = 0; i < 6; ++i) {
        // addFace can take vertex references or indices
        polyMesh.addFace(centerVertex, (i + 1) % 6, i);
    }

    std::cout << "Added " << (polyMesh.faceNumber() - 1)
              << " triangular faces around the center\n";
    std::cout << "Total faces: " << polyMesh.faceNumber() << "\n\n";

    // importing a TriMesh from a PolyMesh will automatically triangulate the
    // polygonal faces

    vcl::TriMesh triMesh;
    triMesh.importFrom(polyMesh);
    triMesh.name() = "Hexagon Pyramid TriMesh";

    std::cout << "Converted to TriMesh: " << triMesh.vertexNumber()
              << " vertices, " << triMesh.faceNumber() << " faces\n\n";

    // ========================================
    // PART 2: POLYMESH VS TRIMESH COMPARISON
    // ========================================

    std::cout << "PART 2: PolyMesh vs TriMesh Comparison\n";
    std::cout << "--------------------------------------\n";

    // Load the same mesh as both PolyMesh and TriMesh
    vcl::PolyMesh polyMeshLoaded;
    vcl::TriMesh  triMeshLoaded;

    // note: the file contains a cube with 6 square faces
    vcl::load(polyMeshLoaded, VCLIB_EXAMPLE_MESHES_PATH "/cube_poly.ply");
    vcl::load(triMeshLoaded, VCLIB_EXAMPLE_MESHES_PATH "/cube_poly.ply");

    std::cout << "Loaded cube as PolyMesh: " << polyMeshLoaded.vertexNumber()
              << " vertices, " << polyMeshLoaded.faceNumber() << " faces\n";
    std::cout << "Loaded cube as TriMesh:  " << triMeshLoaded.vertexNumber()
              << " vertices, " << triMeshLoaded.faceNumber() << " faces\n";

    // Analyze face structure
    std::cout << "\nFace structure analysis:\n";

    // PolyMesh face analysis
    std::map<int, int> polyFaceSizes;
    for (const auto& face : polyMeshLoaded.faces()) {
        int size = face.vertexNumber();
        polyFaceSizes[size]++;
    }

    std::cout << "PolyMesh face distribution:\n";
    for (const auto& pair : polyFaceSizes) {
        std::cout << "  " << pair.second << " faces with " << pair.first
                  << " vertices\n";
    }

    // TriMesh (all faces are triangles)
    std::cout << "TriMesh face distribution:\n";
    std::cout << "  " << triMeshLoaded.faceNumber()
              << " faces with 3 vertices each\n\n";

    // ========================================
    // PART 3: DYNAMIC FACE MANIPULATION
    // ========================================

    std::cout << "PART 3: Dynamic Face Manipulation\n";
    std::cout << "----------------------------------\n";

    // Create a new PolyMesh for dynamic manipulation
    vcl::PolyMesh dynamicMesh;
    dynamicMesh.name() = "Dynamic Square PolyMesh";

    // Add vertices for a square
    for (int i = 0; i < 4; ++i) {
        dynamicMesh.addVertex(
            vcl::Point3d(i % 2 == 0 ? -1.0 : 1.0, i < 2 ? -1.0 : 1.0, 0.0));
    }

    // Create a square face
    vcl::uint sfi        = dynamicMesh.addFace();
    auto&     squareFace = dynamicMesh.face(sfi);
    std::cout << "Created empty face, vertices: " << squareFace.vertexNumber()
              << "\n";

    // Add vertices one by one
    squareFace.pushVertex(0u);
    std::cout << "After adding 1st vertex: " << squareFace.vertexNumber()
              << " vertices\n";

    squareFace.pushVertex(1u);
    squareFace.pushVertex(3u);
    squareFace.pushVertex(2u);
    std::cout << "After adding all vertices: " << squareFace.vertexNumber()
              << " vertices\n";

    // Clear and reset vertices
    squareFace.clearVertices();
    std::cout << "After clearing: " << squareFace.vertexNumber()
              << " vertices\n";

    // Set multiple vertices at once
    squareFace.setVertices(0u, 1u, 3u, 2u);
    std::cout << "After setVertices: " << squareFace.vertexNumber()
              << " vertices\n";

    // ========================================
    // PART 4: ADJACENCY IN POLYMESH
    // ========================================

    std::cout << "\n\nPART 4: Adjacency in PolyMesh\n";
    std::cout << "-----------------------------\n";

    // Enable face-face adjacency
    polyMeshLoaded.enablePerFaceAdjacentFaces();
    vcl::updatePerFaceAdjacentFaces(polyMeshLoaded);

    std::cout << "Face-face adjacency enabled for PolyMesh\n";

    // Check adjacency for first face
    if (polyMeshLoaded.faceNumber() > 0) {
        const auto& face = polyMeshLoaded.face(0);
        std::cout << "Face 0 has " << face.vertexNumber() << " vertices and "
                  << face.adjFacesNumber() << " adjacent faces\n";

        std::cout << "Adjacent faces: ";
        for (int i = 0; i < face.adjFacesNumber(); ++i) {
            const auto* adjFace = face.adjFace(i);
            if (adjFace != nullptr) {
                std::cout << adjFace->index() << " ";
            }
            else {
                std::cout << "border ";
            }
        }
        std::cout << "\n";

        // Show how adjacency corresponds to edges
        std::cout << "Adjacency per edge:\n";
        for (int i = 0; i < face.vertexNumber(); ++i) {
            const auto* adjFace = face.adjFace(i);
            std::cout << "  Edge " << i << " (v" << face.vertexIndex(i) << "-v"
                      << face.vertexIndexMod(i + 1) << "): ";
            if (adjFace != nullptr) {
                std::cout << "face " << adjFace->index() << "\n";
            }
            else {
                std::cout << "border\n";
            }
        }
    }

    // ========================================
    // PART 5: POLYGONAL FACE OPERATIONS
    // ========================================

    std::cout << "\n\nPART 5: Polygonal Face Operations\n";
    std::cout << "---------------------------------\n";

    // Analyze face properties
    if (polyMeshLoaded.faceNumber() > 0) {
        const auto& face = polyMeshLoaded.face(0);

        std::cout << "Face 0 analysis:\n";
        std::cout << "  Vertex count: " << face.vertexNumber() << "\n";
        std::cout << "  Vertices: ";
        for (int i = 0; i < face.vertexNumber(); ++i) {
            std::cout << face.vertex(i)->index() << " ";
        }
        std::cout << "\n";

        // Calculate face centroid (barycenter)
        // can be computed with:
        // vcl::faceBarycenter(face);
        vcl::Point3d centroid(0, 0, 0);
        for (int i = 0; i < face.vertexNumber(); ++i) {
            centroid += face.vertex(i)->position();
        }
        centroid /= face.vertexNumber();
        std::cout << "  Centroid: (" << centroid.x() << ", " << centroid.y()
                  << ", " << centroid.z() << ")\n";

        // Calculate face area (for convex polygons)
        // can be computed exactly with:
        // vcl::faceArea(face);
        double area = 0.0;
        for (int i = 1; i < face.vertexNumber() - 1; ++i) {
            auto v0 = face.vertex(0)->position();
            auto v1 = face.vertex(i)->position();
            auto v2 = face.vertex(i + 1)->position();

            auto edge1 = v1 - v0;
            auto edge2 = v2 - v0;
            area += 0.5 * edge1.cross(edge2).norm();
        }
        std::cout << "  Approximate area: " << area << "\n";
    }

    // ========================================
    // PART 6: CONSIDERATIONS
    // ========================================

    std::cout << "\n\nPART 6: Considerations\n";
    std::cout << "---------------------------------\n";

    // Show statistics for both mesh types
    std::cout << "Memory and performance considerations:\n";

    std::cout << "TriMesh characteristics:\n";
    std::cout << "  - Fixed 3 vertices per face\n";
    std::cout << "  - Predictable memory layout\n";
    std::cout << "  - Optimal for triangle-based algorithms\n";
    std::cout << "  - Standard format for graphics pipelines\n\n";

    std::cout << "PolyMesh characteristics:\n";
    std::cout << "  - Variable vertices per face\n";
    std::cout << "  - Dynamic memory allocation\n";
    std::cout << "  - Preserves original face structure\n";
    std::cout << "  - Better for CAD and architectural models\n\n";

    // Calculate storage efficiency
    size_t triMeshFaceMemory =
        triMeshLoaded.faceNumber() * 3; // 3 vertex indices per face
    size_t polyMeshFaceMemory = 0;
    for (const auto& face : polyMeshLoaded.faces()) {
        polyMeshFaceMemory += face.vertexNumber();
    }

    std::cout << "Storage comparison for cube:\n";
    std::cout << "  TriMesh face indices: " << triMeshFaceMemory << "\n";
    std::cout << "  PolyMesh face indices: " << polyMeshFaceMemory << "\n";
    std::cout << "  Ratio: " << (double) triMeshFaceMemory / polyMeshFaceMemory
              << "\n";

    // ========================================
    // SUMMARY
    // ========================================

    std::cout << "\n\n=== EXAMPLE SUMMARY ===\n";
    std::cout << "\nIn this example we learned:\n\n";

    std::cout << "1. POLYMESH CREATION:\n";
    std::cout << "   - addFace() creates empty polygonal faces\n";
    std::cout << "   - pushVertex() adds vertices to faces dynamically\n";
    std::cout
        << "   - clearVertices() and setVertices() for face manipulation\n\n";

    std::cout << "2. TRIMESH VS POLYMESH:\n";
    std::cout
        << "   - TriMesh: fixed 3 vertices per face, optimized for triangles\n";
    std::cout << "   - PolyMesh: variable vertices per face, preserves "
                 "original geometry\n";
    std::cout << "   - Different memory and performance characteristics\n\n";

    std::cout << "3. DYNAMIC FACE MANIPULATION:\n";
    std::cout << "   - Faces can be modified after creation\n";
    std::cout << "   - Vertices can be added/removed dynamically\n";
    std::cout << "   - Adjacency information updates automatically\n\n";

    std::cout << "4. POLYMESH-SPECIFIC FEATURES:\n";
    std::cout << "   - vertexNumber() varies per face\n";
    std::cout << "   - adjFacesNumber() matches vertex count\n";
    std::cout << "   - Face operations work with arbitrary polygon sizes\n\n";

    std::cout << "PolyMesh is ideal for:\n";
    std::cout << "- CAD and architectural models\n";
    std::cout << "- Preserving original geometric structure\n";
    std::cout << "- Working with quad-dominant or mixed meshes\n";
    std::cout << "- Applications requiring flexible face definitions\n\n";

    std::cout << "Example completed successfully!\n";

    return std::make_tuple(polyMesh, triMesh, dynamicMesh);
}

#endif // POLYMESH_SPECIFICS_H
