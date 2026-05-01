/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#include "igl_booleans.h"

#include <vclib/igl/booleans.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

int main()
{
    auto [m1, m2, res] = meshBooleans();

    /****** Save the created meshes ******/

    std::cout << "\n=== Saving Meshes ===" << std::endl;

    try {
        std::string resultsPath = VCLIB_RESULTS_PATH;

        vcl::saveMesh(m1, resultsPath + "/020_input_bimba.ply");
        std::cout << "Saved: 020_input_bimba.ply\n";

        vcl::saveMesh(m1, resultsPath + "/020_input_bunny.ply");
        std::cout << "Saved: 020_input_bunny.ply\n";

        vcl::saveMesh(res, resultsPath + "/020_boolean_union.ply");
        std::cout << "Saved: 020_boolean_union.ply\n";

        std::cout << "\nAll files have been saved to: " << resultsPath << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error in saving: " << e.what() << "\n";
    }

    return 0;


    return 0;
}
