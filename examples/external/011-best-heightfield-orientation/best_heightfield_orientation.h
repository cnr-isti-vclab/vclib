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

#ifndef BEST_ORIENTATION_HEIGHTFIELD_H
#define BEST_ORIENTATION_HEIGHTFIELD_H

#include <vclib/embree/heightfield_exterior_volume.h>
#include <vclib/io.h>

auto bestHeightfieldOrientation(
    const std::string& filename,
    const std::string& name,
    double scaleFactor)
{
    using namespace vcl;

    const Point2d  gridCellSideLengths = {0.184, 0.234};
    constexpr uint NUM_PLANES          = 2000;
    constexpr double EPSILON           = 1e-6;

    const Point3d Z = Point3d(0, 0, 1);

    vcl::ConsoleLogger logger;

    // brain

    TriMesh m = loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/" + filename);
    m.name() = name + "_input";

    scale(m, scaleFactor);
    updatePerVertexAndFaceNormals(m);

    vcl::Timer timer(name + " Best Orientation");
    Point3d bestNormal =
        embree::findBestOrientationByHeightfieldExteriorVolume(
            m, gridCellSideLengths, NUM_PLANES, EPSILON, logger);
    timer.stop();

    // print results

    std::cout << name << " Best plane normal: " << bestNormal << "\n";
    std::cout << name << " execution time: " << timer.delay() << " s\n";

    Matrix33d rMatrix = vcl::rotationMatrix<Matrix33d>(bestNormal, Z);

    auto mOriented = m;

    vcl::rotate(mOriented, rMatrix);
    mOriented.name() = name + "_oriented";

    embree::VolumeResultMeshes outMeshes;

    embree::heightfieldExteriorVolume(
        m,
        embree::Scene(m),
        gridCellSideLengths,
        bestNormal,
        EPSILON,
        outMeshes);

    vcl::rotate(outMeshes.exteriorVolumeMesh, rMatrix);
    outMeshes.exteriorVolumeMesh.name() = name + "_exterior_volume";

    vcl::rotate(outMeshes.grid2dMesh, rMatrix);
    outMeshes.grid2dMesh.name() = name + "_grid_2d";

    return std::make_tuple(
        m,
        mOriented,
        outMeshes.exteriorVolumeMesh,
        outMeshes.grid2dMesh);
}

#endif // BEST_ORIENTATION_HEIGHTFIELD_H
