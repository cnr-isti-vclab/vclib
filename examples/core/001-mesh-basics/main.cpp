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

#include "mesh_basics.h"

int main()
{
    auto [mesh, polyMesh] = meshBasics();

    /****** Save the created mesh ******/

    std::cout << "\n=== Saving Mesh ===" << std::endl;

    vcl::save(mesh, VCLIB_RESULTS_PATH "/001_mesh-basics_triangle.ply");
    vcl::save(polyMesh, VCLIB_RESULTS_PATH "/001_mesh-basics_polygon.ply");

    std::cout << "Saved meshes to results directory" << std::endl;

    return 0;
}
