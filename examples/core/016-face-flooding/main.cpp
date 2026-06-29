// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "face_flooding.h"

int main()
{
    auto [m] = faceFlooding();

    /****** Save the result ******/

    std::cout << "\n=== Saving Mesh ===\n";

    try {
        std::string resultsPath = VCLIB_CORE_RESULTS_PATH;

        vcl::saveMesh(m, resultsPath + "/016_flooded_faces.ply");

        std::cout << "File saved to: " << resultsPath
                  << "/016_flooded_faces.ply\n";
        std::cout << "- Red face: seed for flooding\n";
        std::cout << "- Green faces: upward-facing connected faces\n";
        std::cout << "- Gray faces: other faces\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error saving file: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
