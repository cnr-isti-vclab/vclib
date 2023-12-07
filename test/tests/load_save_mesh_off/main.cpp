/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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
#include <vclib/algorithms/create.h>
#include <vclib/algorithms/update.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>

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

// Test to load off from a istringstream
TEST_CASE("Load OFF cube from istringstream")
{
    SECTION("TriMesh - PolyCube")
    {
        vcl::TriMesh tm;
        auto         ss = offPolyCube();
        vcl::loadOff(tm, ss);
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 12);
    }

    SECTION("TriMesh - TriCube")
    {
        vcl::TriMesh tm;
        auto         ss = offTriCube();
        vcl::loadOff(tm, ss);
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 12);
    }

    SECTION("PolyMesh - PolyCube")
    {
        vcl::PolyMesh pm;
        auto          ss = offPolyCube();
        vcl::loadOff(pm, ss);
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.faceNumber() == 6);
    }

    SECTION("PolyMesh - TriCube")
    {
        vcl::PolyMesh pm;
        auto          ss = offTriCube();
        vcl::loadOff(pm, ss);
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.faceNumber() == 12);
    }
}

TEST_CASE("Save OFF to a ostringstream")
{
    SECTION("TriMesh - Cube (No Normals)")
    {
        vcl::TriMesh tm = vcl::createCube<vcl::TriMesh>();

        std::ostringstream oss;
        vcl::MeshInfo      i(tm);
        i.setVertexNormals(false);
        vcl::saveOff(tm, oss, i);

        // read some lines from oss and check them
        std::istringstream iss(oss.str());
        std::string        line;
        std::getline(iss, line);
        REQUIRE(line == "OFF");
        std::getline(iss, line);
        REQUIRE(line == "8 12 0 ");
        std::getline(iss, line);
        REQUIRE(line == "-0.5 -0.5 -0.5 ");
        for (uint i = 0; i < 8; i++)
            std::getline(iss, line); // go to face 0
        REQUIRE(line == "3 0 2 1 ");
    }

    SECTION("TriMesh - Cube (Normals)")
    {
        vcl::TriMesh tm = vcl::createCube<vcl::TriMesh>();
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
        for (uint i = 0; i < 8; i++)
            std::getline(iss, line); // go to face 0
        REQUIRE(line == "3 0 2 1 ");
    }

    SECTION("PolyMesh - Cube (Normals and Colors)")
    {
        vcl::PolyMesh pm = vcl::createCube<vcl::PolyMesh>();
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
        for (uint i = 0; i < 8; i++)
            std::getline(iss, line); // go to face 0
        REQUIRE(line == "4 2 3 1 0 ");
    }
}
