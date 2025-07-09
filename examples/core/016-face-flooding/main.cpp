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

#include "face_flooding.h"

int main()
{
    auto [m] = faceFlooding();

    /****** Save the result ******/

    std::cout << "\n=== Saving Mesh ===\n";

    try {
        std::string resultsPath = VCLIB_RESULTS_PATH;

        vcl::save(m, VCLIB_RESULTS_PATH "/016_flooded_faces.ply");

        std::cout << "File saved to: " << resultsPath << "/016_flooded_faces.ply\n";
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
