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

#include <vclib/algorithms/mesh/convex_hull.h>
#include <vclib/load_save.h>
#include <vclib/meshes/tri_mesh.h>

int main()
{
    std::vector<vcl::Point3d> v = {
        {0.0, 0.0, 0.0}, // Belongs to the convex hull surface
        {1.0, 0.0, 0.0}, // Belongs to the convex hull surface
        {0.0, 1.0, 0.0}, // Belongs to the convex hull surface
        {0.0, 0.0, 1.0}, // Belongs to the convex hull surface
        {1.0, 1.0, 0.0}, // Belongs to the convex hull surface
        {1.0, 0.0, 1.0}, // Belongs to the convex hull surface
        {0.0, 1.0, 1.0}, // Belongs to the convex hull surface
        {1.0, 1.0, 1.0}, // Belongs to the convex hull surface
        {0.5, 0.5, 0.5}, // Inside the convex hull
        {0.2, 0.2, 0.2}  // Inside the convex hull
    };

    vcl::TriMesh mesh = vcl::convexHull<vcl::TriMesh>(v, true);

    vcl::save(mesh, VCLIB_RESULTS_PATH "/convex_hull.ply");

    return 0;
}
