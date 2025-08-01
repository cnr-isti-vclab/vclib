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

#include "mesh_sphere_intersection.h"

int main()
{
    auto
        [originalMesh,
         centerSphereMesh,
         centerSphereIntersection,
         earSphereMesh,
         earSphereIntersection,
         largeSphereMesh,
         largeSphereIntersection] = meshSphereIntersection();

    /****** Save the created meshes ******/

    std::cout << "\n=== Saving Meshes ===" << std::endl;

    try {
        std::string resultsPath = VCLIB_RESULTS_PATH;

        vcl::SaveSettings settings;
        settings.binary = false;

        // Save original and intersected meshes
        vcl::saveMesh(originalMesh, resultsPath + "/021_original_bunny.ply");
        vcl::saveMesh(
            centerSphereMesh, resultsPath + "/021_center_sphere.ply", settings);
        vcl::saveMesh(
            centerSphereIntersection,
            resultsPath + "/021_center_sphere_intersection.ply",
            settings);
        vcl::saveMesh(
            earSphereMesh, resultsPath + "/021_ear_sphere.ply", settings);
        vcl::saveMesh(
            earSphereIntersection,
            resultsPath + "/021_ear_sphere_intersection.ply",
            settings);
        vcl::saveMesh(
            largeSphereMesh, resultsPath + "/021_large_sphere.ply", settings);
        vcl::saveMesh(
            largeSphereIntersection,
            resultsPath + "/021_large_sphere_intersection.ply",
            settings);

        std::cout << "\nAll files have been saved to: " << resultsPath << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error saving files: " << e.what() << std::endl;
    }

    return 0;
}
