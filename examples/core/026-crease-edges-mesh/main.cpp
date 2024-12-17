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

#include "crease_edges_mesh.h"

int main()
{
    vcl::TriMesh c = vcl::createCylinder<vcl::TriMesh>(1, 1, 36);

    vcl::EdgeMesh e = creaseEdgesMesh<vcl::EdgeMesh>(c);

    vcl::SaveSettings s;
    s.binary = false;

    vcl::save(c, VCLIB_RESULTS_PATH "/cylinder.ply");
    vcl::save(e, VCLIB_RESULTS_PATH "/cylinder_crease_edges.ply", s);

    return 0;
}
