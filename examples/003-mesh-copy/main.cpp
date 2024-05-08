/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include <iostream>

#include <vclib/algorithms.h>
#include <vclib/io/mesh.h>
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

    vcl::savePly(mesh, VCLIB_RESULTS_PATH "/axis.ply", false);
}
