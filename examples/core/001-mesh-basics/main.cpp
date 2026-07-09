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

    std::cout << "\n=== Saving Mesh ===" << std::endl;

    try {
        std::string resultsPath = VCLIB_CORE_RESULTS_PATH;

        vcl::saveMesh(mesh, resultsPath + "/001_mesh-basics_triangle.ply");
        vcl::saveMesh(polyMesh, resultsPath + "/001_mesh-basics_polygon.ply");

        std::cout << "Saved meshes to results directory" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error in saving: " << e.what() << "\n";
    }

    std::cout << "Example completed successfully!\n";

    return 0;
}
