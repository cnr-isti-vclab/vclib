// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/mesh.h>
#include <vclib/meshes.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

// Create a mesh type that inherits from MeshProviderMixin
// We use the newly defined MeshProvider directly which already does this!
// We can just use vcl::MeshProvider<TestType> directly.

TEMPLATE_TEST_CASE(
    "Test MeshProviderMixin",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using MeshType = vcl::MeshProvider<TestType>;
    using PointT   = typename TestType::VertexType::PositionType;
    using Provider = const vcl::AbstractMeshProvider*;

    MeshType m;
    m.addVertices(
        PointT(0, 0, 0), PointT(1, 0, 0), PointT(0, 1, 0), PointT(1, 1, 0));
    m.reserveFaces(2);
    m.addFace(0, 1, 2);
    m.addFace(1, 3, 2);

    Provider provider = &m;

    THEN("Direct Getters work")
    {
        vcl::Point3d p0 = provider->vertexPosition(0);
        vcl::Point3d p1 = provider->vertexPosition(1);

        REQUIRE(p0 == m.vertex(0).position().template cast<double>());
        REQUIRE(p1 == m.vertex(1).position().template cast<double>());

        auto f0_pos = provider->facePositions(0);
        REQUIRE(f0_pos.size() == 3);
        REQUIRE(f0_pos[0] == m.vertex(0).position().template cast<double>());
        REQUIRE(f0_pos[1] == m.vertex(1).position().template cast<double>());
        REQUIRE(f0_pos[2] == m.vertex(2).position().template cast<double>());
        
        REQUIRE(provider->vertexCount() == 4);
        REQUIRE(provider->faceCount() == 2);
        REQUIRE(provider->edgeCount() == 0); // these meshes don't explicitly instantiate edges unless asked, wait, actually let's just check they compile
    }

    THEN("Callback Getters work")
    {
        provider->queryVertexPosition(0, [&](const vcl::Point3d& p) {
            REQUIRE(p == m.vertex(0).position().template cast<double>());
        });

    }

    THEN("Standalone MeshProviderReference works")
    {
        TestType baseMesh;
        baseMesh.addVertices(
            PointT(0, 0, 0),
            PointT(1, 0, 0),
            PointT(0, 1, 0),
            PointT(1, 1, 0));
        baseMesh.reserveFaces(2);
        baseMesh.addFace(0, 1, 2);
        baseMesh.addFace(1, 3, 2);

        vcl::MeshProviderReference<TestType> standaloneProvider(baseMesh);
        const vcl::AbstractMeshProvider* p = &standaloneProvider;

        REQUIRE(p->vertexPosition(0) == baseMesh.vertex(0).position().template cast<double>());
        auto f_pos = p->facePositions(1);
        REQUIRE(f_pos.size() == 3);
        REQUIRE(f_pos[0] == baseMesh.vertex(1).position().template cast<double>());
        REQUIRE(f_pos[1] == baseMesh.vertex(3).position().template cast<double>());
        REQUIRE(f_pos[2] == baseMesh.vertex(2).position().template cast<double>());
    }
}
