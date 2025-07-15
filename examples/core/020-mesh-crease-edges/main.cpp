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

#include "mesh_crease_edges.h"

int main()
{
    auto [cylinder, icosahedron, cylinderCreaseMesh, icosaCreaseMesh, sharpEdgesMesh] = 
        meshCreaseEdges();

    /****** Save the created meshes ******/

    std::cout << "\n=== Saving Meshes ===" << std::endl;

    try {
        std::string resultsPath = VCLIB_RESULTS_PATH;
        
        vcl::SaveSettings settings;
        settings.binary = false;

        // Save original meshes
        vcl::save(cylinder, resultsPath + "/020_cylinder.ply");
        vcl::save(icosahedron, resultsPath + "/020_icosahedron.ply");
        
        // Save crease edge meshes
        vcl::save(cylinderCreaseMesh, resultsPath + "/020_cylinder_crease_edges.ply", settings);
        vcl::save(icosaCreaseMesh, resultsPath + "/020_icosahedron_crease_edges.ply", settings);
        vcl::save(sharpEdgesMesh, resultsPath + "/020_sharp_edges_only.ply", settings);

        std::cout << "\nAll files have been saved to: " << resultsPath << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error in saving: " << e.what() << "\n";
    }

    return 0;
}
