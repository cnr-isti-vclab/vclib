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

    PolyMesh m =
        loadMesh<PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH "/brain_enlarged.ply");

    const auto    startTime  = std::chrono::steady_clock::now();
    const Point3d bestNormal =
        embree::runPlaneBeam(m, gridCellSideLengths, NUM_PLANES, debug);
    const auto endTime = std::chrono::steady_clock::now();
    const auto elapsedMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            endTime - startTime)
            .count();

    if (bestNormal.norm() == 0.0) {
        return 1;
    }

    std::cout << "Best plane normal: " << bestNormal << "\n";
    std::cout << "Tempo di esecuzione: " << elapsedMs << " ms\n";

    updatePerVertexAndFaceNormals(m);

    const std::string resultsPath = VCLIB_EXTERNAL_RESULTS_PATH;

    const Point3d Z = Point3d(0, 0, 1);

    Matrix44d rMatrix = Matrix44d::Identity();
    setTransformMatrixRotation(rMatrix, Z, bestNormal);

    vcl::applyTransformMatrix(m, rMatrix);

    vcl::saveMesh(m, resultsPath + "/011_brain_aligned.ply");

    return 0;
}
