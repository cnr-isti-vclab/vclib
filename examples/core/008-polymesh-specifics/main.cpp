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

#include "polymesh_specifics.h"

int main()
{
    auto [polyMesh, triMesh, dynamicMesh] = polymeshSpecifics();

    /****** Save the created meshes ******/

    std::cout << "\n=== Saving Meshes ===" << std::endl;

    try {
        std::string resultsPath = VCLIB_RESULTS_PATH;

        // Save the created polymesh
        vcl::saveMesh(polyMesh, resultsPath + "/008_hexagon_pyramid_polymesh.ply");
        std::cout << "Saved: 008_hexagon_polymesh.ply\n";

        // Save the converted TriMesh
        vcl::saveMesh(triMesh, resultsPath + "/008_hexagon_pyramid_trimesh.ply");
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
