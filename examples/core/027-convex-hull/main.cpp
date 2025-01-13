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

#include <iostream>

#include <vclib/algorithms/mesh/convex_hull.h>
#include <vclib/load_save.h>
#include <vclib/meshes/tri_mesh.h>

int main()
{
    vcl::TriMesh mesh =
        vcl::load<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj");

    vcl::ConsoleLogger log;

    log.enablePrintTimer();
    vcl::TriMesh ch = vcl::convexHull<vcl::TriMesh>(
        mesh.vertices() | vcl::views::coords, log);

    vcl::save(ch, VCLIB_RESULTS_PATH "/convex_hull.ply");

    return 0;
}
