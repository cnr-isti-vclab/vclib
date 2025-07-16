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

#ifndef MESH_CREASE_EDGES_H
#define MESH_CREASE_EDGES_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

// This example demonstrates various approaches to compute and handle crease
// edges in meshes using:
// 1. Crease edge selection based on dihedral angles
// 2. Face edge filtering and extraction
// 3. Creating edge meshes from selected crease edges

auto meshCreaseEdges()
{
    std::cout << "=== VCLib Example 020: Mesh Crease Edges ===\n\n";

    /****** Creating test meshes ******/

    std::cout << "=== Creating Test Meshes ===" << std::endl;

    // Create a cylinder that will have clear crease edges
    vcl::TriMesh cylinder = vcl::createCylinder<vcl::TriMesh>(1.0, 2.0, 24);
    cylinder.name()       = "Cylinder";
    vcl::updatePerVertexAndFaceNormals(cylinder);

    // Create an icosahedron for comparison
    vcl::TriMesh icosahedron = vcl::createIcosahedron<vcl::TriMesh>();
    icosahedron.name()       = "Icosahedron";
    vcl::updatePerVertexAndFaceNormals(icosahedron);

    std::cout << "Created cylinder: " << cylinder.vertexNumber()
              << " vertices, " << cylinder.faceNumber() << " faces"
              << std::endl;
    std::cout << "Created icosahedron: " << icosahedron.vertexNumber()
              << " vertices, " << icosahedron.faceNumber() << " faces"
              << std::endl;

    /****** Method 1: Crease Edge Selection ******/

    std::cout << "\n=== Method 1: Crease Edge Selection ===" << std::endl;

    // Enable face adjacency for crease edge detection
    cylinder.enablePerFaceAdjacentFaces();
    icosahedron.enablePerFaceAdjacentFaces();

    vcl::updatePerFaceAdjacentFaces(cylinder);
    vcl::updatePerFaceAdjacentFaces(icosahedron);

    // Select crease edges with different angle thresholds
    vcl::selectCreaseFaceEdges(cylinder, -vcl::toRad(60.0), vcl::toRad(60.0));
    vcl::uint cylinderCreaseEdges = vcl::faceEdgesSelectionNumber(cylinder);

    vcl::selectCreaseFaceEdges(
        icosahedron, -vcl::toRad(30.0), vcl::toRad(30.0));
    vcl::uint icosahedronCreaseEdges =
        vcl::faceEdgesSelectionNumber(icosahedron);

    std::cout << "Cylinder crease edges (±60°): " << cylinderCreaseEdges
              << std::endl;
    std::cout << "Icosahedron crease edges (±30°): " << icosahedronCreaseEdges
              << std::endl;

    /****** Method 2: Direct Crease Edge Computation ******/

    std::cout << "\n=== Method 2: Direct Crease Edge Computation ==="
              << std::endl;

    // Compute crease edges directly without selection
    auto cylinderCreaseList =
        vcl::creaseFaceEdges(cylinder, -vcl::toRad(60.0), vcl::toRad(60.0));
    auto icosaCreaseList = vcl::creaseFaceEdges(
        icosahedron, -vcl::toRad(30.0), vcl::toRad(30.0), true);

    std::cout << "Cylinder crease edges (direct): " << cylinderCreaseList.size()
              << std::endl;
    std::cout << "Icosahedron crease edges (with borders): "
              << icosaCreaseList.size() << std::endl;

    // Display some crease edge details
    std::cout << "First few cylinder crease edges:" << std::endl;
    for (size_t i = 0; i < std::min(size_t(5), cylinderCreaseList.size());
         ++i) {
        auto [faceIdx, edgeIdx] = cylinderCreaseList[i];
        const auto& face        = cylinder.face(faceIdx);
        double      angle       = vcl::faceDihedralAngleOnEdge(face, edgeIdx);
        std::cout << "  Face " << faceIdx << ", edge " << edgeIdx
                  << " (angle: " << vcl::toDeg(angle) << "°)" << std::endl;
    }

    /****** Method 3: Edge Mesh Creation ******/

    std::cout << "\n=== Method 3: Edge Mesh Creation ===" << std::endl;

    // Create edge meshes from selected crease edges
    vcl::EdgeMesh cylinderCreaseMesh =
        vcl::perFaceEdgeSelectionMeshFilter<vcl::EdgeMesh>(cylinder);
    cylinderCreaseMesh.name() = "Cylinder Crease Edge Mesh";

    // Re-select for icosahedron and create edge mesh
    vcl::selectCreaseFaceEdges(
        icosahedron, -vcl::toRad(30.0), vcl::toRad(30.0), true);
    vcl::EdgeMesh icosaCreaseMesh =
        vcl::perFaceEdgeSelectionMeshFilter<vcl::EdgeMesh>(icosahedron);
    icosaCreaseMesh.name() = "Icosahedron Crease Edge Mesh";

    std::cout << "Cylinder crease edge mesh: "
              << cylinderCreaseMesh.vertexNumber() << " vertices, "
              << cylinderCreaseMesh.edgeNumber() << " edges" << std::endl;
    std::cout << "Icosahedron crease edge mesh: "
              << icosaCreaseMesh.vertexNumber() << " vertices, "
              << icosaCreaseMesh.edgeNumber() << " edges" << std::endl;

    /****** Method 4: Custom Filtering ******/

    std::cout << "\n=== Method 4: Custom Filtering ===" << std::endl;

    // Create a custom filter for very sharp edges only
    auto sharpEdgeFilter = [&](const vcl::TriMesh::FaceType& face,
                               vcl::uint                     edgeIdx) {
        if (face.adjFace(edgeIdx) == nullptr)
            return false; // Skip border edges

        double angle = vcl::faceDihedralAngleOnEdge(face, edgeIdx);
        return std::abs(angle) > vcl::toRad(75.0); // Very sharp edges only
    };

    vcl::EdgeMesh sharpEdgesMesh =
        vcl::perFaceEdgeMeshFilter<vcl::EdgeMesh>(cylinder, sharpEdgeFilter);
    sharpEdgesMesh.name() = "Sharp Edges Mesh";

    std::cout << "Sharp edges only (>75°): " << sharpEdgesMesh.edgeNumber()
              << " edges" << std::endl;

    /****** Method 5: Different Angle Thresholds ******/

    std::cout << "\n=== Method 5: Angle Threshold Comparison ===" << std::endl;

    std::vector<double> thresholds = {30.0, 45.0, 60.0, 75.0, 90.0};
    std::cout << "Crease edges count by angle threshold (cylinder):"
              << std::endl;

    for (double thresh : thresholds) {
        vcl::selectCreaseFaceEdges(
            cylinder, -vcl::toRad(thresh), vcl::toRad(thresh));
        vcl::uint count = vcl::faceEdgesSelectionNumber(cylinder);
        std::cout << "  ±" << thresh << "°: " << count << " edges" << std::endl;
    }

    return std::make_tuple(
        std::move(cylinder),
        std::move(icosahedron),
        std::move(cylinderCreaseMesh),
        std::move(icosaCreaseMesh),
        std::move(sharpEdgesMesh));
}

#endif // MESH_CREASE_EDGES_H
