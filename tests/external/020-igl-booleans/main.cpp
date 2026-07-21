// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/io.h>
#include <vclib/meshes.h>

#include <vclib/igl/booleans.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEMPLATE_TEST_CASE(
    "Compute Mesh Booleans",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using namespace vcl;
    using namespace vcl::igl;

    using Mesh = TestType;

    SECTION("Union")
    {
        Mesh tm1 = loadMesh<Mesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
        Mesh tm2 = loadMesh<Mesh>(VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj");

        Mesh result = meshBoolean(tm1, tm2, MeshBoolean::UNION);

        REQUIRE(result.vertexCount() == 43022);
        REQUIRE(result.faceCount() == 86044);
    }

    SECTION("Intersection")
    {
        Mesh tm1 = loadMesh<Mesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
        Mesh tm2 = loadMesh<Mesh>(VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj");

        Mesh result = meshBoolean(tm1, tm2, MeshBoolean::INTERSECTION);

        REQUIRE(result.vertexCount() == 10898);
        REQUIRE(result.faceCount() == 21788);
    }
}
