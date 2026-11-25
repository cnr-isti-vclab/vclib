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

#include <cmath>

TEST_CASE("Update Normals")
{
    TangentMesh m;
    m.addVertices(3);
    m.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    m.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    m.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);

    m.addFace(0, 1, 2);

    // 1. Update per-face normals
    vcl::updateFaceNormals(m);
    Point3f expectedNormal(0.0f, 0.0f, 1.0f);
    REQUIRE((m.face(0).normal() - expectedNormal).norm() < 1e-5f);

    // 2. Clear per-face normals
    vcl::clearFaceNormals(m);
    REQUIRE(m.face(0).normal() == Point3f(0.0f, 0.0f, 0.0f));

    // 3. Update per-vertex normals
    vcl::updateVertexNormals(m);
    for (uint i = 0; i < 3; ++i) {
        REQUIRE((m.vertex(i).normal() - expectedNormal).norm() < 1e-5f);
    }

    // 4. Clear per-vertex normals
    vcl::clearVertexNormals(m);
    for (uint i = 0; i < 3; ++i) {
        REQUIRE(m.vertex(i).normal() == Point3f(0.0f, 0.0f, 0.0f));
    }

    // 5. Update per-vertex normals from face normals
    vcl::updateFaceNormals(m);
    vcl::updateVertexNormalsFromFaceNormals(m);
    for (uint i = 0; i < 3; ++i) {
        REQUIRE((m.vertex(i).normal() - expectedNormal).norm() < 1e-5f);
    }

    // 6. Update per-vertex normals angle weighted
    vcl::clearVertexNormals(m);
    vcl::updateVertexNormalsAngleWeighted(m);
    for (uint i = 0; i < 3; ++i) {
        REQUIRE((m.vertex(i).normal() - expectedNormal).norm() < 1e-5f);
    }

    // 7. Update per-vertex normals Nelson-Max weighted
    vcl::clearVertexNormals(m);
    vcl::updateVertexNormalsNelsonMaxWeighted(m);
    for (uint i = 0; i < 3; ++i) {
        REQUIRE((m.vertex(i).normal() - expectedNormal).norm() < 1e-5f);
    }

    // 8. Update per-vertex and face normals together
    vcl::clearVertexNormals(m);
    vcl::clearFaceNormals(m);
    vcl::updateVertexAndFaceNormals(m);
    REQUIRE((m.face(0).normal() - expectedNormal).norm() < 1e-5f);
    for (uint i = 0; i < 3; ++i) {
        REQUIRE((m.vertex(i).normal() - expectedNormal).norm() < 1e-5f);
    }

    // 9. Multiply normals by matrix (rotate 90 deg around X axis: (0,0,1) ->
    // (0,-1,0))
    Matrix33f rotX;
    rotX << 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f;
    vcl::multiplyFaceNormalsByMatrix(m, rotX);
    Point3f expectedRotated(0.0f, -1.0f, 0.0f);
    REQUIRE((m.face(0).normal() - expectedRotated).norm() < 1e-5f);

    vcl::multiplyVertexNormalsByMatrix(m, rotX);
    for (uint i = 0; i < 3; ++i) {
        REQUIRE((m.vertex(i).normal() - expectedRotated).norm() < 1e-5f);
    }
}

TEST_CASE("Transform - rotate mixed scalar types")
{
    // TriMeshf uses float, rotate with Matrix33d (double)
    vcl::TriMeshf m = vcl::createCube<vcl::TriMeshf>();
    vcl::updateVertexAndFaceNormals(m);

    Point3f origV0 = m.vertex(0).position();
    Point3f origN0 = m.vertex(0).normal();

    // Rotate 90 degrees around Z axis: (x, y, z) -> (-y, x, z)
    Matrix33d rotZ;
    rotZ << 0.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0;

    vcl::rotate(m, rotZ);

    Point3f expectedV0(-origV0.y(), origV0.x(), origV0.z());
    Point3f expectedN0(-origN0.y(), origN0.x(), origN0.z());
    REQUIRE((m.vertex(0).position() - expectedV0).norm() < 1e-5f);
    REQUIRE((m.vertex(0).normal() - expectedN0).norm() < 1e-5f);
}

