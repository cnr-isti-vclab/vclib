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

#include <vclib/algorithms/core/polygon.h>
#include <vclib/algorithms/mesh/create.h>
#include <vclib/ext/vcg/export.h>
#include <vclib/ext/vcg/import.h>
#include <vclib/meshes.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <wrap/io_trimesh/import_ply.h>

#include "mesh.h"

TEMPLATE_TEST_CASE("Import TriMesh from VCG", "", vcl::TriMesh, vcl::TriMeshf)
{
    using TriMesh = TestType;

    VCGMesh vcgMesh;

    vcg::tri::io::ImporterPLY<VCGMesh>::Open(
        vcgMesh, VCLIB_ASSETS_PATH "/cube_tri.ply");

    REQUIRE(vcgMesh.VN() == 8);
    REQUIRE(vcgMesh.FN() == 12);

    SECTION("Test Vertices and Faces")
    {
        TriMesh tm = vcl::vc::meshFromVCGMesh<TriMesh>(vcgMesh);

        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 12);

        for (uint fi = 0; fi < tm.faceNumber(); ++fi) {
            const auto& f    = tm.face(fi);
            const auto& vcgf = vcgMesh.face[fi];
            for (uint vi = 0; vi < 3; ++vi) {
                REQUIRE(
                    tm.index(f.vertex(vi)) ==
                    vcg::tri::Index(vcgMesh, vcgf.V(vi)));
            }
        }
    }

    SECTION("Test Per Vertex Normals")
    {
        vcg::tri::UpdateNormal<VCGMesh>::PerVertexNormalizedPerFaceNormalized(
            vcgMesh);

        TriMesh tm = vcl::vc::meshFromVCGMesh<TriMesh>(vcgMesh);

        for (const auto& v : tm.vertices()) {
            REQUIRE(v.normal().x() == vcgMesh.vert[v.index()].N().X());
            REQUIRE(v.normal().y() == vcgMesh.vert[v.index()].N().Y());
            REQUIRE(v.normal().z() == vcgMesh.vert[v.index()].N().Z());
        }
    }

    SECTION("Test Per Vertex scalar Custom Components")
    {
        auto h = vcg::tri::Allocator<VCGMesh>::AddPerVertexAttribute<float>(
            vcgMesh, "perVertex");

        for (uint vi = 0; vi < vcgMesh.VN(); ++vi) {
            h[vcgMesh.vert[vi]] = (float) vi / vcgMesh.VN();
        }

        TriMesh tm = vcl::vc::meshFromVCGMesh<TriMesh>(vcgMesh);

        REQUIRE(tm.hasPerVertexCustomComponent("perVertex"));
        REQUIRE(tm.template isPerVertexCustomComponentOfType<float>("perVertex"));

        for (const auto& v : tm.vertices()) {
            REQUIRE(
                v.template customComponent<float>("perVertex") ==
                (float) v.index() / tm.vertexNumber());
        }
    }

    SECTION("Test Per Vertex point Custom Components")
    {
        auto h =
            vcg::tri::Allocator<VCGMesh>::AddPerVertexAttribute<vcg::Point3f>(
                vcgMesh, "perVertex");

        for (uint vi = 0; vi < vcgMesh.VN(); ++vi) {
            h[vcgMesh.vert[vi]] = vcgMesh.vert[vi].P();
        }

        TriMesh tm = vcl::vc::meshFromVCGMesh<TriMesh>(vcgMesh);

        REQUIRE(tm.hasPerVertexCustomComponent("perVertex"));
        REQUIRE(tm.template isPerVertexCustomComponentOfType<vcl::Point3f>(
            "perVertex"));

        for (const auto& v : tm.vertices()) {
            REQUIRE(
                v.template customComponent<vcl::Point3f>("perVertex") ==
                v.coord().template cast<float>());
        }
    }

    SECTION("Test Per Face Normals")
    {
        vcg::tri::UpdateNormal<VCGMesh>::PerVertexNormalizedPerFaceNormalized(
            vcgMesh);

        TriMesh tm = vcl::vc::meshFromVCGMesh<TriMesh>(vcgMesh);

        for (const auto& f : tm.faces()) {
            REQUIRE(f.normal().x() == vcgMesh.face[f.index()].N().X());
            REQUIRE(f.normal().y() == vcgMesh.face[f.index()].N().Y());
            REQUIRE(f.normal().z() == vcgMesh.face[f.index()].N().Z());
        }
    }

    SECTION("Test Per Face scalar Custom Components")
    {
        auto h = vcg::tri::Allocator<VCGMesh>::AddPerFaceAttribute<double>(
            vcgMesh, "perFace");

        for (uint fi = 0; fi < vcgMesh.FN(); ++fi) {
            h[vcgMesh.face[fi]] = (double) fi / vcgMesh.FN();
        }

        TriMesh tm = vcl::vc::meshFromVCGMesh<TriMesh>(vcgMesh);

        REQUIRE(tm.hasPerFaceCustomComponent("perFace"));
        REQUIRE(tm.template isPerFaceCustomComponentOfType<double>("perFace"));

        for (const auto& f : tm.faces()) {
            REQUIRE(
                f.template customComponent<double>("perFace") ==
                (double) f.index() / tm.faceNumber());
        }
    }

    SECTION("Test Per Face point Custom Components")
    {
        auto h =
            vcg::tri::Allocator<VCGMesh>::AddPerFaceAttribute<vcg::Point3f>(
                vcgMesh, "perFace");

        for (uint fi = 0; fi < vcgMesh.FN(); ++fi) {
            // assign barycener of face fi
            h[vcgMesh.face[fi]] = vcg::Barycenter(vcgMesh.face[fi]);
        }

        TriMesh tm = vcl::vc::meshFromVCGMesh<TriMesh>(vcgMesh);

        REQUIRE(tm.hasPerFaceCustomComponent("perFace"));
        REQUIRE(tm.template isPerFaceCustomComponentOfType<vcl::Point3f>(
            "perFace"));

        for (const auto& f : tm.faces()) {
            REQUIRE(
                f.template customComponent<vcl::Point3f>("perFace") ==
                vcl::faceBarycenter(f).template cast<float>());
        }
    }

    SECTION("Test Per Mesh point Custom Component")
    {
        auto h =
            vcg::tri::Allocator<VCGMesh>::AddPerMeshAttribute<vcg::Point3f>(
                vcgMesh, "perMesh");

        h() = vcg::Point3f(1, 2, 3);

        TriMesh tm = vcl::vc::meshFromVCGMesh<TriMesh>(vcgMesh);

        REQUIRE(tm.hasCustomComponent("perMesh"));
        REQUIRE(tm.template isCustomComponentOfType<vcl::Point3f>("perMesh"));
        REQUIRE(
            tm.template customComponent<vcl::Point3f>("perMesh") ==
            vcl::Point3f(1, 2, 3));
    }
}

