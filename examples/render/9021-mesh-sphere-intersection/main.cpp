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

#include <default_viewer.h>

#include "bimba_sphere_intersection.h"

int main(int argc, char** argv)
{
    vcl::Sphere<vcl::TriMesh::ScalarType> s = {
        {0, 0, 0},
        0.3
    };

    vcl::TriMesh m = bimbaSphereIntersection(s);

    m.name() = "bimba";
    m.enablePerVertexColor();
    vcl::setPerVertexColor(m, vcl::Color::Gray);

    vcl::TriMesh sm = vcl::createSphere<vcl::TriMesh>(s);

    sm.name() = "sphere";
    sm.enablePerVertexColor();
    vcl::updatePerFaceNormals(sm);
    vcl::updatePerVertexNormals(sm);
    vcl::setPerVertexColor(sm, vcl::Color::Gray);

    return showMeshesOnDefaultViewer(argc, argv, std::move(m), std::move(sm));
}