TEST_CASE("Transform - EdgeNormals")
{
    vcl::TriEdgeMesh m;
    m.enableElementComponent<ElemId::EDGE, CompId::NORMAL>();
    m.addVertices(3);
    m.vertex(0).position() = Point3d(0.0, 0.0, 0.0);
    m.vertex(1).position() = Point3d(1.0, 0.0, 0.0);
    m.vertex(2).position() = Point3d(0.0, 1.0, 0.0);
    m.addFace(0, 1, 2);
    m.addEdge(0, 1);
    m.edge(0).normal() = Point3d(0.0, 0.0, 1.0);

    // Rotate 90 deg around X: (0, 0, 1) -> (0, -1, 0)
    Matrix33d rotX;
    rotX << 1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0;
    vcl::rotate(m, rotX);

    REQUIRE((m.edge(0).normal() - Point3d(0.0, -1.0, 0.0)).norm() < 1e-5);

    // Transform with Matrix44d
    Matrix44d t44         = Matrix44d::Identity();
    t44.block<3, 3>(0, 0) = rotX;
    vcl::applyTransformMatrix(m, t44);
    // (0, -1, 0) rotated by 90 deg around X -> (0, 0, -1)
    REQUIRE((m.edge(0).normal() - Point3d(0.0, 0.0, -1.0)).norm() < 1e-5);
}

TEST_CASE("Transform - WedgeNormals")
{
    using WedgeMeshType = Mesh<
        mesh::VertexContainer<
            VertexComponents<vert::BitFlags, vert::Position3d>>,
        mesh::FaceContainer<FaceComponents<
            face::BitFlags,
            face::TriangleVertexReferences<vcl::uint>,
            face::TriangleWedgeNormals3d>>>;

    WedgeMeshType m;
    m.addVertices(3);
    m.vertex(0).position() = Point3d(0.0, 0.0, 0.0);
    m.vertex(1).position() = Point3d(1.0, 0.0, 0.0);
    m.vertex(2).position() = Point3d(0.0, 1.0, 0.0);
    m.addFace(0, 1, 2);
    for (auto w : m.face(0).wedges()) {
        w.normal() = Point3d(0.0, 0.0, 1.0);
    }

    Matrix33d rotX;
    rotX << 1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0;
    vcl::rotate(m, rotX);

    for (auto w : m.face(0).wedges()) {
        REQUIRE((w.normal() - Point3d(0.0, -1.0, 0.0)).norm() < 1e-5);
    }
}

TEST_CASE("Transform - Tangent and PrincipalCurvature")
{
    using FullGeomMesh = Mesh<
        mesh::VertexContainer<VertexComponents<
            vert::BitFlags,
            vert::Position3d,
            vert::Normal3d,
            vert::Tangent3d,
            vert::PrincipalCurvatured>>,
        mesh::FaceContainer<FaceComponents<
            face::BitFlags,
            face::TriangleVertexReferences<vcl::uint>,
            face::Normal3d>>>;

    FullGeomMesh m;
    m.addVertices(3);
    m.vertex(0).position() = Point3d(0.0, 0.0, 0.0);
    m.vertex(1).position() = Point3d(1.0, 0.0, 0.0);
    m.vertex(2).position() = Point3d(0.0, 1.0, 0.0);
    m.addFace(0, 1, 2);

    m.vertex(0).normal()             = Point3d(0.0, 0.0, 1.0);
    m.vertex(0).tangent()            = Point3d(1.0, 0.0, 0.0);
    m.vertex(0).tangentRightHanded() = true;

    m.vertex(0).principalCurvature().maxDir()   = Point3d(1.0, 0.0, 0.0);
    m.vertex(0).principalCurvature().minDir()   = Point3d(0.0, 1.0, 0.0);
    m.vertex(0).principalCurvature().maxValue() = 2.0;
    m.vertex(0).principalCurvature().minValue() = 0.5;

    // A. Rotate 90 deg around Z: (x, y, z) -> (-y, x, z)
    Matrix33d rotZ;
    rotZ << 0.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0;

    vcl::rotate(m, rotZ);

    REQUIRE((m.vertex(0).tangent() - Point3d(0.0, 1.0, 0.0)).norm() < 1e-5);
    REQUIRE(m.vertex(0).tangentRightHanded() == true);
    REQUIRE(
        (m.vertex(0).principalCurvature().maxDir() - Point3d(0.0, 1.0, 0.0))
            .norm() < 1e-5);
    REQUIRE(
        (m.vertex(0).principalCurvature().minDir() - Point3d(-1.0, 0.0, 0.0))
            .norm() < 1e-5);
    REQUIRE(m.vertex(0).principalCurvature().maxValue() == 2.0);
    REQUIRE(m.vertex(0).principalCurvature().minValue() == 0.5);

    // B. Reflection (det < 0): scale X by -1
    Matrix44d refX = Matrix44d::Identity();
    refX(0, 0)     = -1.0;
    vcl::applyTransformMatrix(m, refX);

    REQUIRE(m.vertex(0).tangentRightHanded() == false); // flipped!
}