TEMPLATE_TEST_CASE(
    "Import PolyMesh from VCG",
    "",
    vcl::PolyMesh,
    vcl::PolyMeshf)
{
    using PolyMesh = TestType;

    VCGMesh vcgMesh;

    vcg::tri::io::ImporterPLY<VCGMesh>::Open(
        vcgMesh, VCLIB_ASSETS_PATH "/cube_tri.ply");

    REQUIRE(vcgMesh.VN() == 8);
    REQUIRE(vcgMesh.FN() == 12);

    PolyMesh pm = vcl::vc::meshFromVCGMesh<PolyMesh>(vcgMesh);

    REQUIRE(pm.vertexNumber() == 8);
    REQUIRE(pm.faceNumber() == 12);

    for (uint fi = 0; fi < pm.faceNumber(); ++fi) {
        const auto& f    = pm.face(fi);
        const auto& vcgf = vcgMesh.face[fi];

        REQUIRE(f.vertexNumber() == 3);
        for (uint vi = 0; vi < 3; ++vi) {
            REQUIRE(
                pm.index(f.vertex(vi)) == vcg::tri::Index(vcgMesh, vcgf.V(vi)));
        }
    }
}

TEMPLATE_TEST_CASE("Export TriMesh to VCG", "", vcl::TriMesh, vcl::TriMeshf)
{
    using TriMesh = TestType;

    TriMesh tm = vcl::createCube<TriMesh>();

    SECTION("Test Vertices and Faces")
    {
        VCGMesh vcgMesh;
        vcl::vc::exportMeshToVCGMesh(tm, vcgMesh);

        REQUIRE(vcgMesh.VN() == tm.vertexNumber());
        REQUIRE(vcgMesh.FN() == tm.faceNumber());

        for (uint fi = 0; fi < tm.faceNumber(); ++fi) {
            const auto& f    = tm.face(fi);
            const auto& vcgf = vcgMesh.face[fi];
            for (uint vi = 0; vi < 3; ++vi) {
                REQUIRE(
                    tm.index(f.vertex(vi)) ==
                    vcg::tri::Index(vcgMesh, vcgf.V(vi)));
            }
        }
    }
}
