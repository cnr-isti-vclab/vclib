// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/algorithms/mesh/stat/topology.h>
#include <vclib/io/mesh/load_mesh.h>
#include <vclib/meshes/tri_mesh.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <list>
#include <utility>
#include <vector>

TEMPLATE_TEST_CASE(
    "Wedge Driven Vertex Duplication",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMesh64,
    vcl::TriMesh64f)
{
    using TriMesh   = TestType;
    using IndexType = TriMesh::IndexType;

    TriMesh tm1 =
        vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/TextureDouble.ply");

    THEN("Test the number of vertices to duplicate")
    {
        REQUIRE(vcl::verticesToDuplicateByWedgeTexCoordsCount(tm1) == 0);
    }

    TriMesh tm2 =
        vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_textured.ply");

    THEN("Test the number of vertices to duplicate")
    {
        // adding two unref vertices to the cube, just to differentiate
        // the number of vertices to duplicate
        tm2.addVertex({0.0, 0.0, 0.0});
        tm2.addVertex({2.0, 2.0, 2.0});

        std::vector<std::pair<IndexType, vcl::uint>>          vertWedgeMap;
        std::list<vcl::uint>                                  vertsToDuplicate;
        std::list<std::list<std::pair<IndexType, vcl::uint>>> facesToReassign;

        IndexType nV = vcl::verticesToDuplicateByWedgeTexCoordsCount(
            tm2, vertWedgeMap, vertsToDuplicate, facesToReassign);

        REQUIRE(tm2.vertexCount() == 10);
        REQUIRE(nV == 8);
        REQUIRE(vertWedgeMap.size() == tm2.vertexCount());
        REQUIRE(vertsToDuplicate.size() == nV);
        REQUIRE(facesToReassign.size() == nV);
    }
}
