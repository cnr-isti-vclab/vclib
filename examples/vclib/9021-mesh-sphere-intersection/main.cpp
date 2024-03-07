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

#include <default_viewer.h>

#include "bimba_sphere_intersection.h"

int main(int argc, char** argv)
{
    vcl::Sphere<vcl::TriMesh::ScalarType> s = {
        {0, 0, 0},
        0.3
    };

    vcl::TriMesh m = bimbaSphereIntersection(s);

    m.enablePerVertexColor();
    vcl::setPerVertexColor(m, vcl::Color::Gray);

    vcl::TriMesh sm = vcl::createSphere<vcl::TriMesh>(s);
    sm.enablePerVertexColor();
    vcl::updatePerFaceNormals(sm);
    vcl::updatePerVertexNormals(sm);
    vcl::setPerVertexColor(sm, vcl::Color::Gray);

    return showMeshesOnDefaultViewer(argc, argv, m, sm);
}
