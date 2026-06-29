// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/algorithms.h>
#include <vclib/meshes.h>
#include <vclib/vcg/export.h>
#include <vclib/vcg/import.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <wrap/io_trimesh/import_ply.h>

#include "mesh.h"

TEMPLATE_TEST_CASE(
    "Import TriMesh from VCG",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh = TestType;

    VCGMesh vcgMesh;

    vcg::tri::io::ImporterPLY<VCGMesh>::Open(
        vcgMesh, VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    REQUIRE(vcgMesh.VN() == 8);
    REQUIRE(vcgMesh.FN() == 12);

    SECTION("Test Vertices and Faces")
    {
        TriMesh tm = vcl::vc::meshFromVCGMesh<TriMesh>(vcgMesh);

        REQUIRE(tm.vertexCount() == 8);
        REQUIRE(tm.faceCount() == 12);

        for (unsigned int fi = 0; fi < tm.faceCount(); ++fi) {
            const auto& f    = tm.face(fi);
            const auto& vcgf = vcgMesh.face[fi];
            for (unsigned int vi = 0; vi < 3; ++vi) {
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

        for (unsigned int vi = 0; vi < vcgMesh.VN(); ++vi) {
            h[vcgMesh.vert[vi]] = (float) vi / vcgMesh.VN();
        }

        TriMesh tm = vcl::vc::meshFromVCGMesh<TriMesh>(vcgMesh);

        REQUIRE(tm.hasPerVertexCustomComponent("perVertex"));
        REQUIRE(
            tm.template isPerVertexCustomComponentOfType<float>("perVertex"));

        for (const auto& v : tm.vertices()) {
            REQUIRE(
                v.template customComponent<float>("perVertex") ==
                (float) v.index() / tm.vertexCount());
        }
    }

    SECTION("Test Per Vertex point Custom Components")
    {
        auto h =
            vcg::tri::Allocator<VCGMesh>::AddPerVertexAttribute<vcg::Point3f>(
                vcgMesh, "perVertex");

        for (unsigned int vi = 0; vi < vcgMesh.VN(); ++vi) {
            h[vcgMesh.vert[vi]] = vcgMesh.vert[vi].P();
        }

        TriMesh tm = vcl::vc::meshFromVCGMesh<TriMesh>(vcgMesh);

        REQUIRE(tm.hasPerVertexCustomComponent("perVertex"));
        REQUIRE(tm.template isPerVertexCustomComponentOfType<vcl::Point3f>(
            "perVertex"));

        for (const auto& v : tm.vertices()) {
            REQUIRE(
                v.template customComponent<vcl::Point3f>("perVertex") ==
                v.position().template cast<float>());
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

        for (unsigned int fi = 0; fi < vcgMesh.FN(); ++fi) {
            h[vcgMesh.face[fi]] = (double) fi / vcgMesh.FN();
        }

        TriMesh tm = vcl::vc::meshFromVCGMesh<TriMesh>(vcgMesh);

        REQUIRE(tm.hasPerFaceCustomComponent("perFace"));
        REQUIRE(tm.template isPerFaceCustomComponentOfType<double>("perFace"));

        for (const auto& f : tm.faces()) {
            REQUIRE(
                f.template customComponent<double>("perFace") ==
                (double) f.index() / tm.faceCount());
        }
    }

    SECTION("Test Per Face point Custom Components")
    {
        auto h =
            vcg::tri::Allocator<VCGMesh>::AddPerFaceAttribute<vcg::Point3f>(
                vcgMesh, "perFace");

        for (unsigned int fi = 0; fi < vcgMesh.FN(); ++fi) {
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
    vcl::PolyMeshf,
    vcl::PolyMeshIndexed,
    vcl::PolyMeshIndexedf)
{
    using PolyMesh = TestType;

    VCGMesh vcgMesh;

    vcg::tri::io::ImporterPLY<VCGMesh>::Open(
        vcgMesh, VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    REQUIRE(vcgMesh.VN() == 8);
    REQUIRE(vcgMesh.FN() == 12);

    PolyMesh pm = vcl::vc::meshFromVCGMesh<PolyMesh>(vcgMesh);

    REQUIRE(pm.vertexCount() == 8);
    REQUIRE(pm.faceCount() == 12);

    for (unsigned int fi = 0; fi < pm.faceCount(); ++fi) {
        const auto& f    = pm.face(fi);
        const auto& vcgf = vcgMesh.face[fi];

        REQUIRE(f.vertexCount() == 3);
        for (unsigned int vi = 0; vi < 3; ++vi) {
            REQUIRE(
                pm.index(f.vertex(vi)) == vcg::tri::Index(vcgMesh, vcgf.V(vi)));
        }
    }
}

TEMPLATE_TEST_CASE(
    "Export TriMesh to VCG",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh = TestType;

    TriMesh tm = vcl::createCube<TriMesh>();

    SECTION("Test Vertices and Faces")
    {
        VCGMesh vcgMesh;
        vcl::vc::exportMeshToVCGMesh(tm, vcgMesh);

        REQUIRE(vcgMesh.VN() == tm.vertexCount());
        REQUIRE(vcgMesh.FN() == tm.faceCount());

        for (unsigned int fi = 0; fi < tm.faceCount(); ++fi) {
            const auto& f    = tm.face(fi);
            const auto& vcgf = vcgMesh.face[fi];
            for (unsigned int vi = 0; vi < 3; ++vi) {
                REQUIRE(
                    tm.index(f.vertex(vi)) ==
                    vcg::tri::Index(vcgMesh, vcgf.V(vi)));
            }
        }
    }
}
