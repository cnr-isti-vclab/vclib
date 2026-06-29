// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "mesh_crease_edges.h"

int main()
{
    auto
        [cylinder,
         icosahedron,
         cylinderCreaseMesh,
         icosaCreaseMesh,
         sharpEdgesMesh] = meshCreaseEdges();

    /****** Save the created meshes ******/

    std::cout << "\n=== Saving Meshes ===" << std::endl;

    try {
        std::string resultsPath = VCLIB_CORE_RESULTS_PATH;

        vcl::SaveSettings settings;
        settings.binary = false;

        // Save original meshes
        vcl::saveMesh(cylinder, resultsPath + "/020_cylinder.ply");
        vcl::saveMesh(icosahedron, resultsPath + "/020_icosahedron.ply");

        // Save crease edge meshes
        vcl::saveMesh(
            cylinderCreaseMesh,
            resultsPath + "/020_cylinder_crease_edges.ply",
            settings);
        vcl::saveMesh(
            icosaCreaseMesh,
            resultsPath + "/020_icosahedron_crease_edges.ply",
            settings);
        vcl::saveMesh(
            sharpEdgesMesh,
            resultsPath + "/020_sharp_edges_only.ply",
            settings);

        std::cout << "\nAll files have been saved to: " << resultsPath << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error in saving: " << e.what() << "\n";
    }

    return 0;
}
