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

#include <vclib/embree/orientation.h>
#include <vclib/io.h>

int main()
{
    using namespace vcl;

    std::vector<double> gridCellSideLengths = {0.184, 0.234};
    constexpr uint      NUM_PLANES          = 2000;
    constexpr bool      debug               = true;

    const std::string resultsPath = VCLIB_EXTERNAL_RESULTS_PATH;

    const Point3d Z = Point3d(0, 0, 1);

    PolyMesh m =
        loadMesh<PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH "/brain_enlarged.ply");

    auto    startTime  = std::chrono::steady_clock::now();
    Point3d bestNormal =
        embree::runPlaneBeam(m, gridCellSideLengths, NUM_PLANES, debug);
    auto endTime = std::chrono::steady_clock::now();
    auto elapsedMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            endTime - startTime)
            .count();

    if (bestNormal.norm() == 0.0) {
        return 1;
    }

    std::cout << "Brain Best plane normal: " << bestNormal << "\n";
    std::cout << "Brain execution time: " << elapsedMs << " ms\n";

    updatePerVertexAndFaceNormals(m);

    Matrix33d rMatrix = vcl::rotationMatrix<Matrix33d>(bestNormal, Z);

    vcl::rotate(m, rMatrix);

    vcl::saveMesh(m, resultsPath + "/011_brain_aligned.ply");

    // bunny

    m = loadMesh<PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");

    vcl::scale(m, 100.0);

    startTime  = std::chrono::steady_clock::now();
    bestNormal =
        embree::runPlaneBeam(m, gridCellSideLengths, NUM_PLANES, debug);
    endTime = std::chrono::steady_clock::now();
    elapsedMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            endTime - startTime)
            .count();

    if (bestNormal.norm() == 0.0) {
        return 1;
    }

    std::cout << "Bunny Best plane normal: " << bestNormal << "\n";
    std::cout << "Bunny execution time: " << elapsedMs << " ms\n";

    updatePerVertexAndFaceNormals(m);

    rMatrix = vcl::rotationMatrix<Matrix33d>(bestNormal, Z);

    vcl::rotate(m, rMatrix);

    vcl::saveMesh(m, resultsPath + "/011_bunny_aligned.ply");

    return 0;
}
