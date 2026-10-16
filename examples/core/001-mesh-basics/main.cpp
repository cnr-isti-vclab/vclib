// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "mesh_basics.h"

int main()
{
    auto [mesh, polyMesh] = meshBasics();

    /****** Save the created mesh ******/

    std::println("\n=== Saving Mesh ===");

    try {
        std::string resultsPath = VCLIB_CORE_RESULTS_PATH;

        vcl::saveMesh(mesh, resultsPath + "/001_mesh-basics_triangle.ply");
        vcl::saveMesh(polyMesh, resultsPath + "/001_mesh-basics_polygon.ply");

        std::println("Saved meshes to results directory");
    }
    catch (const std::exception& e) {
        std::println(stderr, "Error in saving: {}", e.what());
    }

    std::println("Example completed successfully!");

    return 0;
}
