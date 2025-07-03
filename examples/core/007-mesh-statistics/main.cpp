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

// This example demonstrates how to:
// 1. Calculate geometric properties of meshes
// 2. Compute volume, surface area, and barycenter
// 3. Perform mesh quality and statistical analysis

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

int main()
{
    std::cout << "=== VCLib Example 007: Mesh Statistics ===\n\n";

    // ========================================
    // PART 1: BASIC GEOMETRIC PROPERTIES
    // ========================================

    std::cout << "PART 1: Basic Geometric Properties\n";
    std::cout << "-----------------------------------\n";

    // Load a mesh for analysis
    vcl::TriMesh mesh;
    vcl::load(mesh, VCLIB_EXAMPLE_MESHES_PATH "/bimba_simplified.obj");

    std::cout << "Loaded mesh: " << mesh.vertexNumber() << " vertices, "
              << mesh.faceNumber() << " faces\n\n";

    // Update bounding box for geometric calculations
    vcl::updateBoundingBox(mesh);
    auto bbox = mesh.boundingBox();

    std::cout << "Bounding Box:\n";
    std::cout << "  Min: (" << bbox.min().x() << ", " << bbox.min().y() << ", "
              << bbox.min().z() << ")\n";
    std::cout << "  Max: (" << bbox.max().x() << ", " << bbox.max().y() << ", "
              << bbox.max().z() << ")\n";

    auto size = bbox.size();
    std::cout << "  Size: (" << size.x() << ", " << size.y() << ", " << size.z()
              << ")\n";
    std::cout << "  Diagonal: " << bbox.diagonal() << "\n\n";

    // ========================================
    // PART 2: BARYCENTER CALCULATIONS
    // ========================================

    std::cout << "PART 2: Barycenter Calculations\n";
    std::cout << "--------------------------------\n";

    // Geometric barycenter (average of vertex positions)
    auto geomBarycenter = vcl::barycenter(mesh);
    std::cout << "Geometric barycenter: (" << geomBarycenter.x() << ", "
              << geomBarycenter.y() << ", " << geomBarycenter.z() << ")\n";

    // Shell barycenter (considering surface area)
    auto shellBarycenter = vcl::shellBarycenter(mesh);
    std::cout << "Shell barycenter: (" << shellBarycenter.x() << ", "
              << shellBarycenter.y() << ", " << shellBarycenter.z() << ")\n";

    // Compare the two barycenters
    auto barycenterDiff = (geomBarycenter - shellBarycenter).norm();
    std::cout << "Distance between barycenters: " << barycenterDiff << "\n";

    if (barycenterDiff < 1e-6) {
        std::cout << "  → Barycenters are very close (uniform distribution)\n";
    }
    else {
        std::cout << "  → Barycenters differ (non-uniform distribution)\n";
    }

    // ========================================
    // PART 3: SURFACE AREA AND VOLUME
    // ========================================

    std::cout << "\n\nPART 3: Surface Area and Volume\n";
    std::cout << "-------------------------------\n";

    // Calculate surface area
    double surfaceArea = vcl::surfaceArea(mesh);
    std::cout << "Surface area: " << surfaceArea << "\n";

    double volume = vcl::volume(mesh);
    std::cout << "Volume: " << volume << "\n";

    // Surface area to volume ratio
    if (volume > 1e-10) {
        double surfaceToVolumeRatio = surfaceArea / volume;
        std::cout << "Surface area to volume ratio: " << surfaceToVolumeRatio
                  << "\n";

        // Compare with a sphere of same volume
        double sphereRadius = std::pow(3.0 * volume / (4.0 * M_PI), 1.0 / 3.0);
        double sphereSurfaceArea = 4.0 * M_PI * sphereRadius * sphereRadius;
        double compactness       = sphereSurfaceArea / surfaceArea;
        std::cout << "Compactness (sphere/mesh surface ratio): " << compactness
                  << "\n";

        if (compactness > 0.8) {
            std::cout << "  → Shape is relatively compact (sphere-like)\n";
        }
        else {
            std::cout
                << "  → Shape has complex surface (elongated or detailed)\n";
        }
    }

    // ========================================
    // PART 4: EDGE AND FACE STATISTICS
    // ========================================

    std::cout << "\n\nPART 4: Edge and Face Statistics\n";
    std::cout << "--------------------------------\n";

    // Calculate edge length statistics
    std::vector<double> edgeLengths;
    for (const auto& face : mesh.faces()) {
        for (int i = 0; i < 3; ++i) {
            auto   v1     = face.vertex(i)->position();
            auto   v2     = face.vertex((i + 1) % 3)->position();
            double length = (v2 - v1).norm();
            edgeLengths.push_back(length);
        }
    }

    // Remove duplicate edges (each internal edge is counted twice)
    std::sort(edgeLengths.begin(), edgeLengths.end());

    double minEdge = edgeLengths.front();
    double maxEdge = edgeLengths.back();
    double avgEdge =
        std::accumulate(edgeLengths.begin(), edgeLengths.end(), 0.0) /
        edgeLengths.size();
    double medianEdge = edgeLengths[edgeLengths.size() / 2];

    std::cout << "Edge length statistics:\n";
    std::cout << "  Minimum: " << minEdge << "\n";
    std::cout << "  Maximum: " << maxEdge << "\n";
    std::cout << "  Average: " << avgEdge << "\n";
    std::cout << "  Median: " << medianEdge << "\n";
    std::cout << "  Ratio (max/min): " << maxEdge / minEdge << "\n";

    // Calculate face area statistics
    std::vector<double> faceAreas;
    for (const auto& face : mesh.faces()) {
        auto v0 = face.vertex(0)->position();
        auto v1 = face.vertex(1)->position();
        auto v2 = face.vertex(2)->position();

        // Triangle area using cross product
        auto   edge1 = v1 - v0;
        auto   edge2 = v2 - v0;
        double area  = 0.5 * edge1.cross(edge2).norm();
        faceAreas.push_back(area);
    }

    std::sort(faceAreas.begin(), faceAreas.end());

    double minArea = faceAreas.front();
    double maxArea = faceAreas.back();
    double avgArea = std::accumulate(faceAreas.begin(), faceAreas.end(), 0.0) /
                     faceAreas.size();
    double medianArea = faceAreas[faceAreas.size() / 2];

    std::cout << "\nFace area statistics:\n";
    std::cout << "  Minimum: " << minArea << "\n";
    std::cout << "  Maximum: " << maxArea << "\n";
    std::cout << "  Average: " << avgArea << "\n";
    std::cout << "  Median: " << medianArea << "\n";
    std::cout << "  Ratio (max/min): " << maxArea / minArea << "\n";

    // ========================================
    // PART 5: MESH QUALITY ANALYSIS
    // ========================================

    std::cout << "\n\nPART 5: Mesh Quality Analysis\n";
    std::cout << "-----------------------------\n";

    // Aspect ratio analysis (for triangular faces)
    std::vector<double> aspectRatios;
    int                 degenerateTriangles = 0;

    for (const auto& face : mesh.faces()) {
        auto v0 = face.vertex(0)->position();
        auto v1 = face.vertex(1)->position();
        auto v2 = face.vertex(2)->position();

        double a = (v1 - v0).norm();
        double b = (v2 - v1).norm();
        double c = (v0 - v2).norm();

        if (a < 1e-10 || b < 1e-10 || c < 1e-10) {
            degenerateTriangles++;
            continue;
        }

        double maxSide     = std::max({a, b, c});
        double minSide     = std::min({a, b, c});
        double aspectRatio = maxSide / minSide;
        aspectRatios.push_back(aspectRatio);
    }

    if (!aspectRatios.empty()) {
        std::sort(aspectRatios.begin(), aspectRatios.end());

        double minAspect = aspectRatios.front();
        double maxAspect = aspectRatios.back();
        double avgAspect =
            std::accumulate(aspectRatios.begin(), aspectRatios.end(), 0.0) /
            aspectRatios.size();

        std::cout << "Triangle aspect ratio statistics:\n";
        std::cout << "  Minimum: " << minAspect << " (1.0 = equilateral)\n";
        std::cout << "  Maximum: " << maxAspect << "\n";
        std::cout << "  Average: " << avgAspect << "\n";

        int goodTriangles = std::count_if(
            aspectRatios.begin(), aspectRatios.end(), [](double ar) {
                return ar < 2.0;
            });
        std::cout << "  Good triangles (aspect < 2.0): " << goodTriangles
                  << " (" << (100.0 * goodTriangles / aspectRatios.size())
                  << "%)\n";
    }

    if (degenerateTriangles > 0) {
        std::cout << "  Degenerate triangles found: " << degenerateTriangles
                  << "\n";
    }

    // ========================================
    // PART 6: COMPARISON WITH DIFFERENT MESH
    // ========================================

    std::cout << "\n\nPART 6: Comparison with Different Mesh\n";
    std::cout << "--------------------------------------\n";

    // Load a different mesh for comparison
    vcl::TriMesh mesh2;
    vcl::load(mesh2, VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    std::cout << "Comparison mesh (cube): " << mesh2.vertexNumber()
              << " vertices, " << mesh2.faceNumber() << " faces\n";

    double surfaceArea2 = vcl::surfaceArea(mesh2);
    auto   barycenter2  = vcl::barycenter(mesh2);

    std::cout << "Cube surface area: " << surfaceArea2 << "\n";
    std::cout << "Cube barycenter: (" << barycenter2.x() << ", "
              << barycenter2.y() << ", " << barycenter2.z() << ")\n";

    double volume2 = vcl::volume(mesh2);
    std::cout << "Cube volume: " << volume2 << "\n";

    // For a cube, we can verify the calculation
    vcl::updateBoundingBox(mesh2);
    auto   cubeSize = mesh2.boundingBox().max() - mesh2.boundingBox().min();
    double expectedVolume = cubeSize.x() * cubeSize.y() * cubeSize.z();
    std::cout << "Expected cube volume: " << expectedVolume << "\n";
    std::cout << "Volume calculation accuracy: "
              << (100.0 *
                  (1.0 - std::abs(volume2 - expectedVolume) / expectedVolume))
              << "%\n";

    // ========================================
    // SUMMARY
    // ========================================

    std::cout << "\n\n=== EXAMPLE SUMMARY ===\n";
    std::cout << "\nIn this example we learned:\n\n";

    std::cout << "1. GEOMETRIC PROPERTIES:\n";
    std::cout << "   - Bounding box calculation and analysis\n";
    std::cout << "   - barycenter(): geometric center (vertex average)\n";
    std::cout << "   - shellBarycenter(): surface-weighted center\n\n";

    std::cout << "2. VOLUMETRIC MEASUREMENTS:\n";
    std::cout << "   - surfaceArea(): total surface area\n";
    std::cout << "   - volume(): enclosed volume (closed meshes)\n";
    std::cout << "   - Compactness and shape analysis\n\n";

    std::cout << "3. STATISTICAL ANALYSIS:\n";
    std::cout << "   - Edge length distributions\n";
    std::cout << "   - Face area distributions\n";
    std::cout << "   - Triangle quality metrics (aspect ratios)\n\n";

    std::cout << "4. MESH QUALITY ASSESSMENT:\n";
    std::cout << "   - Detection of degenerate triangles\n";
    std::cout << "   - Aspect ratio analysis for triangle quality\n";
    std::cout << "   - Statistical measures for mesh uniformity\n\n";

    std::cout << "These statistics are essential for:\n";
    std::cout << "- Mesh quality assessment and validation\n";
    std::cout << "- Geometric analysis and comparison\n";
    std::cout << "- Preprocessing for simulation and rendering\n";
    std::cout << "- Shape analysis and classification\n\n";

    std::cout << "Example completed successfully!\n";

    return 0;
}
