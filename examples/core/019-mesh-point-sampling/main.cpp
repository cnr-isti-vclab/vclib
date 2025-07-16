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

#include "mesh_point_sampling.h"

int main()
{
    auto [originalMesh, allVertices, uniform, montecarlo, poisson] =
        meshPointSampling();

    /****** Save the sampled meshes ******/

    std::cout << "\n=== Saving Sampled Meshes ===" << std::endl;

    try {
        vcl::save(originalMesh, VCLIB_RESULTS_PATH "/019_original_mesh.ply");
        vcl::save(allVertices, VCLIB_RESULTS_PATH "/019_vertices_sampling.ply");
        vcl::save(uniform, VCLIB_RESULTS_PATH "/019_uniform_sampling.ply");
        vcl::save(
            montecarlo, VCLIB_RESULTS_PATH "/019_montecarlo_sampling.ply");
        vcl::save(poisson, VCLIB_RESULTS_PATH "/019_poisson_sampling.ply");

        std::cout << "Saved all sampled meshes to results directory"
                  << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error in saving: " << e.what() << "\n";
    }

    std::cout << "Example completed successfully!\n";

    return 0;
}
