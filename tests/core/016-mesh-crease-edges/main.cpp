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
#include <vclib/meshes.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

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

    unsigned int creaseEdges = vcl::faceEdgesSelectionNumber(cylinder);

    THEN("Compute the number of selected face edges on cylinder")
    {
        REQUIRE(creaseEdges == 36 * 4);
    }
}
