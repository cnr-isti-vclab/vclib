// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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

    std::cout << "Range mesh: " << rangeMesh.vertexCount() << " vertices, "
              << rangeMesh.faceCount() << " faces" << std::endl;

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
    vcl::uint nonManifold = vcl::nonManifoldVertexCount(mesh);
    std::cout << "Non-manifold vertices: " << nonManifold << std::endl;

    // Count holes
    vcl::uint holes = vcl::holeCount(mesh);
    std::cout << "Number of holes: " << holes << std::endl;

    std::cout << "Example completed successfully!\n";
}

#endif // MESH_TOPOLOGY_STATISTICS_H
