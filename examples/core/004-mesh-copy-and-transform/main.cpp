// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "mesh_copy_and_trasnform.h"

int main()
{
    auto [transformMesh, scaleMesh, rotateMesh, matrixMesh, combinedMesh] =
        meshCopyAndTransform();

    /****** Save the created meshes ******/

    std::cout << "\n=== Saving Meshes ===" << std::endl;

    try {
        // Save some of the created meshes
        std::string resultsPath = VCLIB_CORE_RESULTS_PATH;

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
