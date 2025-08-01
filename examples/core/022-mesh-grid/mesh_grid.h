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

#ifndef MESH_GRID_H
#define MESH_GRID_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>
#include <vclib/space.h>

#include <iostream>

// This example demonstrates various grid data structures in VCLib:
// 1. Regular grids for spatial subdivision
// 2. HashTableGrid for dynamic spatial hashing
// 3. StaticGrid for optimized read-only spatial queries
// 4. Using grids with points and mesh faces
// 5. Spatial queries: nearest neighbor, k-closest, sphere queries

void meshGridExamples()
{
    std::cout << "=== VCLib Example 022: Mesh Grid Structures ===\n\n";

    /****** Regular Grid Setup ******/

    std::cout << "=== Creating Regular Grid ===" << std::endl;

    // Create a regular 3D grid with specified bounds and resolution
    vcl::RegularGrid3<double> grid(
        vcl::Point3d(0, 0, 0),                  // min corner
        vcl::Point3d(1, 1, 1),                  // max corner
        vcl::Point3<unsigned int>(10, 10, 10)); // resolution (10x10x10 cells)

    std::cout << "Created regular grid with bounds (0,0,0) to (1,1,1)"
              << std::endl;
    std::cout << "Grid resolution: 10x10x10 cells" << std::endl;

    /****** HashTableGrid with Points ******/

    std::cout << "\n=== HashTableGrid with Points ===" << std::endl;

    // HashTableGrid allows dynamic insertion/deletion of elements
    vcl::HashTableGrid3<vcl::Point3d, double, false> hashGrid(grid);

    // Insert points into the grid
    hashGrid.insert(vcl::Point3d(0.05, 0.15, 0.25));
    hashGrid.insert(
        vcl::Point3d(0.05, 0.15, 0.25)); // duplicate won't be inserted
    hashGrid.insert(vcl::Point3d(0.02, 0.12, 0.29));
    hashGrid.insert(vcl::Point3d(0.12, 0.09, 0.32));
    hashGrid.insert(vcl::Point3d(0.24, 0.52, 0.29));

    std::cout << "Inserted 4 unique points into HashTableGrid" << std::endl;

    // Display non-empty cells
    std::cout << "Non-empty cells in HashTableGrid:" << std::endl;
    for (const auto& cell : hashGrid.nonEmptyCells()) {
        std::cout << "  Cell: " << cell << std::endl;
    }

    // Iterate over all stored values
    std::cout << "All values in HashTableGrid:" << std::endl;
    for (const auto& element : hashGrid) {
        std::cout << "  " << element.first << " -> " << element.second
                  << std::endl;
    }

    /****** Spatial Queries on HashTableGrid ******/

    std::cout << "\n=== Spatial Queries on HashTableGrid ===" << std::endl;

    // Query values in a specific cell
    auto cellValues = hashGrid.valuesInCell(vcl::Point3<unsigned int>(0, 1, 2));
    std::cout << "Values in cell (0,1,2): ";
    for (auto it = cellValues.first; it != cellValues.second; ++it) {
        std::cout << it->second << " ";
    }
    std::cout << std::endl;

    // Query values within a sphere
    auto sphereValues =
        hashGrid.valuesInSphere({vcl::Point3d(0.05, 0.15, 0.25), 0.2});
    std::cout << "Values within sphere (center: 0.05,0.15,0.25, radius: 0.2): ";
    for (const auto& value : sphereValues) {
        std::cout << value->second << " ";
    }
    std::cout << std::endl;

    // Find closest value to a query point
    auto closest = hashGrid.closestValue(vcl::Point3d(0.09, 0.09, 0.29));
    std::cout << "Closest value to (0.09,0.09,0.29): " << closest->second
              << std::endl;

    // Remove values within sphere
    hashGrid.eraseInSphere({vcl::Point3d(0.05, 0.15, 0.25), 0.2});
    std::cout << "Removed values within sphere" << std::endl;

    std::cout << "Remaining values after sphere removal:" << std::endl;
    for (const auto& element : hashGrid) {
        std::cout << "  " << element.first << " -> " << element.second
                  << std::endl;
    }

    /****** StaticGrid with Points ******/

    std::cout << "\n=== StaticGrid with Points ===" << std::endl;

    // StaticGrid is optimized for read-only operations after build()
    vcl::StaticGrid3<vcl::Point3d> staticGrid(grid);

    // Insert the same points
    staticGrid.insert(vcl::Point3d(0.05, 0.15, 0.25));
    staticGrid.insert(
        vcl::Point3d(0.05, 0.15, 0.25)); // duplicates allowed in StaticGrid
    staticGrid.insert(vcl::Point3d(0.02, 0.12, 0.29));
    staticGrid.insert(vcl::Point3d(0.12, 0.09, 0.32));
    staticGrid.insert(vcl::Point3d(0.24, 0.52, 0.29));

    // Build the grid for optimized queries
    staticGrid.build();
    std::cout << "Built StaticGrid with points" << std::endl;

    // Display non-empty cells
    std::cout << "Non-empty cells in StaticGrid:" << std::endl;
    for (const auto& cell : staticGrid.nonEmptyCells()) {
        std::cout << "  Cell: " << cell << std::endl;
    }

    // Spatial queries on StaticGrid
    auto staticSphereValues =
        staticGrid.valuesInSphere({vcl::Point3d(0.05, 0.15, 0.25), 0.2});
    std::cout << "Values within sphere in StaticGrid: ";
    for (const auto& value : staticSphereValues) {
        std::cout << value->second << " ";
    }
    std::cout << std::endl;

    /****** Grids with Mesh Faces ******/

    std::cout << "\n=== Grids with Mesh Faces ===" << std::endl;

    // Create a simple mesh (hexahedron)
    vcl::TriMesh mesh = vcl::createHexahedron<vcl::TriMesh>();
    using ScalarType  = vcl::TriMesh::ScalarType;

    std::cout << "Created hexahedron mesh with " << mesh.faceNumber()
              << " faces" << std::endl;

    // Create intersection function for faces and bounding boxes
    auto intersectionFunc =
        vcl::intersectFunction<vcl::Box3<ScalarType>, vcl::TriMesh::Face>();

    // HashTableGrid storing face pointers
    vcl::HashTableGrid3<const vcl::TriMesh::Face*, ScalarType> faceHashGrid(
        mesh.faces() | vcl::views::constAddrOf, intersectionFunc);

    std::cout << "Face indices in HashTableGrid:" << std::endl;
    for (const auto& element : faceHashGrid) {
        std::cout << "  Face " << mesh.index(element.second) << " in cell "
                  << element.first << std::endl;
    }

    // Query faces near a point
    auto nearbyFaces =
        faceHashGrid.valuesInSphere({vcl::Point3<ScalarType>(-1, -1, -1), 0.5});
    std::cout << "Faces near point (-1,-1,-1) within radius 0.5:" << std::endl;
    for (const auto& face : nearbyFaces) {
        std::cout << "  Face " << mesh.index(face->second) << std::endl;
    }

    /****** StaticGrid with Mesh Faces ******/

    std::cout << "\n=== StaticGrid with Mesh Faces ===" << std::endl;

    vcl::StaticGrid3<const vcl::TriMesh::Face*, ScalarType> faceStaticGrid(
        mesh.faces() | vcl::views::constAddrOf, intersectionFunc);

    std::cout << "Built StaticGrid with mesh faces" << std::endl;

    auto staticNearbyFaces = faceStaticGrid.valuesInSphere(
        {vcl::Point3<ScalarType>(-1, -1, -1), 0.5});
    std::cout << "Faces near point (-1,-1,-1) in StaticGrid:" << std::endl;
    for (const auto& face : staticNearbyFaces) {
        std::cout << "  Face " << mesh.index(face->second) << std::endl;
    }

    /****** K-Nearest Neighbors ******/

    std::cout << "\n=== K-Nearest Neighbors Search ===" << std::endl;

    // Load a more complex mesh for k-nearest search
    try {
        mesh =
            vcl::loadMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bone.ply");
        std::cout << "Loaded bone mesh with " << mesh.vertexNumber()
                  << " vertices" << std::endl;

        // Create StaticGrid with mesh vertices
        vcl::StaticGrid vertexGrid(mesh.vertices() | vcl::views::constAddrOf);

        const vcl::Point3<ScalarType> queryPoint(0.5, 0.5, 0.5);
        const int                     k = 5;

        // Find k closest vertices
        auto kClosest = vertexGrid.kClosestValues(queryPoint, k);

        std::cout << "5 closest vertices to point (0.5,0.5,0.5):" << std::endl;
        for (const auto& vertex : kClosest) {
            std::cout << "  Vertex " << mesh.index(vertex->second) << " at "
                      << vertex->first << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Could not load bone mesh: " << e.what() << std::endl;
        std::cout << "Skipping k-nearest neighbors example" << std::endl;
    }

    std::cout << "\nExample completed successfully!" << std::endl;
}

#endif // MESH_GRID_H
