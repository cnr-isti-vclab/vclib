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

#include "mesh_copy_and_trasnform.h"

int main()
{
    auto [transformMesh, scaleMesh, rotateMesh, matrixMesh, combinedMesh] =
        meshCopyAndTransform();

    /****** Save the created meshes ******/

    std::cout << "\n=== Saving Meshes ===" << std::endl;

    try {
        // Save some of the created meshes
        std::string resultsPath = VCLIB_RESULTS_PATH;

        vcl::saveMesh(
            transformMesh, resultsPath + "/004_transformed_icosahedron.ply");
        std::cout << "Saved: 004_transformed_icosahedron.ply (translated "
                     "icosahedron)\n";

        vcl::saveMesh(scaleMesh, resultsPath + "/004_scaled_cube.ply");
        std::cout << "Saved: 004_scaled_cube.ply (scaled cube)\n";

        vcl::saveMesh(rotateMesh, resultsPath + "/004_rotated_cube.ply");
        std::cout << "Saved: 004_rotated_cube.ply (rotated cube)\n";

        vcl::saveMesh(matrixMesh, resultsPath + "/004_matrix_transformed.ply");
        std::cout
            << "Saved: 004_matrix_transformed.ply (matrix transformation)\n";

        vcl::saveMesh(combinedMesh, resultsPath + "/004_combined_scene.ply");
        std::cout << "Saved: 004_combined_scene.ply (combined scene)\n";

        std::cout << "\nAll files have been saved to: " << resultsPath << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error in saving: " << e.what() << "\n";
    }

    std::cout << "Example completed successfully!\n";

    return 0;
}
