// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "foo_component.h"

#include <vclib/meshes.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEMPLATE_TEST_CASE(
    "Simple User Component",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh = TestType;

    FooMesh m;

    m.addVertices(10);

    for (auto& v : m.vertices()) {
        v.foo() = m.index(v) + 42;
    }

    for (unsigned int i = 0; i < 10; ++i) {
        REQUIRE(m.vertex(i).foo() == 42 + i);
    }

    TriMesh tm;

    // when importing in a trimesh, the foo component is not copied
    // because the trimesh does not have the component in its vertices
    tm.importFrom(m);

    // reverse importFrom, to assert that everything builds correctly
    m.importFrom(tm);
}
