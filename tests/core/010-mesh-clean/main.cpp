/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <vclib/algorithms.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>

template<vcl::FaceMeshConcept MeshType>
void populateTriMesh(MeshType& tm)
{
    using Point = MeshType::VertexType::CoordType;

    // note: p3 and p4 have same coords
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
    tm.addFace(1, 4, 0); // not dup of 3 (different coordinates)
    tm.addFace(2, 1, 0); // dup of 0
    tm.addFace(0, 1, 2); // dup of 0
    tm.addFace(5, 3, 4);
}

template<vcl::FaceMeshConcept MeshType>
void populatePolyMesh(MeshType& pm)
{
    using Point = MeshType::VertexType::CoordType;

    // note: p3 and p4 have same coords
    const Point p0(0, 0, 0);
    const Point p1(1, 0, 0);
    const Point p2(0, 1, 0);
    const Point p3(1, 1, 0);
    const Point p4(1, 1, 0);

    pm.addVertices(p0, p1, p2, p3, p4);

    pm.addFace(0, 1, 2);
    pm.addFace(0, 1, 2, 3);
    pm.addFace(0, 1, 2, 4); // not dup of 1 (different coordinates)
    pm.addFace(0, 2, 1, 3); // dup of 1
    pm.addFace(4, 1, 2, 0); // dup of 2
    pm.addFace(0, 2, 1);    // dup of 0
}

template<vcl::EdgeMeshConcept MeshType>
void populateEdgeMesh(MeshType& m)
{
    using Point = MeshType::VertexType::CoordType;

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

using Meshes  = std::tuple<vcl::TriMesh, vcl::PolyMesh, vcl::EdgeMesh>;
using Meshesf = std::tuple<vcl::TriMeshf, vcl::PolyMeshf, vcl::EdgeMeshf>;
using MeshesIndexed =
    std::tuple<vcl::TriMeshIndexed, vcl::PolyMeshIndexed, vcl::EdgeMeshIndexed>;
using MeshesIndexedf = std::
    tuple<vcl::TriMeshIndexedf, vcl::PolyMeshIndexedf, vcl::EdgeMeshIndexedf>;

TEMPLATE_TEST_CASE(
    "Clean Duplicated Faces",
    "",
    Meshes,
    Meshesf,
    MeshesIndexed,
    MeshesIndexedf)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using PolyMesh = std::tuple_element_t<1, TestType>;

    SECTION("TriMesh")
    {
        TriMesh tm;

        populateTriMesh(tm);

        REQUIRE(tm.vertexNumber() == 7);
        REQUIRE(tm.faceNumber() == 9);

        unsigned int nr = vcl::removeDuplicatedFaces(tm);
        REQUIRE(nr == 5);
        REQUIRE(tm.vertexNumber() == 7);
        REQUIRE(tm.faceNumber() == 4);
    }

    SECTION("PolyMesh with triangles")
    {
        PolyMesh pm;

        populateTriMesh(pm);

        REQUIRE(pm.vertexNumber() == 7);
        REQUIRE(pm.faceNumber() == 9);

        unsigned int nr = vcl::removeDuplicatedFaces(pm);
        REQUIRE(nr == 5);
        REQUIRE(pm.vertexNumber() == 7);
        REQUIRE(pm.faceNumber() == 4);
    }

    SECTION("PolyMesh with polygons")
    {
        PolyMesh pm;

        populatePolyMesh(pm);

        REQUIRE(pm.vertexNumber() == 5);
        REQUIRE(pm.faceNumber() == 6);

        unsigned int nr = vcl::removeDuplicatedFaces(pm);

        REQUIRE(nr == 3);
        REQUIRE(pm.vertexNumber() == 5);
        REQUIRE(pm.faceNumber() == 3);
    }
}

TEMPLATE_TEST_CASE(
    "WaterTightness",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh = TestType;

    SECTION("A TriMesh that is not watertight")
    {
        TriMesh t = vcl::load<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/brain.ply");

        REQUIRE(t.vertexNumber() == 18844);
        REQUIRE(t.faceNumber() == 36752);

        REQUIRE(!vcl::isWaterTight(t));
    }

    SECTION("A TriMesh that is watertight")
    {
        vcl::TriMesh t =
            vcl::load<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bone.ply");

        REQUIRE(t.vertexNumber() == 1872);
        REQUIRE(t.faceNumber() == 3022);

        REQUIRE(vcl::isWaterTight(t));
    }
}

TEMPLATE_TEST_CASE(
    "Duplicated Vertices",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh = TestType;

    TriMesh tm;

    populateTriMesh(tm);

    REQUIRE(tm.face(5).vertex(1) == &tm.vertex(4));

    unsigned int nv = vcl::removeDuplicatedVertices(tm);

    REQUIRE(nv == 1);

    REQUIRE(tm.face(5).vertex(1) == &tm.vertex(3));
}

TEMPLATE_TEST_CASE("Unreferenced Vertices", "", Meshes, Meshesf)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using EdgeMesh = std::tuple_element_t<2, TestType>;

    SECTION("TriMesh")
    {
        TriMesh tm;

        populateTriMesh(tm);

        unsigned int nv = vcl::numberUnreferencedVertices(tm);

        REQUIRE(nv == 1);
    }

    SECTION("EdgeMesh")
    {
        EdgeMesh em;

        populateEdgeMesh(em);

        unsigned int nv = vcl::numberUnreferencedVertices(em);

        REQUIRE(nv == 1);
    }
}

TEMPLATE_TEST_CASE(
    "Duplicated Vertices brain.ply",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh = TestType;

    TriMesh t = vcl::load<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/brain.ply");

    unsigned int nv = vcl::removeDuplicatedVertices(t);

    SECTION("Test number duplicated vertices")
    {
        REQUIRE(nv == 453);
        REQUIRE(t.vertexNumber() == 18844 - nv);
        REQUIRE(t.vertexContainerSize() == 18844);
        REQUIRE(t.faceNumber() == 36752);
    }

    SECTION("Test compactness")
    {
        REQUIRE(t.vertexNumber() != t.vertexContainerSize());

        t.compact();

        REQUIRE(t.vertexNumber() == t.vertexContainerSize());
        REQUIRE(t.vertexNumber() == 18844 - nv);
    }
}
