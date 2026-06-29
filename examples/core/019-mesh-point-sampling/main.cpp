// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "mesh_point_sampling.h"

int main()
{
    auto [originalMesh, allVertices, uniform, montecarlo, poisson] =
        meshPointSampling();

    /****** Save the sampled meshes ******/

    std::cout << "\n=== Saving Sampled Meshes ===" << std::endl;

    try {
        std::string resultsPath = VCLIB_CORE_RESULTS_PATH;

        vcl::saveMesh(originalMesh, resultsPath + "/019_original_mesh.ply");
        vcl::saveMesh(allVertices, resultsPath + "/019_vertices_sampling.ply");
        vcl::saveMesh(uniform, resultsPath + "/019_uniform_sampling.ply");
        vcl::saveMesh(montecarlo, resultsPath + "/019_montecarlo_sampling.ply");
        vcl::saveMesh(poisson, resultsPath + "/019_poisson_sampling.ply");

        std::cout << "Saved all sampled meshes to results directory"
                  << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error in saving: " << e.what() << "\n";
    }

    std::cout << "Example completed successfully!\n";

    return 0;
}
