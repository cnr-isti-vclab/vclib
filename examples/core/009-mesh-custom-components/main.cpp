// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "mesh_custom_components.h"

int main()
{
    auto [mesh] = meshCustomComponents();

    /****** Save the created meshes ******/

    std::cout << "\n=== Saving Meshes ===" << std::endl;

    try {
        std::string resultsPath = VCLIB_CORE_RESULTS_PATH;

        // save the mesh after processing
        // note: ply format is able to save custom components having primitive
        // types
        vcl::saveMesh(mesh, resultsPath + "/009_processed_bimba.ply");

        std::cout << "\nAll files have been saved to: " << resultsPath << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error in saving: " << e.what() << "\n";
    }

    return 0;
}
