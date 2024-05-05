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
#include <vclib/load_save.h>
#include <vclib/meshes.h>

TEMPLATE_TEST_CASE(
    "TriMesh Filter",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh = TestType;

    TriMesh tm = vcl::loadPly<TriMesh>(VCLIB_ASSETS_PATH "/cube_tri.ply");

    THEN("The mesh has 8 vertices, 12 triangles")
    {
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 12);
    }

    std::vector<bool> filter = {
        true, false, false, true, false, false, true, true};

    TriMesh anotherMesh = vcl::perVertexMeshFilter(tm, filter);

    THEN("The mesh has been filtered")
    {
        REQUIRE(anotherMesh.vertexNumber() == 4);
        REQUIRE(anotherMesh.faceNumber() == 0);

        REQUIRE(anotherMesh.hasPerVertexCustomComponent("birthVertex"));
        REQUIRE(
            anotherMesh.vertex(0).template customComponent<uint>(
                "birthVertex") == 0);
        REQUIRE(
            anotherMesh.vertex(1).template customComponent<uint>(
                "birthVertex") == 3);
        REQUIRE(
            anotherMesh.vertex(2).template customComponent<uint>(
                "birthVertex") == 6);
        REQUIRE(
            anotherMesh.vertex(3).template customComponent<uint>(
                "birthVertex") == 7);
    }
}
