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

#include "foo_mesh.h"

#include <vclib/meshes/tri_mesh.h>

TEMPLATE_TEST_CASE("Simple User Component", "", vcl::TriMesh, vcl::TriMeshf)
{
    using TriMesh = TestType;

    FooMesh m;

    m.addVertices(10);

    for (auto& v : m.vertices()) {
        v.foo() = m.index(v) + 42;
    }

    for (uint i = 0; i < 10; ++i) {
        REQUIRE(m.vertex(i).foo() == 42 + i);
    }

    TriMesh tm;

    // when importing in a trimesh, the foo component is not copied
    // because the trimesh does not have the component in its vertices
    tm.importFrom(m);

    // reverse importFrom, to assert that everything builds correctly
    m.importFrom(tm);
}
