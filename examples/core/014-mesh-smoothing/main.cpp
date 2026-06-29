// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "mesh_smoothing.h"

int main()
{
    auto [originalMesh, laplacianMesh, cotangentMesh, taubinMesh, normalMesh] =
        meshSmoothing();

    /****** Save the smoothed meshes ******/

    std::cout << "\n=== Saving Meshes ===" << std::endl;

    try {
        std::string resultsPath = VCLIB_CORE_RESULTS_PATH;

        vcl::saveMesh(
            laplacianMesh, resultsPath + "/013_laplacian_smoothed.ply");
        vcl::saveMesh(
            cotangentMesh, resultsPath + "/013_cotangent_smoothed.ply");
        vcl::saveMesh(taubinMesh, resultsPath + "/013_taubin_smoothed.ply");
        vcl::saveMesh(normalMesh, resultsPath + "/013_normal_smoothed.ply");

        std::cout << "\nAll files have been saved to: " << resultsPath << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error in saving: " << e.what() << "\n";
    }

    return 0;
}
