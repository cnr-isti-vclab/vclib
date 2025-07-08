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

#include "mesh_curvature.h"

int main()
{
    auto [originalMesh, taubinMesh, pcaMesh, generalMesh] = meshCurvature();

    /****** Save the curvature meshes ******/

    std::cout << "\n=== Saving Meshes ===" << std::endl;

    try {
        std::string resultsPath = VCLIB_RESULTS_PATH;

        vcl::save(taubinMesh, VCLIB_RESULTS_PATH "/014_taubin_curvature.ply");
        vcl::save(pcaMesh, VCLIB_RESULTS_PATH "/014_pca_curvature.ply");
        vcl::save(generalMesh, VCLIB_RESULTS_PATH "/014_general_curvature.ply");

        std::cout << "\nAll files have been saved to: " << resultsPath << "\n";
        std::cout << "Files saved:" << std::endl;
        std::cout << "  - 014_taubin_curvature.ply (colored by mean curvature)" << std::endl;
        std::cout << "  - 014_pca_curvature.ply (colored by Gaussian curvature)" << std::endl;
        std::cout << "  - 014_general_curvature.ply (colored by maximum curvature)" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error saving files: " << e.what() << std::endl;
        return -1;
    }

    std::cout << "\n=== Example 014 completed successfully! ===" << std::endl;

    return 0;
}
