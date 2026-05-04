/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

    SECTION("Union")
    {
        TriMesh tm1 =
            vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
        TriMesh tm2 =
            vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj");

        TriMesh result =
            vcl::igl::meshBoolean(tm1, tm2, ::igl::MESH_BOOLEAN_TYPE_UNION);

        REQUIRE(result.vertexCount() == 43022);
        REQUIRE(result.faceCount() == 86044);
    }

    SECTION("Intersection")
    {
        TriMesh tm1 =
            vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
        TriMesh tm2 =
            vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj");

        TriMesh result =
            vcl::igl::meshBoolean(tm1, tm2, ::igl::MESH_BOOLEAN_TYPE_INTERSECT);

        REQUIRE(result.vertexCount() == 10898);
        REQUIRE(result.faceCount() == 21788);
    }
}
