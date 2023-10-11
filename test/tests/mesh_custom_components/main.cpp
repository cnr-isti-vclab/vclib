/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#include <iostream>

#include <vclib/load_save.h>
#include <vclib/meshes.h>
#include <vclib/algorithms.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE( "Test Custom Components and Handles" ) {
    vcl::TriMesh m;
    m.addVertices(10);

    m.addPerVertexCustomComponent<int>("flag");

    REQUIRE(m.hasPerVertexCustomComponent("flag"));

    for (vcl::TriMesh::Vertex& v : m.vertices()){
        v.customComponent<int>("flag") = -4;
    }

    REQUIRE(m.vertex(3).customComponent<int>("flag") == -4);

    vcl::CustomComponentVectorHandle<int> v =
        m.perVertexCustomComponentVectorHandle<int>("flag");

    int tmp = 8;
    for (auto& m : v){
        m = tmp;
    }
    int tmp2 = 4;

    v.front() = tmp2;

    m.vertex(1).customComponent<int>("flag") = 2;

    REQUIRE(m.vertex(0).customComponent<int>("flag") == 4);
    REQUIRE(tmp == 8);
    REQUIRE(m.vertex(9).customComponent<int>("flag") == 8);

    m.deletePerVertexCustomComponent("flag");

    REQUIRE(!m.hasPerVertexCustomComponent("flag"));
}
