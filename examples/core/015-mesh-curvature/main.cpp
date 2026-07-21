// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "mesh_curvature.h"

int main()
{
    auto [originalMesh, taubinMesh, pcaMesh, generalMesh] = meshCurvature();

    /****** Save the curvature meshes ******/

    std::cout << "\n=== Saving Meshes ===" << std::endl;

    try {
        std::string resultsPath = VCLIB_CORE_RESULTS_PATH;

        vcl::saveMesh(taubinMesh, resultsPath + "/014_taubin_curvature.ply");
        vcl::saveMesh(pcaMesh, resultsPath + "/014_pca_curvature.ply");
        vcl::saveMesh(generalMesh, resultsPath + "/014_general_curvature.ply");

        std::cout << "\nAll files have been saved to: " << resultsPath << "\n";
        std::cout << "Files saved:" << std::endl;
        std::cout << "  - 014_taubin_curvature.ply (colored by mean curvature)"
                  << std::endl;
        std::cout << "  - 014_pca_curvature.ply (colored by Gaussian curvature)"
                  << std::endl;
        std::cout
            << "  - 014_general_curvature.ply (colored by maximum curvature)"
            << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error saving files: " << e.what() << std::endl;
        return -1;
    }

    std::cout << "\n=== Example 014 completed successfully! ===" << std::endl;

    return 0;
}
