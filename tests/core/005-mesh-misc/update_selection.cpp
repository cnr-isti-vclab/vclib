// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <vclib/algorithms/mesh.h>
#include <vclib/meshes.h>

#include <catch2/catch_test_macros.hpp>

#include <numbers>

namespace {

template<typename Func>
void assertThrowsMissingSelectionComponent(Func&& func)
{
    bool thrown = false;
    try {
        func();
    }
    catch (const vcl::MissingComponentException&) {
        thrown = true;
    }
    REQUIRE(thrown);
}

} // namespace

TEST_CASE("Update Selection - Clear Selection")
{
    // --- 1. Vertex Selection ---
    {
        vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
        REQUIRE(vcl::vertexSelectionCount(tet) == 0);

        for (auto v : tet.vertices()) {
            v.selected() = true;
        }
        REQUIRE(vcl::vertexSelectionCount(tet) == tet.vertexCount());

        vcl::clearVertexSelection(tet);
        REQUIRE(vcl::vertexSelectionCount(tet) == 0);
        for (const auto v : tet.vertices()) {
            REQUIRE(!v.selected());
        }

        // Test clearElementSelection<ElemId::VERTEX>
        tet.vertex(0).selected() = true;
        tet.vertex(2).selected() = true;
        REQUIRE(vcl::vertexSelectionCount(tet) == 2);

        vcl::clearElementSelection<vcl::ElemId::VERTEX>(tet);
        REQUIRE(vcl::vertexSelectionCount(tet) == 0);
        for (const auto v : tet.vertices()) {
            REQUIRE(!v.selected());
        }
    }

    // --- 2. Face Selection ---
    {
        vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
        REQUIRE(vcl::faceSelectionCount(tet) == 0);

        for (auto f : tet.faces()) {
            f.selected() = true;
        }
        REQUIRE(vcl::faceSelectionCount(tet) == tet.faceCount());

        vcl::clearFaceSelection(tet);
        REQUIRE(vcl::faceSelectionCount(tet) == 0);
        for (const auto f : tet.faces()) {
            REQUIRE(!f.selected());
        }

        // Test clearElementSelection<ElemId::FACE>
        tet.face(1).selected() = true;
        tet.face(3).selected() = true;
        REQUIRE(vcl::faceSelectionCount(tet) == 2);

        vcl::clearElementSelection<vcl::ElemId::FACE>(tet);
        REQUIRE(vcl::faceSelectionCount(tet) == 0);
        for (const auto f : tet.faces()) {
            REQUIRE(!f.selected());
        }
    }

    // --- 3. Face Edges Selection ---
    {
        vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
        REQUIRE(vcl::faceEdgesSelectionCount(tet) == 0);

        tet.face(0).edgeSelected(0) = true;
        tet.face(0).edgeSelected(2) = true;
        tet.face(1).edgeSelected(1) = true;
        REQUIRE(vcl::faceEdgesSelectionCount(tet) == 3);

        vcl::clearFaceEdgesSelection(tet);
        REQUIRE(vcl::faceEdgesSelectionCount(tet) == 0);
        for (const auto f : tet.faces()) {
            for (vcl::uint i = 0; i < f.vertexCount(); ++i) {
                REQUIRE(!f.edgeSelected(i));
            }
        }
    }

    // --- 4. Edge Selection (EdgeMesh) ---
    {
        vcl::EdgeMesh em;
        em.addVertices(
            vcl::Point3d(0.0, 0.0, 0.0),
            vcl::Point3d(1.0, 0.0, 0.0),
            vcl::Point3d(2.0, 0.0, 0.0));
        em.addEdge(0, 1);
        em.addEdge(1, 2);

        em.edge(0).selected() = true;
        em.edge(1).selected() = true;
        REQUIRE(vcl::edgeSelectionCount(em) == 2);

        vcl::clearEdgeSelection(em);
        REQUIRE(vcl::edgeSelectionCount(em) == 0);
        for (const auto e : em.edges()) {
            REQUIRE(!e.selected());
        }

        // Test clearElementSelection<ElemId::EDGE>
        em.edge(1).selected() = true;
        REQUIRE(vcl::edgeSelectionCount(em) == 1);

        vcl::clearElementSelection<vcl::ElemId::EDGE>(em);
        REQUIRE(vcl::edgeSelectionCount(em) == 0);
        for (const auto e : em.edges()) {
            REQUIRE(!e.selected());
        }
    }
}

