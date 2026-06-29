// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
        std::string resultsPath = VCLIB_CORE_RESULTS_PATH;

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
