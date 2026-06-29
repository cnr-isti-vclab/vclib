// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "igl_booleans.h"

#include <vclib/igl/booleans.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

int main()
{
    auto [m1, m2, mUnion, mIntersection] = meshBooleans();

    /****** Save the created meshes ******/

    std::cout << "\n=== Saving Meshes ===" << std::endl;

    try {
        std::string resultsPath = VCLIB_EXTERNAL_RESULTS_PATH;

        vcl::saveMesh(m1, resultsPath + "/020_input_bimba.ply");
        std::cout << "Saved: 020_input_bimba.ply\n";

        vcl::saveMesh(m2, resultsPath + "/020_input_bunny.ply");
        std::cout << "Saved: 020_input_bunny.ply\n";

        vcl::saveMesh(mUnion, resultsPath + "/020_boolean_union.ply");
        std::cout << "Saved: 020_boolean_union.ply\n";

        vcl::saveMesh(
            mIntersection, resultsPath + "/020_boolean_intersection.ply");
        std::cout << "Saved: 020_boolean_intersection.ply\n";

        std::cout << "\nAll files have been saved to: " << resultsPath << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error in saving: " << e.what() << "\n";
    }

    return 0;
}
