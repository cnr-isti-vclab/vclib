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

    // brain

    TriMesh m = loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/brain.ply");

    scale(m, 10.0);
    updatePerVertexAndFaceNormals(m);

    vcl::Timer tbrain("Brain Best Orientation");
    Point3d bestNormalBrain =
        embree::findBestOrientationByHeightfieldExteriorVolume(
            m, gridCellSideLengths, NUM_PLANES, 1e-6, debug);
    tbrain.stop();

    Matrix33d rMatrix = vcl::rotationMatrix<Matrix33d>(bestNormalBrain, Z);

    vcl::rotate(m, rMatrix);

    vcl::saveMesh(m, resultsPath + "/011_brain_aligned.ply");

    // bunny

    m = loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");

    scale(m, 100.0);
    updatePerVertexAndFaceNormals(m);

    vcl::Timer tbunny("Bunny Best Orientation");
    Point3d bestNormalBunny =
        embree::findBestOrientationByHeightfieldExteriorVolume(
            m, gridCellSideLengths, NUM_PLANES, 1e-6, debug);
    tbunny.stop();

    rMatrix = vcl::rotationMatrix<Matrix33d>(bestNormalBunny, Z);

    vcl::rotate(m, rMatrix);

    vcl::saveMesh(m, resultsPath + "/011_bunny_aligned.ply");

    // print results

    std::cout << "Brain Best plane normal: " << bestNormalBrain << "\n";
    std::cout << "Brain execution time: " << tbrain.delay() << " s\n";

    std::cout << "Bunny Best plane normal: " << bestNormalBunny << "\n";
    std::cout << "Bunny execution time: " << tbunny.delay() << " s\n";

    return 0;
}
