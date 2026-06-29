// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/algorithms.h>
#include <vclib/meshes.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEMPLATE_TEST_CASE(
    "Compute Number of Crease Edges",
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

    unsigned int creaseEdges = vcl::faceEdgesSelectionCount(cylinder);

    THEN("Compute the number of selected face edges on cylinder")
    {
        REQUIRE(creaseEdges == 36 * 4);
    }
}
