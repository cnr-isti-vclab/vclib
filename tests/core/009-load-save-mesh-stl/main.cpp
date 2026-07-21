// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/algorithms.h>
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
        REQUIRE(tm.vertexCount() == 36);
        REQUIRE(tm.faceCount() == 12);
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

        const unsigned int expectedStlSize = 7 * tm.faceCount() + 2;

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
