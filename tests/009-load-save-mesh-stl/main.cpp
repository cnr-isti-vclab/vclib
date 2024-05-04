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

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <vclib/algorithms/mesh/create/hexahedron.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>

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
    vcl::TriMeshf)
{
    using TriMesh = TestType;

    SECTION("TriMesh - Cube")
    {
        TriMesh tm;
        auto         ss = stlCube();
        vcl::loadStl(tm, ss);
        REQUIRE(tm.vertexNumber() == 36);
        REQUIRE(tm.faceNumber() == 12);
    }
}

TEMPLATE_TEST_CASE(
    "Save STL cube in a ostringstream",
    "",
    vcl::TriMesh,
    vcl::TriMeshf)
{
    using TriMesh = TestType;

    SECTION("TriMesh - Cube")
    {
        TriMesh tm = vcl::createCube<TriMesh>();

        std::ostringstream ss;
        vcl::saveStl(tm, ss);

        const uint expectedStlSize = 7 * tm.faceNumber() + 2;

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
