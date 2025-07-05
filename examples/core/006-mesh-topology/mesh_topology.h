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

#ifndef MESH_TOPOLOGY_H
#define MESH_TOPOLOGY_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

// This example demonstrates how to:
// 1. Work with adjacency relationships in meshes
// 2. Navigate vertex-face, vertex-vertex, and face-face adjacencies
// 3. Explore mesh topology and connectivity

inline void meshTopology()
{
    std::cout << "=== VCLib Example 006: Mesh Topology ===\n\n";

    // ========================================
    // PART 1: LOADING AND BASIC INFO
    // ========================================

    std::cout << "PART 1: Loading Mesh and Basic Information\n";
    std::cout << "-------------------------------------------\n";

    // Load a simple mesh for topology exploration
    vcl::TriMesh mesh;
    vcl::load(mesh, VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    std::cout << "Loaded triangular mesh: " << mesh.vertexNumber()
              << " vertices, " << mesh.faceNumber() << " faces\n";
    std::cout << "Expected edges: ~" << (mesh.faceNumber() * 3 / 2)
              << " (Euler's formula approximation)\n\n";

    // ========================================
    // PART 2: VERTEX-FACE ADJACENCY
    // ========================================

    std::cout << "PART 2: Vertex-Face Adjacency\n";
    std::cout << "------------------------------\n";

    // Enable and compute vertex-face adjacency
    mesh.enablePerVertexAdjacentFaces();
    vcl::updatePerVertexAdjacentFaces(mesh);

    std::cout << "Vertex-face adjacency enabled and computed\n\n";

    // Show adjacency for first few vertices
    std::cout << "Adjacent faces for first 3 vertices:\n";
    for (int i = 0; i < 3 && i < mesh.vertexNumber(); ++i) {
        const auto& vertex = mesh.vertex(i);
        std::cout << "  Vertex " << i << " is adjacent to faces: ";

        for (const auto* face : vertex.adjFaces()) {
            // this should never happen
            // if it happens, something is wrong with the mesh
            // (e.g. a face with a null pointer as vertex)
            assert(face != nullptr); // Ensure face pointer is valid
            std::cout << face->index() << " ";
        }
        std::cout << "(" << vertex.adjFaces().size() << " faces total)\n";
    }

    // Find vertex with most adjacent faces
    vcl::uint maxAdjFaces = 0;
    vcl::uint maxVertex   = 0;
    for (const auto& vertex : mesh.vertices()) {
        if (vertex.adjFaces().size() > maxAdjFaces) {
            maxAdjFaces = vertex.adjFaces().size();
            maxVertex   = vertex.index();
        }
    }
    std::cout << "\nVertex with most adjacent faces: " << maxVertex << " ("
              << maxAdjFaces << " faces)\n";

    // ========================================
    // PART 3: VERTEX-VERTEX ADJACENCY
    // ========================================

    std::cout << "\n\nPART 3: Vertex-Vertex Adjacency\n";
    std::cout << "--------------------------------\n";

    // Enable and compute vertex-vertex adjacency
    mesh.enablePerVertexAdjacentVertices();
    vcl::updatePerVertexAdjacentVertices(mesh);

    std::cout << "Vertex-vertex adjacency enabled and computed\n\n";

    // Show adjacency for first few vertices
    std::cout << "Adjacent vertices for first 3 vertices:\n";
    for (int i = 0; i < 3 && i < mesh.vertexNumber(); ++i) {
        const auto& vertex = mesh.vertex(i);
        std::cout << "  Vertex " << i << " is adjacent to vertices: ";

        for (const auto* adjVertex : vertex.adjVertices()) {
            // this should never happen
            // if it happens, something is wrong with the mesh
            // (e.g. a face with a null pointer as vertex)
            assert(adjVertex != nullptr);
            std::cout << adjVertex->index() << " ";
        }
        std::cout << "(" << vertex.adjVertices().size() << " vertices total)\n";
    }

    // Analyze vertex degrees (number of adjacent vertices)
    vcl::uint minDegree = UINT_MAX, maxDegree = 0;
    double    avgDegree = 0;
    for (const auto& vertex : mesh.vertices()) {
        vcl::uint degree = vertex.adjVertices().size();
        minDegree        = std::min(minDegree, degree);
        maxDegree        = std::max(maxDegree, degree);
        avgDegree += degree;
    }
    avgDegree /= mesh.vertexNumber();

    std::cout << "\nVertex degree statistics:\n";
    std::cout << "  Minimum degree: " << minDegree << "\n";
    std::cout << "  Maximum degree: " << maxDegree << "\n";
    std::cout << "  Average degree: " << avgDegree << "\n";

    // ========================================
    // PART 4: FACE-FACE ADJACENCY
    // ========================================

    std::cout << "\n\nPART 4: Face-Face Adjacency\n";
    std::cout << "----------------------------\n";

    // Enable and compute face-face adjacency
    mesh.enablePerFaceAdjacentFaces();
    vcl::updatePerFaceAdjacentFaces(mesh);

    std::cout << "Face-face adjacency enabled and computed\n\n";

    // Show adjacency for first few faces
    std::cout << "Adjacent faces for first 3 faces:\n";
    for (int i = 0; i < 3 && i < mesh.faceNumber(); ++i) {
        const auto& face = mesh.face(i);
        std::cout << "  Face " << i << " is adjacent to faces: ";

        for (const auto* adjFace : face.adjFaces()) {
            if (adjFace != nullptr) {
                std::cout << mesh.index(adjFace) << " ";
            }
            else {
                std::cout << "border ";
            }
        }
        std::cout << "\n";
    }

    // Count border edges (faces with null adjacent faces)
    vcl::uint borderEdges = 0;
    for (const auto& face : mesh.faces()) {
        for (const auto* adjFace : face.adjFaces()) {
            if (adjFace == nullptr) {
                borderEdges++;
            }
        }
    }
    std::cout << "\nBorder edges found: " << borderEdges << "\n";

    // ========================================
    // PART 5: MESH NAVIGATION
    // ========================================

    std::cout << "\n\nPART 5: Mesh Navigation\n";
    std::cout << "-----------------------\n";

    // Navigate around a vertex using adjacency information
    if (mesh.vertexNumber() > 0) {
        vcl::uint   startVertex = 0;
        const auto& vertex      = mesh.vertex(startVertex);

        std::cout << "Navigation example starting from vertex " << startVertex
                  << ":\n";

        std::cout << "  1-ring neighborhood (direct neighbors): ";
        for (const auto* adjVertex : vertex.adjVertices()) {
            std::cout << adjVertex->index() << " ";
        }
        std::cout << "\n";

        // 2-ring neighborhood (neighbors of neighbors)
        std::set<vcl::uint> twoRingVertices;
        for (const auto* adjVertex : vertex.adjVertices()) {
            for (const auto* adjAdjVertex : adjVertex->adjVertices()) {
                vcl::uint idx = adjAdjVertex->index();
                if (idx != startVertex) { // Exclude the starting vertex
                    twoRingVertices.insert(idx);
                }
            }
        }

        std::cout << "  2-ring neighborhood: ";
        for (vcl::uint idx : twoRingVertices) {
            std::cout << idx << " ";
        }
        std::cout << "(" << twoRingVertices.size() << " vertices)\n";

        // Star of faces around the vertex
        std::cout << "  Star of faces around vertex: ";
        for (const auto* face : vertex.adjFaces()) {
            assert(face != nullptr);
            std::cout << face->index() << " ";
        }
        std::cout << "\n";
    }

    // ========================================
    // PART 6: TOPOLOGY ANALYSIS
    // ========================================

    std::cout << "\n\nPART 6: Topology Analysis\n";
    std::cout << "-------------------------\n";

    // Verify Euler's formula: V - E + F = 2 (for a closed surface)
    vcl::uint V = mesh.vertexNumber();
    vcl::uint F = mesh.faceNumber();

    // Count edges by examining face adjacencies
    vcl::uint E = 0;
    for (const auto& face : mesh.faces()) {
        E += 3; // Each triangle has 3 edges
    }
    E /= 2;           // Each edge is shared by 2 faces (except border edges)
    E += borderEdges; // Add back the border edges

    int eulerCharacteristic = V - E + F;

    std::cout << "Euler characteristic analysis:\n";
    std::cout << "  Vertices (V): " << V << "\n";
    std::cout << "  Edges (E): " << E << "\n";
    std::cout << "  Faces (F): " << F << "\n";
    std::cout << "  V - E + F = " << eulerCharacteristic << "\n";
    std::cout << "  Expected for closed surface: 2\n";
    std::cout << "  Border edges: " << borderEdges << "\n";

    if (eulerCharacteristic == 2 && borderEdges == 0) {
        std::cout << "  → This appears to be a closed, genus-0 surface (like a "
                     "sphere)\n";
    }
    else if (borderEdges > 0) {
        std::cout << "  → This appears to be an open surface (has boundary)\n";
    }
    else {
        std::cout << "  → This may be a surface with genus > 0 or have other "
                     "topological features\n";
    }

    // ========================================
    // SUMMARY
    // ========================================

    std::cout << "\n\n=== EXAMPLE SUMMARY ===\n";
    std::cout << "\nIn this example we learned:\n\n";

    std::cout << "1. VERTEX-FACE ADJACENCY:\n";
    std::cout << "   - enablePerVertexAdjacentFaces() + "
                 "updatePerVertexAdjacentFaces()\n";
    std::cout << "   - Find all faces incident to a vertex\n";
    std::cout << "   - Useful for local surface analysis\n\n";

    std::cout << "2. VERTEX-VERTEX ADJACENCY:\n";
    std::cout << "   - enablePerVertexAdjacentVertices() + "
                 "updatePerVertexAdjacentVertices()\n";
    std::cout << "   - Find all vertices connected by an edge\n";
    std::cout << "   - Essential for graph-based algorithms\n\n";

    std::cout << "3. FACE-FACE ADJACENCY:\n";
    std::cout
        << "   - enablePerFaceAdjacentFaces() + updatePerFaceAdjacentFaces()\n";
    std::cout << "   - Find faces sharing an edge\n";
    std::cout << "   - Identify border edges (null adjacencies)\n\n";

    std::cout << "4. MESH NAVIGATION:\n";
    std::cout << "   - Use adjacency information to traverse the mesh\n";
    std::cout << "   - Compute k-ring neighborhoods\n";
    std::cout << "   - Analyze local and global topology\n\n";

    std::cout << "Topology information is essential for:\n";
    std::cout << "- Mesh processing algorithms (smoothing, subdivision)\n";
    std::cout << "- Surface analysis and feature detection\n";
    std::cout
        << "- Geometric computations requiring neighborhood information\n";
    std::cout << "- Mesh quality assessment and repair\n\n";

    std::cout << "Example completed successfully!\n";
}

#endif // MESH_TOPOLOGY_H
