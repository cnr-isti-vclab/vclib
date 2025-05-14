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

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

#include <iostream>

int main()
{
    using PointType = vcl::TriMesh::VertexType::PositionType;
    vcl::TriMesh m =
        vcl::loadPly<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    vcl::updatePerFaceNormals(m);

    PointType p(2, 1, 0);
    PointType c;

    for (const auto& f : m.faces()) {
        double dist = vcl::distance(p, f, c);

        std::cerr << "Face " << m.index(f) << ": \n";
        std::cerr << "\tdist: " << dist << ";\n";
        std::cerr << "\tclos: " << c << "\n";
    }

    vcl::ConsoleLogger log;
    log.enablePrintTimer();

    vcl::TriMesh m1 =
        vcl::loadObj<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj");
    vcl::TriMesh m2 =
        vcl::loadObj<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");

    vcl::updateBoundingBox(m1);
    vcl::updateBoundingBox(m2);
    vcl::updatePerFaceNormals(m1);
    vcl::updatePerFaceNormals(m2);

    log.startTimer();
    auto res =
        vcl::hausdorffDistance(m1, m2, log, vcl::HAUSDORFF_VERTEX_UNIFORM);

    std::cerr << "\n\nmin: " << res.minDist << ";\n";
    std::cerr << "max: " << res.maxDist << ";\n";
    std::cerr << "mean: " << res.meanDist << ";\n";
    std::cerr << "rms: " << res.RMSDist << ";\n";
    std::cerr << "======================================\n";

    log.startTimer();
    res = vcl::hausdorffDistance(m1, m2, log, vcl::HAUSDORFF_MONTECARLO);

    std::cerr << "\n\nmin: " << res.minDist << ";\n";
    std::cerr << "max: " << res.maxDist << ";\n";
    std::cerr << "mean: " << res.meanDist << ";\n";
    std::cerr << "rms: " << res.RMSDist << ";\n";

    return 0;
}
