// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

template<vcl::FaceMeshConcept MeshType>
void populateTriMesh(MeshType& tm)
{
    using Point = vcl::VertexType<MeshType>::PositionType;

    // note: p3 and p4 have same position
    const Point p0(0, 0, 0);
    const Point p1(1, 0, 0);
    const Point p2(0, 1, 0);
    const Point p3(1, 1, 0);
    const Point p4(1, 1, 0); // dup of p3
    const Point p5(1, 1, 1);
    const Point p6(2, 0, 0); // unref

    tm.addVertices(p0, p1, p2, p3, p4, p5, p6);

    tm.addFace(0, 1, 2);
    tm.addFace(1, 2, 0); // dup of 0
    tm.addFace(1, 2, 0); // dup of 0
    tm.addFace(3, 1, 0);
    tm.addFace(1, 3, 0); // dup of 3
    tm.addFace(1, 4, 0); // not dup of 3 (different position)
    tm.addFace(2, 1, 0); // dup of 0
    tm.addFace(0, 1, 2); // dup of 0
    tm.addFace(5, 3, 4);
}

template<vcl::FaceMeshConcept MeshType>
void populatePolyMesh(MeshType& pm)
{
    using Point = vcl::VertexType<MeshType>::PositionType;

    // note: p3 and p4 have same position
    const Point p0(0, 0, 0);
    const Point p1(1, 0, 0);
    const Point p2(0, 1, 0);
    const Point p3(1, 1, 0);
    const Point p4(1, 1, 0);

    pm.addVertices(p0, p1, p2, p3, p4);

    pm.addFace(0, 1, 2);
    pm.addFace(0, 1, 2, 3);
    pm.addFace(0, 1, 2, 4); // not dup of 1 (different position)
    pm.addFace(0, 2, 1, 3); // dup of 1
    pm.addFace(4, 1, 2, 0); // dup of 2
    pm.addFace(0, 2, 1);    // dup of 0
}

template<vcl::EdgeMeshConcept MeshType>
void populateEdgeMesh(MeshType& m)
{
    using Point = vcl::VertexType<MeshType>::PositionType;

    const Point p0(0, 0, 0);
    const Point p1(1, 0, 0);
    const Point p2(0, 1, 0); // unref
    const Point p3(1, 1, 0);

    m.addVertices(p0, p1, p2, p3);

    m.addEdge(0, 1);
    m.addEdge(0, 3);
    m.addEdge(1, 3);
    m.addEdge(3, 1);
}

using Meshes   = std::tuple<vcl::TriMesh, vcl::PolyMesh, vcl::EdgeMesh>;
using Meshesf  = std::tuple<vcl::TriMeshf, vcl::PolyMeshf, vcl::EdgeMeshf>;
using Meshes64 = std::tuple<vcl::TriMesh64, vcl::PolyMesh64, vcl::EdgeMesh64>;
using Meshes64f =
    std::tuple<vcl::TriMesh64f, vcl::PolyMesh64f, vcl::EdgeMesh64f>;

TEMPLATE_TEST_CASE(
    "Clean Duplicated Faces",
    "",
    Meshes,
    Meshesf,
    Meshes64,
    Meshes64f)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using PolyMesh = std::tuple_element_t<1, TestType>;

    SECTION("TriMesh")
    {
        TriMesh tm;

        populateTriMesh(tm);

        REQUIRE(tm.vertexCount() == 7);
        REQUIRE(tm.faceCount() == 9);

        unsigned int nr = vcl::removeDuplicateFaces(tm);
        REQUIRE(nr == 5);
        REQUIRE(tm.vertexCount() == 7);
        REQUIRE(tm.faceCount() == 4);
    }

    SECTION("PolyMesh with triangles")
    {
        PolyMesh pm;

        populateTriMesh(pm);

        REQUIRE(pm.vertexCount() == 7);
        REQUIRE(pm.faceCount() == 9);

        unsigned int nr = vcl::removeDuplicateFaces(pm);
        REQUIRE(nr == 5);
        REQUIRE(pm.vertexCount() == 7);
        REQUIRE(pm.faceCount() == 4);
    }

    SECTION("PolyMesh with polygons")
    {
        PolyMesh pm;

        populatePolyMesh(pm);

        REQUIRE(pm.vertexCount() == 5);
        REQUIRE(pm.faceCount() == 6);

        unsigned int nr = vcl::removeDuplicateFaces(pm);

        REQUIRE(nr == 3);
        REQUIRE(pm.vertexCount() == 5);
        REQUIRE(pm.faceCount() == 3);
    }
}

TEMPLATE_TEST_CASE(
    "WaterTightness",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMesh64,
    vcl::TriMesh64f)
{
    using TriMesh = TestType;

    SECTION("A TriMesh that is not watertight")
    {
        TriMesh t =
            vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/brain.ply");

        REQUIRE(t.vertexCount() == 18844);
        REQUIRE(t.faceCount() == 36752);

        REQUIRE(!vcl::isWaterTight(t));
    }

    SECTION("A TriMesh that is watertight")
    {
        vcl::TriMesh t =
            vcl::loadMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bone.ply");

        REQUIRE(t.vertexCount() == 1872);
        REQUIRE(t.faceCount() == 3022);

        REQUIRE(vcl::isWaterTight(t));
    }
}

