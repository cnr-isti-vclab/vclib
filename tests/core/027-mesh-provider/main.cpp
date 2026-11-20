// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/algorithms/mesh.h>
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
    vcl::TriMesh64,
    vcl::TriMesh64f,
    vcl::TriEdgeMesh)
{
    using MeshType = vcl::MeshProvider<TestType>;
    using PointT   = typename vcl::PositionType<TestType>;
    using Provider = const vcl::AbstractMeshProvider*;

    MeshType m;
    m.addVertices(
        PointT(0, 0, 0), PointT(1, 0, 0), PointT(0, 1, 0), PointT(1, 1, 0));
    m.reserveFaces(2);
    m.addFace(0, 1, 2);
    m.addFace(1, 3, 2);

    if constexpr (vcl::HasEdges<TestType>) {
        m.reserveEdges(2);
        m.addEdge(0, 1);
        m.addEdge(1, 2);
    }

    // Make sure bb is computed if needed
    vcl::updateBoundingBox(static_cast<TestType&>(m));

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

        auto f0_vert = provider->faceVertices(0);
        REQUIRE(f0_vert.size() == 3);
        REQUIRE(f0_vert[0] == 0);
        REQUIRE(f0_vert[1] == 1);
        REQUIRE(f0_vert[2] == 2);

        if constexpr (vcl::HasEdges<TestType>) {
            REQUIRE(provider->edgeCount() == 2);

            auto e0_vert = provider->edgeVertices(0);
            REQUIRE(
                (e0_vert.first == 0 || e0_vert.first == 1 ||
                 e0_vert.first == 2));

            auto e0_pos = provider->edgePositions(0);
            REQUIRE(
                e0_pos.first ==
                m.vertex(e0_vert.first).position().template cast<double>());
            REQUIRE(
                e0_pos.second ==
                m.vertex(e0_vert.second).position().template cast<double>());
        }
        else {
            REQUIRE(provider->edgeCount() == 0);
            auto e0_vert = provider->edgeVertices(0);
            REQUIRE(e0_vert.first == 0);
            REQUIRE(e0_vert.second == 0);
        }

        REQUIRE(provider->selectedVertexCount() == 0);
        REQUIRE(provider->selectedFaceCount() == 0);
        REQUIRE(provider->selectedEdgeCount() == 0);

        REQUIRE(
            provider->boundingBox().isNull() ==
            (!vcl::HasBoundingBox<TestType>) );

        REQUIRE(provider->vertexContainerSize() == 4);
        REQUIRE(provider->faceContainerSize() == 2);
        REQUIRE(!provider->isVertexDeleted(0));
        REQUIRE(!provider->isFaceDeleted(0));
    }

    THEN("Callback Getters work")
    {
        provider->queryVertexPosition(0, [&](const vcl::Point3d& p) {
            REQUIRE(p == m.vertex(0).position().template cast<double>());
        });
    }

    THEN("Non-compact mesh with deleted elements and normal availability")
    {
        m.deleteVertex(0);
        m.deleteFace(0);

        REQUIRE(provider->vertexCount() == 3);
        REQUIRE(provider->vertexContainerSize() == 4);
        REQUIRE(provider->isVertexDeleted(0));
        REQUIRE(!provider->isVertexDeleted(1));

        REQUIRE(provider->faceCount() == 1);
        REQUIRE(provider->faceContainerSize() == 2);
        REQUIRE(provider->isFaceDeleted(0));
        REQUIRE(!provider->isFaceDeleted(1));

        // Deleted elements return safe default Point3d(0, 0, 0)
        REQUIRE(provider->vertexPosition(0) == vcl::Point3d(0, 0, 0));
        REQUIRE(provider->facePositions(0).empty());
        REQUIRE(provider->faceBarycenter(0) == vcl::Point3d(0, 0, 0));

        // queryVertexPosition does not invoke callback for deleted vertex
        bool called = false;
        provider->queryVertexPosition(
            0, [&](const vcl::Point3d&) { called = true; });
        REQUIRE(!called);

        // Active vertex 1 works
        REQUIRE(
            provider->vertexPosition(1) ==
            m.vertex(1).position().template cast<double>());

        // Normals safely return Point3d() if unavailable or deleted
        REQUIRE(provider->vertexNormal(0) == vcl::Point3d(0, 0, 0));
        REQUIRE(provider->faceNormal(0) == vcl::Point3d(0, 0, 0));
    }

    THEN("Standalone MeshProviderReference works")
    {
        TestType baseMesh;
        baseMesh.addVertices(
            PointT(0, 0, 0), PointT(1, 0, 0), PointT(0, 1, 0), PointT(1, 1, 0));
        baseMesh.reserveFaces(2);
        baseMesh.addFace(0, 1, 2);
        baseMesh.addFace(1, 3, 2);

        vcl::MeshProviderReference<TestType> standaloneProvider(baseMesh);
        const vcl::AbstractMeshProvider*     p = &standaloneProvider;

        REQUIRE(
            p->vertexPosition(0) ==
            baseMesh.vertex(0).position().template cast<double>());
        auto f_pos = p->facePositions(1);
        REQUIRE(f_pos.size() == 3);
        REQUIRE(
            f_pos[0] == baseMesh.vertex(1).position().template cast<double>());
        REQUIRE(
            f_pos[1] == baseMesh.vertex(3).position().template cast<double>());
        REQUIRE(
            f_pos[2] == baseMesh.vertex(2).position().template cast<double>());
    }
}
