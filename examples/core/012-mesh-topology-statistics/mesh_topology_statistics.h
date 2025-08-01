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

#ifndef MESH_TOPOLOGY_STATISTICS_H
#define MESH_TOPOLOGY_STATISTICS_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

void meshTopologyStatistics()
{
    std::cout << "=== VCLib Example 012: Mesh Topology Statistics ===\n\n";

    /****** Connected components analysis ******/

    std::cout << "=== Connected Components Analysis ===" << std::endl;

    // Load a mesh with multiple components
    vcl::TriMesh rangeMesh =
        vcl::loadMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/rangemap.ply");

    rangeMesh.enablePerFaceAdjacentFaces();
    vcl::updatePerFaceAdjacentFaces(rangeMesh);

    std::cout << "Range mesh: " << rangeMesh.vertexNumber() << " vertices, "
              << rangeMesh.faceNumber() << " faces" << std::endl;

    // Find connected components
    auto components = vcl::connectedComponents(rangeMesh);
    std::cout << "Number of connected components: " << components.size()
              << std::endl;

    // Print component sizes
    for (size_t i = 0; i < components.size() && i < 5; ++i) {
        std::cout << "Component " << i << ": " << components[i].size()
                  << " faces" << std::endl;
    }
    if (components.size() > 5) {
        std::cout << "... and " << (components.size() - 5) << " more components"
                  << std::endl;
    }

    /****** Topology analysis ******/

    std::cout << "=== Topology Analysis ===" << std::endl;

    vcl::TriMesh mesh =
        vcl::loadMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/brain.ply");
    mesh.name() = "Brain Mesh";

    // Enable adjacency information for topology checks
    mesh.enablePerFaceAdjacentFaces();
    vcl::updatePerFaceAdjacentFaces(mesh);

    // Check if mesh is water tight
    bool waterTight = vcl::isWaterTight(mesh);
    std::cout << "Is water tight: " << (waterTight ? "Yes" : "No") << std::endl;

    // Count non-manifold vertices
    vcl::uint nonManifold = vcl::numberNonManifoldVertices(mesh);
    std::cout << "Non-manifold vertices: " << nonManifold << std::endl;

    // Count holes
    vcl::uint holes = vcl::numberHoles(mesh);
    std::cout << "Number of holes: " << holes << std::endl;

    std::cout << "Example completed successfully!\n";
}

#endif // MESH_TOPOLOGY_STATISTICS_H
