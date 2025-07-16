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

#include <vclib/algorithms/mesh/create.h>
#include <vclib/algorithms/mesh/update.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

std::istringstream offPolyCube()
{
    // string containing a cube in OFF format

    std::string s =
        "OFF\n"
        "8 6 0\n"
        "-0.500000 -0.500000 0.500000\n"
        "0.500000 -0.500000 0.500000\n"
        "-0.500000 0.500000 0.500000\n"
        "0.500000 0.500000 0.500000\n"
        "-0.500000 0.500000 -0.500000\n"
        "0.500000 0.500000 -0.500000\n"
        "-0.500000 -0.500000 -0.500000\n"
        "0.500000 -0.500000 -0.500000\n"
        "4 0 1 3 2\n"
        "4 2 3 5 4\n"
        "4 4 5 7 6\n"
        "4 6 7 1 0\n"
        "4 1 7 5 3\n"
        "4 6 0 2 4\n";

    std::istringstream ss(s);
    return ss;
}

std::istringstream offTriCube()
{
    // string containing a cube in OFF format

    std::string s =
        "OFF\n"
        "8 12 0\n"
        "-0.500000 -0.500000 0.500000\n"
        "0.500000 -0.500000 0.500000\n"
        "-0.500000 0.500000 0.500000\n"
        "0.500000 0.500000 0.500000\n"
        "-0.500000 0.500000 -0.500000\n"
        "0.500000 0.500000 -0.500000\n"
        "-0.500000 -0.500000 -0.500000\n"
        "0.500000 -0.500000 -0.500000\n"
        "3 0 1 2\n"
        "3 1 3 2\n"
        "3 2 3 4\n"
        "3 3 5 4\n"
        "3 4 5 6\n"
        "3 5 7 6\n"
        "3 6 7 0\n"
        "3 7 1 0\n"
        "3 1 7 3\n"
        "3 7 5 3\n"
        "3 5 7 6\n"
        "3 7 5 4\n";

    std::istringstream ss(s);
    return ss;
}

using Meshes         = std::pair<vcl::TriMesh, vcl::PolyMesh>;
using Meshesf        = std::pair<vcl::TriMeshf, vcl::PolyMeshf>;
using MeshesIndexed  = std::pair<vcl::TriMeshIndexed, vcl::PolyMeshIndexed>;
using MeshesIndexedf = std::pair<vcl::TriMeshIndexedf, vcl::PolyMeshIndexedf>;

// Test to load off from a istringstream
TEMPLATE_TEST_CASE(
    "Load OFF cube from istringstream",
    "",
    Meshes,
    Meshesf,
    MeshesIndexed,
    MeshesIndexedf)
{
    using TriMesh  = typename TestType::first_type;
    using PolyMesh = typename TestType::second_type;

    vcl::LoadSettings settings;

    vcl::MeshInfo loadedInfo;

    SECTION("TriMesh - PolyCube")
    {
        TriMesh tm;
        auto    ss = offPolyCube();
        vcl::loadOff(tm, ss, loadedInfo);
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 12);
    }

    SECTION("TriMesh - TriCube")
    {
        TriMesh tm;
        auto    ss = offTriCube();
        vcl::loadOff(tm, ss, loadedInfo);
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 12);
    }

    SECTION("PolyMesh - PolyCube")
    {
        PolyMesh pm;
        auto     ss = offPolyCube();
        vcl::loadOff(pm, ss, loadedInfo);
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.faceNumber() == 6);
    }

    SECTION("PolyMesh - TriCube")
    {
        PolyMesh pm;
        auto     ss = offTriCube();
        vcl::loadOff(pm, ss, loadedInfo);
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.faceNumber() == 12);
    }
}

TEMPLATE_TEST_CASE(
    "Save OFF to a ostringstream",
    "",
    Meshes,
    Meshesf,
    MeshesIndexed,
    MeshesIndexedf)
{
    using TriMesh  = typename TestType::first_type;
    using PolyMesh = typename TestType::second_type;

    SECTION("TriMesh - Cube (No Normals)")
    {
        TriMesh tm = vcl::createCube<TriMesh>();

        std::ostringstream oss;
        vcl::MeshInfo      i(tm);
        i.setPerVertexNormal(false);
        vcl::SaveSettings settings;
        settings.info = i;
        vcl::saveOff(tm, oss, settings);

        // read some lines from oss and check them
        std::istringstream iss(oss.str());
        std::string        line;
        std::getline(iss, line);
        REQUIRE(line == "OFF");
        std::getline(iss, line);
        REQUIRE(line == "8 12 0 ");
        std::getline(iss, line);
        REQUIRE(line == "-0.5 -0.5 -0.5 ");
        for (unsigned int i = 0; i < 8; i++)
            std::getline(iss, line); // go to face 0
        REQUIRE(line == "3 0 2 1 ");
    }

    SECTION("TriMesh - Cube (Normals)")
    {
        TriMesh tm = vcl::createCube<TriMesh>();
        vcl::updatePerVertexNormals(tm);

        std::ostringstream oss;
        vcl::saveOff(tm, oss);

        // read first two lines from oss and check them
        std::istringstream iss(oss.str());
        std::string        line;
        std::getline(iss, line);
        REQUIRE(line == "NOFF");
        std::getline(iss, line);
        REQUIRE(line == "8 12 0 ");
        std::getline(iss, line);
        REQUIRE(line == "-0.5 -0.5 -0.5 -0.57735 -0.57735 -0.57735 ");
        for (unsigned int i = 0; i < 8; i++)
            std::getline(iss, line); // go to face 0
        REQUIRE(line == "3 0 2 1 ");
    }

    SECTION("PolyMesh - Cube (Normals and Colors)")
    {
        PolyMesh pm = vcl::createCube<PolyMesh>();
        vcl::updatePerVertexNormals(pm);

        pm.enablePerVertexColor();
        vcl::setPerVertexColor(pm, vcl::Color::Blue);

        std::ostringstream oss;
        vcl::saveOff(pm, oss);

        // read first two lines from oss and check them
        std::istringstream iss(oss.str());
        std::string        line;
        std::getline(iss, line);
        REQUIRE(line == "NCOFF");
        std::getline(iss, line);
        REQUIRE(line == "8 6 0 ");
        std::getline(iss, line);
        REQUIRE(
            line == "-0.5 -0.5 -0.5 0 0 255 255 -0.57735 -0.57735 -0.57735 ");
        for (unsigned int i = 0; i < 8; i++)
            std::getline(iss, line); // go to face 0
        REQUIRE(line == "4 2 3 1 0 ");
    }
}