TEST_CASE("Update Selection - Select Non Manifold Vertices")
{
    // --- 1. Manifold Mesh (Tetrahedron) ---
    {
        vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
        tet.enableFaceAdjacentFaces();
        vcl::updateFaceAdjacentFaces(tet);

        vcl::selectNonManifoldVertices(tet);
        REQUIRE(vcl::vertexSelectionCount(tet) == 0);
        for (const auto v : tet.vertices()) {
            REQUIRE(!v.selected());
        }
    }

    // --- 2. Non-Manifold Mesh (Pinch / Bowtie Vertex) ---
    {
        // Two triangles sharing only vertex 0:
        // Face 0: (0, 1, 2)
        // Face 1: (0, 3, 4)
        vcl::TriMesh m;
        m.enableFaceAdjacentFaces();
        m.addVertices(
            vcl::Point3d(0.0, 0.0, 0.0),
            vcl::Point3d(1.0, 0.0, 0.0),
            vcl::Point3d(0.0, 1.0, 0.0),
            vcl::Point3d(-1.0, 0.0, 0.0),
            vcl::Point3d(0.0, -1.0, 0.0));
        m.addFace(0, 1, 2);
        m.addFace(0, 3, 4);

        vcl::updateFaceAdjacentFaces(m);

        // A. Default: clearSelectionFirst = true
        vcl::selectNonManifoldVertices(m, true);
        REQUIRE(m.vertex(0).selected());
        REQUIRE(!m.vertex(1).selected());
        REQUIRE(!m.vertex(2).selected());
        REQUIRE(!m.vertex(3).selected());
        REQUIRE(!m.vertex(4).selected());
        REQUIRE(vcl::vertexSelectionCount(m) == 1);

        // B. clearSelectionFirst = false: previously selected vertices remain
        // selected
        m.vertex(1).selected() = true;
        vcl::selectNonManifoldVertices(m, false);
        REQUIRE(m.vertex(0).selected());
        REQUIRE(m.vertex(1).selected());
        REQUIRE(!m.vertex(2).selected());
        REQUIRE(!m.vertex(3).selected());
        REQUIRE(!m.vertex(4).selected());
        REQUIRE(vcl::vertexSelectionCount(m) == 2);

        // C. clearSelectionFirst = true resets vertex 1
        vcl::selectNonManifoldVertices(m, true);
        REQUIRE(m.vertex(0).selected());
        REQUIRE(!m.vertex(1).selected());
        REQUIRE(vcl::vertexSelectionCount(m) == 1);
    }
}

TEST_CASE("Update Selection - Select Crease Face Edges")
{
    // Two triangles at a 90-degree angle sharing edge (0, 1):
    // Face 0: (0, 1, 2) in XY plane (z = 0), normal is (0, 0, 1)
    // Face 1: (0, 3, 1) in XZ plane (y = 0), normal is (0, -1, 0)
    // Edge (0, 1) is internal; edges (1, 2), (2, 0), (0, 3), (3, 1) are on
    // border. Dihedral angle between face 0 and face 1 is pi / 2 (~1.5708 rad).
    vcl::TriMesh m;
    m.enableFaceAdjacentFaces();
    m.addVertices(
        vcl::Point3d(0.0, 0.0, 0.0),
        vcl::Point3d(1.0, 0.0, 0.0),
        vcl::Point3d(0.0, 1.0, 0.0),
        vcl::Point3d(0.0, 0.0, 1.0));
    m.addFace(0, 1, 2);
    m.addFace(0, 3, 1);

    vcl::updateFaceAdjacentFaces(m);

    // Face 0: edge 0 is (0, 1) -> internal edge
    // Face 1: edge 2 is (1, 0) -> internal edge

    // 1. Threshold range [0.1, 0.5] does NOT include pi/2 (~1.5708)
    // So edge (0, 1) is a crease edge!
    vcl::selectCreaseFaceEdges(m, 0.1, 0.5, false);
    REQUIRE(m.face(0).edgeSelected(0));
    REQUIRE(!m.face(0).edgeSelected(1));
    REQUIRE(!m.face(0).edgeSelected(2));
    REQUIRE(m.face(1).edgeSelected(2));
    REQUIRE(!m.face(1).edgeSelected(0));
    REQUIRE(!m.face(1).edgeSelected(1));
    REQUIRE(vcl::faceEdgesSelectionCount(m) == 2);

    // 2. alsoBorderEdges = true: internal crease edge + 4 border edges = 6
    // edges
    vcl::selectCreaseFaceEdges(m, 0.1, 0.5, true);
    REQUIRE(vcl::faceEdgesSelectionCount(m) == 6);

    // 3. Threshold range [-3.0, 3.0] includes pi/2, so internal edge is NOT a
    // crease edge
    vcl::selectCreaseFaceEdges(m, -3.0, 3.0, false);
    REQUIRE(vcl::faceEdgesSelectionCount(m) == 0);

    // 4. Threshold range [-3.0, 3.0] with alsoBorderEdges = true: only 4 border
    // edges selected
    vcl::selectCreaseFaceEdges(m, -3.0, 3.0, true);
    REQUIRE(vcl::faceEdgesSelectionCount(m) == 4);
    REQUIRE(!m.face(0).edgeSelected(0));
    REQUIRE(!m.face(1).edgeSelected(2));
}

TEST_CASE("Update Selection - Selection Exceptions")
{
    // Calling selectNonManifoldVertices or selectCreaseFaceEdges without
    // FaceAdjacentFaces enabled
    vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
    // FaceAdjacentFaces is optional and not enabled

    assertThrowsMissingSelectionComponent(
        [&]() { vcl::selectNonManifoldVertices(tet); });

    assertThrowsMissingSelectionComponent(
        [&]() { vcl::selectCreaseFaceEdges(tet, 0.0, 1.0); });
}

TEST_CASE("Update Selection - 64-bit selection count return type")
{
    vcl::TriMesh64 m;
    m.addVertex(Point3d(0, 0, 0));
    m.addVertex(Point3d(1, 0, 0));
    m.addVertex(Point3d(0, 1, 0));
    m.addFace(0, 1, 2);

    m.vertex(0).selected() = true;
    m.vertex(2).selected() = true;
    m.face(0).selected()   = true;

    static_assert(
        std::same_as<decltype(vcl::vertexSelectionCount(m)), uint64_t>);
    static_assert(std::same_as<decltype(vcl::faceSelectionCount(m)), uint64_t>);
    static_assert(
        std::same_as<decltype(vcl::faceEdgesSelectionCount(m)), uint64_t>);

    REQUIRE(vcl::vertexSelectionCount(m) == 2);
    REQUIRE(vcl::faceSelectionCount(m) == 1);
}
