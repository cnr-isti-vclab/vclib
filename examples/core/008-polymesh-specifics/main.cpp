// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "polymesh_specifics.h"

int main()
{
    auto [polyMesh, triMesh, dynamicMesh] = polymeshSpecifics();

    /****** Save the created meshes ******/

    std::cout << "\n=== Saving Meshes ===" << std::endl;

    try {
        std::string resultsPath = VCLIB_CORE_RESULTS_PATH;

        // Save the created polymesh
        vcl::saveMesh(
            polyMesh, resultsPath + "/008_hexagon_pyramid_polymesh.ply");
        std::cout << "Saved: 008_hexagon_polymesh.ply\n";

        // Save the converted TriMesh
        vcl::saveMesh(
            triMesh, resultsPath + "/008_hexagon_pyramid_trimesh.ply");
        std::cout << "Saved: 008_hexagon_trimesh.ply\n";

        vcl::saveMesh(dynamicMesh, resultsPath + "/008_square_polymesh.ply");
        std::cout << "Saved: 008_square_polymesh.ply\n";

        std::cout << "\nAll files have been saved to: " << resultsPath << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error in saving: " << e.what() << "\n";
    }

    return 0;
}
