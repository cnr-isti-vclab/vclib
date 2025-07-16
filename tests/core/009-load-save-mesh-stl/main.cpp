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

#include <vclib/algorithms/mesh/create/hexahedron.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

std::istringstream stlCube()
{
    // string containing a triangulted cube in STL format

    std::string s =
        "solid cube\n"
        "  facet normal 0.0 -1.0 0.0\n"
        "    outer loop\n"
        "      vertex -0.5 -0.5 0.5\n"
        "      vertex 0.5 -0.5 0.5\n"
        "      vertex -0.5 -0.5 -0.5\n"
        "    endloop\n"
        "  endfacet\n"
        "  facet normal 0.0 0.0 1.0\n"
        "    outer loop\n"
        "      vertex -0.5 -0.5 0.5\n"
        "      vertex 0.5 -0.5 0.5\n"
        "      vertex -0.5 0.5 0.5\n"
        "    endloop\n"
        "  endfacet\n"
        "  facet normal 1.0 0.0 0.0\n"
        "    outer loop\n"
        "      vertex 0.5 -0.5 0.5\n"
        "      vertex 0.5 -0.5 -0.5\n"
        "      vertex 0.5 0.5 0.5\n"
        "    endloop\n"
        "  endfacet\n"
        "  facet normal 0.0 0.0 -1.0\n"
        "    outer loop\n"
        "      vertex 0.5 -0.5 -0.5\n"
        "      vertex -0.5 -0.5 -0.5\n"
        "      vertex 0.5 0.5 -0.5\n"
        "    endloop\n"
        "  endfacet\n"
        "  facet normal -1.0 0.0 0.0\n"
        "    outer loop\n"
        "      vertex -0.5 -0.5 -0.5\n"
        "      vertex -0.5 -0.5 0.5\n"
        "      vertex -0.5 0.5 -0.5\n"
        "    endloop\n"
        "  endfacet\n"
        "  facet normal 0.0 1.0 0.0\n"
        "    outer loop\n"
        "      vertex -0.5 0.5 0.5\n"
        "      vertex 0.5 0.5 0.5\n"
        "      vertex -0.5 0.5 -0.5\n"
        "    endloop\n"
        "  endfacet\n"
        "  facet normal 0.0 0.0 -1.0\n"
        "    outer loop\n"
        "      vertex 0.5 0.5 -0.5\n"
        "      vertex -0.5 0.5 -0.5\n"
        "      vertex 0.5 -0.5 -0.5\n"
        "    endloop\n"
        "  endfacet\n"
        "  facet normal 0.0 -1.0 0.0\n"
        "    outer loop\n"
        "      vertex -0.5 -0.5 -0.5\n"
        "      vertex 0.5 -0.5 -0.5\n"
        "      vertex -0.5 0.5 -0.5\n"
        "    endloop\n"
        "  endfacet\n"
        "  facet normal 0.0 0.0 1.0\n"
        "    outer loop\n"
        "      vertex -0.5 0.5 0.5\n"
        "      vertex 0.5 0.5 0.5\n"
        "      vertex -0.5 -0.5 0.5\n"
        "    endloop\n"
        "  endfacet\n"
        "  facet normal 1.0 0.0 0.0\n"
        "    outer loop\n"
        "      vertex 0.5 -0.5 0.5\n"
        "      vertex 0.5 -0.5 -0.5\n"
        "      vertex 0.5 0.5 0.5\n"
        "    endloop\n"
        "  endfacet\n"
        "  facet normal 0.0 1.0 0.0\n"
        "    outer loop\n"
        "      vertex -0.5 0.5 0.5\n"
        "      vertex 0.5 0.5 0.5\n"
        "      vertex -0.5 0.5 -0.5\n"
        "    endloop\n"
        "  endfacet\n"
        "  facet normal 0.0 0.0 -1.0\n"
        "    outer loop\n"
        "      vertex 0.5 0.5 -0.5\n"
        "      vertex -0.5 0.5 -0.5\n"
        "      vertex 0.5 -0.5 -0.5\n"
        "    endloop\n"
        "  endfacet\n"
        "endsolid cube\n";

    std::istringstream ss(s);
    return ss;
}

// Test to load stl from a istringstream
TEMPLATE_TEST_CASE(
    "Load STL cube from istringstream",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh = TestType;

    vcl::MeshInfo info;

    SECTION("TriMesh - Cube")
    {
        TriMesh tm;
        auto    ss = stlCube();
        vcl::loadStl(tm, ss, info);
        REQUIRE(tm.vertexNumber() == 36);
        REQUIRE(tm.faceNumber() == 12);
    }
}

TEMPLATE_TEST_CASE(
    "Save STL cube in a ostringstream",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh = TestType;

    SECTION("TriMesh - Cube")
    {
        TriMesh tm = vcl::createCube<TriMesh>();

        vcl::SaveSettings settings;
        settings.binary = false;

        std::ostringstream ss;
        vcl::saveStl(tm, ss, settings);

        const unsigned int expectedStlSize = 7 * tm.faceNumber() + 2;

        // verify that ss contains 86 lines
        std::istringstream iss(ss.str());

        std::string line;
        int         count = 0;
        while (std::getline(iss, line)) {
            count++;
        }
        REQUIRE(count == expectedStlSize);
    }
}
