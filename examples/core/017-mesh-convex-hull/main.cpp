// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "mesh_convex_hull.h"

int main()
{
    auto [inputMesh, hullMesh, cubeHull] = meshConvexHull();

    /****** Save the results ******/

    std::cout << "\n=== Saving Meshes ===\n";

    try {
        std::string resultsPath = VCLIB_CORE_RESULTS_PATH;

        vcl::saveMesh(hullMesh, resultsPath + "/017_convex_hull.ply");
        vcl::saveMesh(cubeHull, resultsPath + "/017_cube_hull.ply");

        std::cout << "Files saved to: " << resultsPath << "\n";
        std::cout << "- 017_convex_hull.ply (convex hull of bunny mesh)\n";
        std::cout << "- 017_cube_hull.ply (convex hull of cube points)\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error saving files: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
