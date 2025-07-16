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

#include "mesh_convex_hull.h"

int main()
{
    auto [inputMesh, hullMesh, cubeHull] = meshConvexHull();

    /****** Save the results ******/

    std::cout << "\n=== Saving Meshes ===\n";

    try {
        std::string resultsPath = VCLIB_RESULTS_PATH;

        vcl::save(hullMesh, VCLIB_RESULTS_PATH "/017_convex_hull.ply");
        vcl::save(cubeHull, VCLIB_RESULTS_PATH "/017_cube_hull.ply");

        std::cout << "Files saved to: " << resultsPath << "\n";
        std::cout << "- 017_convex_hull.ply (convex hull of bunny mesh)\n";
        std::cout << "- 017_cube_hull.ply (convex hull of cube points)\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error saving files: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
