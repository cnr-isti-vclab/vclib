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

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <vclib/algorithms.h>
#include <vclib/meshes.h>

TEMPLATE_TEST_CASE(
    "Compute Crease Edges Number",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::PolyMesh,
    vcl::PolyMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf,
    vcl::PolyMeshIndexed,
    vcl::PolyMeshIndexedf)
{
    using TriMesh = TestType;

    TriMesh cylinder = vcl::createCylinder<TriMesh>(1, 1, 36);
    cylinder.enablePerFaceAdjacentFaces();

    vcl::updatePerFaceAdjacentFaces(cylinder);

    // select crease edges
    vcl::selectCreaseFaceEdges(cylinder, -vcl::toRad(30.f), vcl::toRad(30.f));

    uint creaseEdges = vcl::faceEdgesSelectionNumber(cylinder);

    THEN("Compute the number of selected face edges on cylinder")
    {
        REQUIRE(creaseEdges == 36 * 4);
    }
}