TEMPLATE_TEST_CASE(
    "Duplicated Vertices",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMesh64,
    vcl::TriMesh64f)
{
    using TriMesh = TestType;

    TriMesh tm;

    populateTriMesh(tm);

    REQUIRE(tm.face(5).vertex(1) == tm.vertex(4));

    unsigned int nv = vcl::removeDuplicateVertices(tm);

    REQUIRE(nv == 1);

    REQUIRE(tm.face(5).vertex(1) == tm.vertex(3));
}

TEMPLATE_TEST_CASE("Unreferenced Vertices", "", Meshes, Meshesf)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using EdgeMesh = std::tuple_element_t<2, TestType>;

    SECTION("TriMesh")
    {
        TriMesh tm;

        populateTriMesh(tm);

        unsigned int nv = vcl::unreferencedVertexCount(tm);

        REQUIRE(nv == 1);
    }

    SECTION("EdgeMesh")
    {
        EdgeMesh em;

        populateEdgeMesh(em);

        unsigned int nv = vcl::unreferencedVertexCount(em);

        REQUIRE(nv == 1);
    }
}

TEMPLATE_TEST_CASE(
    "Duplicated Vertices brain.ply",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMesh64,
    vcl::TriMesh64f)
{
    using TriMesh = TestType;

    TriMesh t = vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/brain.ply");

    unsigned int nv = vcl::removeDuplicateVertices(t);

    SECTION("Test number of duplicated vertices")
    {
        REQUIRE(nv == 453);
        REQUIRE(t.vertexCount() == 18844 - nv);
        REQUIRE(t.vertexContainerSize() == 18844);
        REQUIRE(t.faceCount() == 36752);
    }

    SECTION("Test compactness")
    {
        REQUIRE(t.vertexCount() != t.vertexContainerSize());

        t.compact();

        REQUIRE(t.vertexCount() == t.vertexContainerSize());
        REQUIRE(t.vertexCount() == 18844 - nv);
    }
}

TEST_CASE("Check Mesh References")
{
    using namespace vcl;

    SECTION("Valid TriMesh")
    {
        TriMesh tm;
        populateTriMesh(tm);
        REQUIRE(checkMeshReferences(tm));
    }

    SECTION("TriMesh with Out-of-Range Vertex Reference")
    {
        TriMesh tm;
        populateTriMesh(tm);
        tm.face(0).setVertexIndex(0, 9999);
        REQUIRE_THROWS_AS(checkMeshReferences(tm), InconsistentMeshException);
    }

    SECTION("TriMesh with NULL_INDEX Vertex Reference")
    {
        TriMesh tm;
        populateTriMesh(tm);
        tm.face(0).setVertexIndex(0, TriMesh::NULL_INDEX);
        REQUIRE_THROWS_AS(checkMeshReferences(tm), InconsistentMeshException);
    }

    SECTION("TriMesh with Reference to Deleted Vertex")
    {
        TriMesh tm;
        populateTriMesh(tm);
        // vertex 1 is referenced by face 0
        tm.deleteVertex(1);
        REQUIRE_THROWS_AS(checkMeshReferences(tm), InconsistentMeshException);
    }

    SECTION("TriMesh with Face Adjacencies")
    {
        TriMesh tm;
        tm.enableFaceAdjacentFaces();
        populateTriMesh(tm);
        updateFaceAdjacentFaces(tm);
        REQUIRE(checkMeshReferences(tm));

        // Corrupt adjacency: out of range
        tm.face(0).setAdjFace(0, 9999);
        REQUIRE_THROWS_AS(checkMeshReferences(tm), InconsistentMeshException);

        // Corrupt adjacency: pointing to deleted face
        tm.face(0).setAdjFace(0, 1);
        tm.deleteFace(1);
        REQUIRE_THROWS_AS(checkMeshReferences(tm), InconsistentMeshException);
    }

    SECTION("EdgeMesh with Edge References")
    {
        EdgeMesh em;
        populateEdgeMesh(em);
        REQUIRE(checkMeshReferences(em));

        // Out of range vertex in edge
        em.edge(0).setVertexIndex(0, 9999);
        REQUIRE_THROWS_AS(checkMeshReferences(em), InconsistentMeshException);

        // Deleted vertex referenced in edge
        em.edge(0).setVertexIndex(0, 0);
        em.deleteVertex(0);
        REQUIRE_THROWS_AS(checkMeshReferences(em), InconsistentMeshException);
    }

    SECTION("PolyMesh References")
    {
        PolyMesh pm;
        populatePolyMesh(pm);
        REQUIRE(checkMeshReferences(pm));

        pm.face(0).setVertexIndex(0, 9999);
        REQUIRE_THROWS_AS(checkMeshReferences(pm), InconsistentMeshException);
    }
}
