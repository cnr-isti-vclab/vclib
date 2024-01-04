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
#include <vclib/load_save.h>
#include <vclib/meshes.h>

std::istringstream objPolyCube()
{
    // string containing a cube in OBJ format

    std::string s =
        "v -0.500000 -0.500000 0.500000\n"
        "v 0.500000 -0.500000 0.500000\n"
        "v -0.500000 0.500000 0.500000\n"
        "v 0.500000 0.500000 0.500000\n"
        "v -0.500000 0.500000 -0.500000\n"
        "v 0.500000 0.500000 -0.500000\n"
        "v -0.500000 -0.500000 -0.500000\n"
        "v 0.500000 -0.500000 -0.500000\n"
        "f 1 2 4 3\n"
        "f 3 4 6 5\n"
        "f 5 6 8 7\n"
        "f 7 8 2 1\n"
        "f 2 8 6 4\n"
        "f 7 1 3 5\n";

    std::istringstream ss(s);
    return ss;
}

std::istringstream objTriCube()
{
    // string containing a triangulted cube in OBJ format

    std::string s =
        "v -0.500000 -0.500000 0.500000\n"
        "v 0.500000 -0.500000 0.500000\n"
        "v -0.500000 0.500000 0.500000\n"
        "v 0.500000 0.500000 0.500000\n"
        "v -0.500000 0.500000 -0.500000\n"
        "v 0.500000 0.500000 -0.500000\n"
        "v -0.500000 -0.500000 -0.500000\n"
        "v 0.500000 -0.500000 -0.500000\n"
        "f 1 2 4\n"
        "f 1 4 3\n"
        "f 3 4 6\n"
        "f 3 6 5\n"
        "f 5 6 8\n"
        "f 5 8 7\n"
        "f 7 8 2\n"
        "f 7 2 1\n"
        "f 2 8 6\n"
        "f 2 6 4\n"
        "f 7 1 3\n"
        "f 7 3 5\n";

    std::istringstream ss(s);
    return ss;
}

// Test to load obj from a istringstream
TEST_CASE("Load OBJ cube from istringstream")
{
    SECTION("TriMesh - PolyCube")
    {
        vcl::TriMesh tm;
        auto         ss = objPolyCube();
        vcl::loadObj(tm, ss, {});
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 12);
    }

    SECTION("TriMesh - TriCube")
    {
        vcl::TriMesh tm;
        auto         ss = objTriCube();
        vcl::loadObj(tm, ss, {});
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 12);
    }

    SECTION("PolyMesh - PolyCube")
    {
        vcl::PolyMesh pm;
        auto          ss = objPolyCube();
        vcl::loadObj(pm, ss, {});
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.faceNumber() == 6);
    }

    SECTION("PolyMesh - TriCube")
    {
        vcl::PolyMesh pm;
        auto          ss = objTriCube();
        vcl::loadObj(pm, ss, {});
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.faceNumber() == 12);
    }
}
