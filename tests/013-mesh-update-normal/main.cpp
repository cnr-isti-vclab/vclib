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

#include <catch2/catch_test_macros.hpp>
#include <vclib/algorithms.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>

TEST_CASE("TriMesh Update Normal")
{
    vcl::TriMesh tm =
        vcl::loadPly<vcl::TriMesh>(VCLIB_ASSETS_PATH "/cube_tri.ply");

    THEN("The mesh normals are not updated - all zero")
    {
        for (const auto& v : tm.vertices()) {
            REQUIRE(v.normal() == vcl::Point3d(0, 0, 0));
        }

        for (const auto& f : tm.faces()) {
            REQUIRE(f.normal() == vcl::Point3d(0, 0, 0));
        }
    }

    THEN("Update Face Normals")
    {
        vcl::updatePerFaceNormals(tm);

        REQUIRE(tm.face(0).normal() == vcl::Point3d(0, 0, -1));
        REQUIRE(tm.face(1).normal() == vcl::Point3d(0, 0, -1));
        REQUIRE(tm.face(2).normal() == vcl::Point3d(-1, 0, 0));
        REQUIRE(tm.face(3).normal() == vcl::Point3d(-1, 0, 0));
        REQUIRE(tm.face(4).normal() == vcl::Point3d(0, -1, 0));
        REQUIRE(tm.face(5).normal() == vcl::Point3d(0, -1, 0));
        REQUIRE(tm.face(6).normal() == vcl::Point3d(0, 0, 1));
        REQUIRE(tm.face(7).normal() == vcl::Point3d(0, 0, 1));
        REQUIRE(tm.face(8).normal() == vcl::Point3d(0, 1, 0));
        REQUIRE(tm.face(9).normal() == vcl::Point3d(0, 1, 0));
        REQUIRE(tm.face(10).normal() == vcl::Point3d(1, 0, 0));
        REQUIRE(tm.face(11).normal() == vcl::Point3d(1, 0, 0));

        for (const auto& v : tm.vertices()) {
            REQUIRE(v.normal() == vcl::Point3d(0, 0, 0));
        }
    }

    THEN("Update Vertex Normals - Area Weighted")
    {
        vcl::updatePerVertexNormals(tm);

        const double a = 1 / std::sqrt(3.0);
        const double b = 1 / 3.0;
        const double c = 2 / 3.0;

        REQUIRE(tm.vertex(0).normal() == vcl::Point3d(-a, -a, -a));
        REQUIRE(tm.vertex(1).normal() == vcl::Point3d(b, -c, -c));
        REQUIRE(tm.vertex(2).normal() == vcl::Point3d(-c, b, -c));
        REQUIRE(tm.vertex(3).normal() == vcl::Point3d(c, c, -b));
        REQUIRE(tm.vertex(4).normal() == vcl::Point3d(-c, -c, b));
        REQUIRE(tm.vertex(5).normal() == vcl::Point3d(c, -b, c));
        REQUIRE(tm.vertex(6).normal() == vcl::Point3d(-b, c, c));
        REQUIRE(tm.vertex(7).normal() == vcl::Point3d(a, a, a));

        for (const auto& f : tm.faces()) {
            REQUIRE(f.normal() == vcl::Point3d(0, 0, 0));
        }
    }

    THEN("Update Vertex Normals - Angle Weighted")
    {
        vcl::updatePerVertexNormalsAngleWeighted(tm);

        const double a = 1 / std::sqrt(3.0);

        REQUIRE(tm.vertex(0).normal() == vcl::Point3d(-a, -a, -a));
        REQUIRE(tm.vertex(1).normal() == vcl::Point3d(a, -a, -a));
        REQUIRE(tm.vertex(2).normal() == vcl::Point3d(-a, a, -a));
        REQUIRE(tm.vertex(3).normal() == vcl::Point3d(a, a, -a));
        REQUIRE(tm.vertex(4).normal() == vcl::Point3d(-a, -a, a));
        REQUIRE(tm.vertex(5).normal() == vcl::Point3d(a, -a, a));
        REQUIRE(tm.vertex(6).normal() == vcl::Point3d(-a, a, a));
        REQUIRE(tm.vertex(7).normal() == vcl::Point3d(a, a, a));

        for (const auto& f : tm.faces()) {
            REQUIRE(f.normal() == vcl::Point3d(0, 0, 0));
        }
    }
}

TEST_CASE("Update Referenced Vertex Normals")
{
    vcl::TriEdgeMesh tem;

    tem.addVertex(vcl::Point3d(0, 0, 0));
    tem.addVertex(vcl::Point3d(1, 0, 0));
    tem.addVertex(vcl::Point3d(0, 1, 0));
    tem.addVertex(vcl::Point3d(1, 1, 0));
    tem.addVertex(vcl::Point3d(0, 0, 1)); // only referenced by edge

    tem.addVertex(vcl::Point3d(2, 2, 2)); // unreferenced
    tem.addVertex(vcl::Point3d(3, 2, 2)); // unreferenced

    tem.addFace(0, 1, 2);
    tem.addFace(1, 3, 2);

    tem.addEdge(0, 4);

    for(auto& v : tem.vertices()) {
        v.normal() = vcl::Point3d(1, 1, 1);
    }

    THEN("Clear Referenced Vertex Normals")
    {
        vcl::clearPerReferencedVertexNormals(tem);

        REQUIRE(tem.vertex(0).normal() == vcl::Point3d(0, 0, 0));
        REQUIRE(tem.vertex(1).normal() == vcl::Point3d(0, 0, 0));
        REQUIRE(tem.vertex(2).normal() == vcl::Point3d(0, 0, 0));
        REQUIRE(tem.vertex(3).normal() == vcl::Point3d(0, 0, 0));
        REQUIRE(tem.vertex(4).normal() == vcl::Point3d(0, 0, 0));
        REQUIRE(tem.vertex(5).normal() == vcl::Point3d(1, 1, 1));
        REQUIRE(tem.vertex(6).normal() == vcl::Point3d(1, 1, 1));
    }

    THEN("Normalize Referenced Vertex Normals")
    {
        vcl::normalizePerReferencedVertexNormals(tem);

        const double a = 1 / std::sqrt(3.0);

        REQUIRE(tem.vertex(0).normal() == vcl::Point3d(a, a, a));
        REQUIRE(tem.vertex(1).normal() == vcl::Point3d(a, a, a));
        REQUIRE(tem.vertex(2).normal() == vcl::Point3d(a, a, a));
        REQUIRE(tem.vertex(3).normal() == vcl::Point3d(a, a, a));
        REQUIRE(tem.vertex(4).normal() == vcl::Point3d(a, a, a));
        REQUIRE(tem.vertex(5).normal() == vcl::Point3d(1, 1, 1));
        REQUIRE(tem.vertex(6).normal() == vcl::Point3d(1, 1, 1));
    }
}
