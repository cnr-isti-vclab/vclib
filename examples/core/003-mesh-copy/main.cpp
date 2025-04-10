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

int main()
{
    vcl::TriMesh yCyl = vcl::createCylinder<vcl::TriMesh>(0.02, 2);
    yCyl.enablePerVertexColor();
    vcl::setPerVertexColor(yCyl, vcl::Color::Green);

    vcl::TriMesh xCyl = yCyl;
    vcl::setPerVertexColor(xCyl, vcl::Color::Red);
    vcl::rotateDeg(xCyl, vcl::Point3d(0, 0, 1), 90.f);

    vcl::savePly(yCyl, VCLIB_RESULTS_PATH "/yasis.ply");
    vcl::savePly(xCyl, VCLIB_RESULTS_PATH "/xasis.ply");

    vcl::TriMesh mesh = xCyl;
    mesh.append(yCyl);

    vcl::SaveSettings s;
    s.binary = false;

    vcl::savePly(mesh, VCLIB_RESULTS_PATH "/axis.ply", s);
}
