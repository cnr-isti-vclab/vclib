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

#ifndef BIMBA_SPHERE_INTERSECTION_H
#define BIMBA_SPHERE_INTERSECTION_H

#include <vclib/algorithms.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>

vcl::TriMesh bimbaSphereIntersection(
    vcl::Sphere<vcl::TriMesh::ScalarType> s = {
        {0, 0, 0},
        0.3
})
{
    vcl::TriMesh m = vcl::loadObj<vcl::TriMesh>(VCLIB_ASSETS_PATH "/bimba.obj");

    vcl::updatePerFaceNormals(m);
    vcl::updatePerVertexNormals(m);

    return vcl::meshSphereIntersection(m, s);
}

#endif // BIMBA_SPHERE_INTERSECTION_H
