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

TEMPLATE_TEST_CASE(
    "TriMesh Update Normal",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh     = TestType;
    using VNormalType = TriMesh::VertexType::NormalType;
    using FNormalType = TriMesh::FaceType::NormalType;
    using VNScalar    = typename VNormalType::ScalarType;

    TriMesh tm =
        vcl::loadPly<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    THEN("The mesh normals are not updated - all zero")
    {
        for (const auto& v : tm.vertices()) {
            REQUIRE(v.normal() == VNormalType(0, 0, 0));
        }

        for (const auto& f : tm.faces()) {
            REQUIRE(f.normal() == FNormalType(0, 0, 0));
        }
    }

    THEN("Update Face Normals")
    {
        vcl::updatePerFaceNormals(tm);

        REQUIRE(tm.face(0).normal() == FNormalType(0, 0, -1));
        REQUIRE(tm.face(1).normal() == FNormalType(0, 0, -1));
        REQUIRE(tm.face(2).normal() == FNormalType(-1, 0, 0));
        REQUIRE(tm.face(3).normal() == FNormalType(-1, 0, 0));
        REQUIRE(tm.face(4).normal() == FNormalType(0, -1, 0));
        REQUIRE(tm.face(5).normal() == FNormalType(0, -1, 0));
        REQUIRE(tm.face(6).normal() == FNormalType(0, 0, 1));
        REQUIRE(tm.face(7).normal() == FNormalType(0, 0, 1));
        REQUIRE(tm.face(8).normal() == FNormalType(0, 1, 0));
        REQUIRE(tm.face(9).normal() == FNormalType(0, 1, 0));
        REQUIRE(tm.face(10).normal() == FNormalType(1, 0, 0));
        REQUIRE(tm.face(11).normal() == FNormalType(1, 0, 0));

        for (const auto& v : tm.vertices()) {
            REQUIRE(v.normal() == VNormalType(0, 0, 0));
        }
    }

    THEN("Update Vertex Normals - Area Weighted")
    {
        vcl::updatePerVertexNormals(tm);

        const VNScalar a = 1 / std::sqrt(3.0);
        const VNScalar b = 1 / 3.0;
        const VNScalar c = 2 / 3.0;

        REQUIRE(tm.vertex(0).normal() == VNormalType(-a, -a, -a));
        REQUIRE(tm.vertex(1).normal() == VNormalType(b, -c, -c));
        REQUIRE(tm.vertex(2).normal() == VNormalType(-c, b, -c));
        REQUIRE(tm.vertex(3).normal() == VNormalType(c, c, -b));
        REQUIRE(tm.vertex(4).normal() == VNormalType(-c, -c, b));
        REQUIRE(tm.vertex(5).normal() == VNormalType(c, -b, c));
        REQUIRE(tm.vertex(6).normal() == VNormalType(-b, c, c));
        REQUIRE(tm.vertex(7).normal() == VNormalType(a, a, a));

        for (const auto& f : tm.faces()) {
            REQUIRE(f.normal() == FNormalType(0, 0, 0));
        }
    }

    THEN("Update Vertex Normals - Angle Weighted")
    {
        vcl::updatePerVertexNormalsAngleWeighted(tm);

        const VNScalar a = 1 / std::sqrt(3.0);

        REQUIRE(
            vcl::epsilonEquals(tm.vertex(0).normal(), VNormalType(-a, -a, -a)));
        REQUIRE(
            vcl::epsilonEquals(tm.vertex(1).normal(), VNormalType(a, -a, -a)));
        REQUIRE(
            vcl::epsilonEquals(tm.vertex(2).normal(), VNormalType(-a, a, -a)));
        REQUIRE(
            vcl::epsilonEquals(tm.vertex(3).normal(), VNormalType(a, a, -a)));
        REQUIRE(
            vcl::epsilonEquals(tm.vertex(4).normal(), VNormalType(-a, -a, a)));
        REQUIRE(
            vcl::epsilonEquals(tm.vertex(5).normal(), VNormalType(a, -a, a)));
        REQUIRE(
            vcl::epsilonEquals(tm.vertex(6).normal(), VNormalType(-a, a, a)));
        REQUIRE(
            vcl::epsilonEquals(tm.vertex(7).normal(), VNormalType(a, a, a)));

        for (const auto& f : tm.faces()) {
            REQUIRE(f.normal() == FNormalType(0, 0, 0));
        }
    }
}

TEMPLATE_TEST_CASE(
    "Update Referenced Vertex Normals",
    "",
    vcl::TriEdgeMesh,
    vcl::TriEdgeMeshf,
    vcl::TriEdgeMeshIndexed,
    vcl::TriEdgeMeshIndexedf)
{
    using TriEdgeMesh = TestType;
    using VNormalType = TriEdgeMesh::VertexType::NormalType;
    using VNScalar    = typename VNormalType::ScalarType;

    TriEdgeMesh tem;

    tem.addVertex(VNormalType(0, 0, 0));
    tem.addVertex(VNormalType(1, 0, 0));
    tem.addVertex(VNormalType(0, 1, 0));
    tem.addVertex(VNormalType(1, 1, 0));
    tem.addVertex(VNormalType(0, 0, 1)); // only referenced by edge

    tem.addVertex(VNormalType(2, 2, 2)); // unreferenced
    tem.addVertex(VNormalType(3, 2, 2)); // unreferenced

    tem.addFace(0, 1, 2);
    tem.addFace(1, 3, 2);

    tem.addEdge(0, 4);

    for (auto& v : tem.vertices()) {
        v.normal() = VNormalType(1, 1, 1);
    }

    THEN("Clear Referenced Vertex Normals")
    {
        vcl::clearPerReferencedVertexNormals(tem);

        REQUIRE(tem.vertex(0).normal() == VNormalType(0, 0, 0));
        REQUIRE(tem.vertex(1).normal() == VNormalType(0, 0, 0));
        REQUIRE(tem.vertex(2).normal() == VNormalType(0, 0, 0));
        REQUIRE(tem.vertex(3).normal() == VNormalType(0, 0, 0));
        REQUIRE(tem.vertex(4).normal() == VNormalType(0, 0, 0));
        REQUIRE(tem.vertex(5).normal() == VNormalType(1, 1, 1));
        REQUIRE(tem.vertex(6).normal() == VNormalType(1, 1, 1));
    }

    THEN("Normalize Referenced Vertex Normals")
    {
        vcl::normalizePerReferencedVertexNormals(tem);

        const VNScalar a = 1 / std::sqrt(3.0);

        REQUIRE(
            vcl::epsilonEquals(tem.vertex(0).normal(), VNormalType(a, a, a)));
        REQUIRE(
            vcl::epsilonEquals(tem.vertex(1).normal(), VNormalType(a, a, a)));
        REQUIRE(
            vcl::epsilonEquals(tem.vertex(2).normal(), VNormalType(a, a, a)));
        REQUIRE(
            vcl::epsilonEquals(tem.vertex(3).normal(), VNormalType(a, a, a)));
        REQUIRE(
            vcl::epsilonEquals(tem.vertex(4).normal(), VNormalType(a, a, a)));
        REQUIRE(
            vcl::epsilonEquals(tem.vertex(5).normal(), VNormalType(1, 1, 1)));
        REQUIRE(
            vcl::epsilonEquals(tem.vertex(6).normal(), VNormalType(1, 1, 1)));
    }
}
