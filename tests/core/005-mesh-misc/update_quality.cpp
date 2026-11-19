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

namespace {

template<typename Func>
void assertThrowsMissingQualityComponent(Func&& func)
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

TEST_CASE("Update Quality - Set Quality")
{
    // --- 1. TriMesh (double, optional vertex and face quality) ---
    {
        vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
        tet.enableVertexQuality();
        tet.enableFaceQuality();

        // setVertexQuality
        vcl::setVertexQuality(tet, 42.5);
        for (const auto v : tet.vertices()) {
            REQUIRE(v.quality() == 42.5);
        }

        // setFaceQuality
        vcl::setFaceQuality(tet, 17.25);
        for (const auto f : tet.faces()) {
            REQUIRE(f.quality() == 17.25);
        }

        // setElementQuality<ElemId::VERTEX>
        vcl::setElementQuality<vcl::ElemId::VERTEX>(tet, 99.0);
        for (const auto v : tet.vertices()) {
            REQUIRE(v.quality() == 99.0);
        }

        // setElementQuality<ElemId::FACE>
        vcl::setElementQuality<vcl::ElemId::FACE>(tet, -5.5);
        for (const auto f : tet.faces()) {
            REQUIRE(f.quality() == -5.5);
        }
    }

    // --- 2. QualityMesh (float, non-optional vertex quality, optional face
    // quality) ---
    {
        QualityMesh qm;
        qm.enableFaceQuality();

        vcl::uint v0 = qm.addVertex(Point3f(0.0f, 0.0f, 0.0f));
        vcl::uint v1 = qm.addVertex(Point3f(1.0f, 0.0f, 0.0f));
        vcl::uint v2 = qm.addVertex(Point3f(0.0f, 1.0f, 0.0f));
        vcl::uint f0 = qm.addFace(v0, v1, v2);

        vcl::setVertexQuality(qm, 12.5f);
        for (const auto v : qm.vertices()) {
            REQUIRE(std::abs(v.quality() - 12.5f) < 1e-5f);
        }

        vcl::setFaceQuality(qm, 34.5f);
        REQUIRE(std::abs(qm.face(f0).quality() - 34.5f) < 1e-5f);

        vcl::setElementQuality<vcl::ElemId::VERTEX>(qm, 50.0f);
        for (const auto v : qm.vertices()) {
            REQUIRE(std::abs(v.quality() - 50.0f) < 1e-5f);
        }

        vcl::setElementQuality<vcl::ElemId::FACE>(qm, 70.0f);
        REQUIRE(std::abs(qm.face(f0).quality() - 70.0f) < 1e-5f);
    }

    // --- 3. EdgeMesh (optional edge quality) ---
    {
        vcl::EdgeMesh em;
        em.enableEdgeQuality();
        em.addVertices(
            vcl::Point3d(0.0, 0.0, 0.0),
            vcl::Point3d(1.0, 0.0, 0.0),
            vcl::Point3d(2.0, 0.0, 0.0));
        em.addEdge(0, 1);
        em.addEdge(1, 2);

        vcl::setElementQuality<vcl::ElemId::EDGE>(em, 7.5);
        for (const auto e : em.edges()) {
            REQUIRE(e.quality() == 7.5);
        }
    }
}

TEST_CASE("Update Quality - Clamp Quality")
{
    // --- 1. Vertex Quality Clamping ---
    {
        vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
        tet.enableVertexQuality();

        tet.vertex(0).quality() = -5.0;
        tet.vertex(1).quality() = 0.5;
        tet.vertex(2).quality() = 2.0;
        tet.vertex(3).quality() = 10.0;

        // Custom range clamping
        vcl::clampVertexQuality(tet, 0.0, 5.0);
        REQUIRE(tet.vertex(0).quality() == 0.0);
        REQUIRE(tet.vertex(1).quality() == 0.5);
        REQUIRE(tet.vertex(2).quality() == 2.0);
        REQUIRE(tet.vertex(3).quality() == 5.0);

        // Default range clamping [0, 1]
        tet.vertex(0).quality() = -0.8;
        tet.vertex(1).quality() = 0.3;
        tet.vertex(2).quality() = 0.9;
        tet.vertex(3).quality() = 2.5;

        vcl::clampVertexQuality(tet);
        REQUIRE(tet.vertex(0).quality() == 0.0);
        REQUIRE(tet.vertex(1).quality() == 0.3);
        REQUIRE(tet.vertex(2).quality() == 0.9);
        REQUIRE(tet.vertex(3).quality() == 1.0);

        // clampElementQuality<ElemId::VERTEX>
        tet.vertex(0).quality() = -10.0;
        tet.vertex(1).quality() = 15.0;
        tet.vertex(2).quality() = 25.0;
        tet.vertex(3).quality() = 50.0;

        vcl::clampElementQuality<vcl::ElemId::VERTEX>(tet, 10.0, 30.0);
        REQUIRE(tet.vertex(0).quality() == 10.0);
        REQUIRE(tet.vertex(1).quality() == 15.0);
        REQUIRE(tet.vertex(2).quality() == 25.0);
        REQUIRE(tet.vertex(3).quality() == 30.0);
    }

    // --- 2. Face Quality Clamping ---
    {
        vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
        tet.enableFaceQuality();

        tet.face(0).quality() = -10.0;
        tet.face(1).quality() = 25.0;
        tet.face(2).quality() = 50.0;
        tet.face(3).quality() = 120.0;

        // Custom range clamping
        vcl::clampFaceQuality(tet, 0.0, 100.0);
        REQUIRE(tet.face(0).quality() == 0.0);
        REQUIRE(tet.face(1).quality() == 25.0);
        REQUIRE(tet.face(2).quality() == 50.0);
        REQUIRE(tet.face(3).quality() == 100.0);

        // Default range clamping [0, 1]
        tet.face(0).quality() = -2.0;
        tet.face(1).quality() = 0.4;
        tet.face(2).quality() = 0.7;
        tet.face(3).quality() = 3.0;

        vcl::clampFaceQuality(tet);
        REQUIRE(tet.face(0).quality() == 0.0);
        REQUIRE(tet.face(1).quality() == 0.4);
        REQUIRE(tet.face(2).quality() == 0.7);
        REQUIRE(tet.face(3).quality() == 1.0);

        // clampElementQuality<ElemId::FACE>
        tet.face(0).quality() = -1.0;
        tet.face(1).quality() = 5.0;
        tet.face(2).quality() = 10.0;
        tet.face(3).quality() = 20.0;

        vcl::clampElementQuality<vcl::ElemId::FACE>(tet, 0.0, 8.0);
        REQUIRE(tet.face(0).quality() == 0.0);
        REQUIRE(tet.face(1).quality() == 5.0);
        REQUIRE(tet.face(2).quality() == 8.0);
        REQUIRE(tet.face(3).quality() == 8.0);
    }

    // --- 3. Edge Quality Clamping ---
    {
        vcl::EdgeMesh em;
        em.enableEdgeQuality();
        em.addVertices(
            vcl::Point3d(0.0, 0.0, 0.0),
            vcl::Point3d(1.0, 0.0, 0.0),
            vcl::Point3d(2.0, 0.0, 0.0));
        em.addEdge(0, 1);
        em.addEdge(1, 2);

        em.edge(0).quality() = -5.0;
        em.edge(1).quality() = 15.0;

        vcl::clampElementQuality<vcl::ElemId::EDGE>(em, 0.0, 10.0);
        REQUIRE(em.edge(0).quality() == 0.0);
        REQUIRE(em.edge(1).quality() == 10.0);
    }
}

TEST_CASE("Update Quality - Normalize Quality")
{
    // --- 1. Vertex Quality Normalization ---
    {
        vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
        tet.enableVertexQuality();

        tet.vertex(0).quality() = 10.0;
        tet.vertex(1).quality() = 20.0;
        tet.vertex(2).quality() = 30.0;
        tet.vertex(3).quality() = 40.0;

        // Default [0, 1] normalization
        vcl::normalizeVertexQuality(tet);
        REQUIRE(std::abs(tet.vertex(0).quality() - 0.0) < 1e-6);
        REQUIRE(std::abs(tet.vertex(1).quality() - (1.0 / 3.0)) < 1e-6);
        REQUIRE(std::abs(tet.vertex(2).quality() - (2.0 / 3.0)) < 1e-6);
        REQUIRE(std::abs(tet.vertex(3).quality() - 1.0) < 1e-6);

        // Custom [minQ, maxQ] normalization
        tet.vertex(0).quality() = 10.0;
        tet.vertex(1).quality() = 20.0;
        tet.vertex(2).quality() = 30.0;
        tet.vertex(3).quality() = 40.0;

        vcl::normalizeVertexQuality(tet, 100.0, 400.0);
        REQUIRE(std::abs(tet.vertex(0).quality() - 100.0) < 1e-6);
        REQUIRE(std::abs(tet.vertex(1).quality() - 200.0) < 1e-6);
        REQUIRE(std::abs(tet.vertex(2).quality() - 300.0) < 1e-6);
        REQUIRE(std::abs(tet.vertex(3).quality() - 400.0) < 1e-6);

        // normalizeElementQuality<ElemId::VERTEX>
        tet.vertex(0).quality() = 2.0;
        tet.vertex(1).quality() = 4.0;
        tet.vertex(2).quality() = 6.0;
        tet.vertex(3).quality() = 8.0;

        vcl::normalizeElementQuality<vcl::ElemId::VERTEX>(tet, -10.0, 10.0);
        REQUIRE(std::abs(tet.vertex(0).quality() - (-10.0)) < 1e-6);
        REQUIRE(
            std::abs(tet.vertex(1).quality() - (-10.0 + 20.0 * (1.0 / 3.0))) <
            1e-6);
        REQUIRE(
            std::abs(tet.vertex(2).quality() - (-10.0 + 20.0 * (2.0 / 3.0))) <
            1e-6);
        REQUIRE(std::abs(tet.vertex(3).quality() - 10.0) < 1e-6);
    }

    // --- 2. Face Quality Normalization ---
    {
        vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
        tet.enableFaceQuality();

        tet.face(0).quality() = 1.0;
        tet.face(1).quality() = 2.0;
        tet.face(2).quality() = 3.0;
        tet.face(3).quality() = 4.0;

        // Default [0, 1] normalization
        vcl::normalizeFaceQuality(tet);
        REQUIRE(std::abs(tet.face(0).quality() - 0.0) < 1e-6);
        REQUIRE(std::abs(tet.face(1).quality() - (1.0 / 3.0)) < 1e-6);
        REQUIRE(std::abs(tet.face(2).quality() - (2.0 / 3.0)) < 1e-6);
        REQUIRE(std::abs(tet.face(3).quality() - 1.0) < 1e-6);

        // Custom [minQ, maxQ] normalization
        tet.face(0).quality() = 1.0;
        tet.face(1).quality() = 2.0;
        tet.face(2).quality() = 3.0;
        tet.face(3).quality() = 4.0;

        vcl::normalizeFaceQuality(tet, -10.0, 20.0);
        REQUIRE(std::abs(tet.face(0).quality() - (-10.0)) < 1e-6);
        REQUIRE(std::abs(tet.face(1).quality() - 0.0) < 1e-6);
        REQUIRE(std::abs(tet.face(2).quality() - 10.0) < 1e-6);
        REQUIRE(std::abs(tet.face(3).quality() - 20.0) < 1e-6);

        // normalizeElementQuality<ElemId::FACE>
        tet.face(0).quality() = 100.0;
        tet.face(1).quality() = 200.0;
        tet.face(2).quality() = 300.0;
        tet.face(3).quality() = 400.0;

        vcl::normalizeElementQuality<vcl::ElemId::FACE>(tet);
        REQUIRE(std::abs(tet.face(0).quality() - 0.0) < 1e-6);
        REQUIRE(std::abs(tet.face(1).quality() - (1.0 / 3.0)) < 1e-6);
        REQUIRE(std::abs(tet.face(2).quality() - (2.0 / 3.0)) < 1e-6);
        REQUIRE(std::abs(tet.face(3).quality() - 1.0) < 1e-6);
    }

    // --- 3. Edge Quality Normalization ---
    {
        vcl::EdgeMesh em;
        em.enableEdgeQuality();
        em.addVertices(
            vcl::Point3d(0.0, 0.0, 0.0),
            vcl::Point3d(1.0, 0.0, 0.0),
            vcl::Point3d(2.0, 0.0, 0.0));
        em.addEdge(0, 1);
        em.addEdge(1, 2);

        em.edge(0).quality() = 10.0;
        em.edge(1).quality() = 30.0;

        vcl::normalizeElementQuality<vcl::ElemId::EDGE>(em, 0.0, 1.0);
        REQUIRE(std::abs(em.edge(0).quality() - 0.0) < 1e-6);
        REQUIRE(std::abs(em.edge(1).quality() - 1.0) < 1e-6);
    }
}

TEST_CASE("Update Quality - Quality From Valence And Area")
{
    // --- 1. Valence on Tetrahedron (each vertex has 3 incident faces) ---
    {
        vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
        tet.enableVertexQuality();

        vcl::setVertexQualityFromVertexValence(tet);
        for (const auto v : tet.vertices()) {
            REQUIRE(v.quality() == 3.0);
        }
    }

    // --- 2. Valence on Quad (2 triangles sharing edge 1-2) ---
    {
        vcl::TriMesh m;
        m.enableVertexQuality();
        m.addVertices(
            vcl::Point3d(0.0, 0.0, 0.0),
            vcl::Point3d(1.0, 0.0, 0.0),
            vcl::Point3d(0.0, 1.0, 0.0),
            vcl::Point3d(1.0, 1.0, 0.0));
        m.addFace(0, 1, 2);
        m.addFace(1, 3, 2);

        vcl::setVertexQualityFromVertexValence(m);
        REQUIRE(m.vertex(0).quality() == 1.0);
        REQUIRE(m.vertex(1).quality() == 2.0);
        REQUIRE(m.vertex(2).quality() == 2.0);
        REQUIRE(m.vertex(3).quality() == 1.0);
    }

    // --- 3. Face Area ---
    {
        vcl::TriMesh m;
        m.enableFaceQuality();
        m.addVertices(
            vcl::Point3d(0.0, 0.0, 0.0),
            vcl::Point3d(1.0, 0.0, 0.0),
            vcl::Point3d(0.0, 1.0, 0.0),
            vcl::Point3d(0.0, 4.0, 0.0),
            vcl::Point3d(3.0, 0.0, 0.0));

        // Triangle 0: (0,0,0)-(1,0,0)-(0,1,0) -> right triangle, area = 0.5
        m.addFace(0, 1, 2);
        // Triangle 1: (0,0,0)-(3,0,0)-(0,4,0) -> right triangle, area = 0.5 * 3
        // * 4 = 6.0
        m.addFace(0, 4, 3);

        vcl::setFaceQualityFromFaceArea(m);
        REQUIRE(std::abs(m.face(0).quality() - 0.5) < 1e-6);
        REQUIRE(std::abs(m.face(1).quality() - 6.0) < 1e-6);
    }
}

TEST_CASE("Update Quality - Quality From Curvature")
{
    vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
    m.enableVertexQuality();
    m.enableVertexPrincipalCurvature();

    // Setup known principal curvature values on vertices
    // vertex 0: k1 = 3.0, k2 = 2.0
    m.vertex(0).principalCurvature().maxValue() = 3.0;
    m.vertex(0).principalCurvature().minValue() = 2.0;

    // vertex 1: k1 = 4.0, k2 = -2.0
    m.vertex(1).principalCurvature().maxValue() = 4.0;
    m.vertex(1).principalCurvature().minValue() = -2.0;

    // vertex 2: k1 = 1.0, k2 = 1.0
    m.vertex(2).principalCurvature().maxValue() = 1.0;
    m.vertex(2).principalCurvature().minValue() = 1.0;

    // vertex 3: k1 = -1.0, k2 = -5.0
    m.vertex(3).principalCurvature().maxValue() = -1.0;
    m.vertex(3).principalCurvature().minValue() = -5.0;

    // --- 1. Gaussian Curvature: K = k1 * k2 ---
    {
        vcl::setVertexQualityFromPrincipalCurvatureGaussian(m);
        REQUIRE(std::abs(m.vertex(0).quality() - 6.0) < 1e-6);
        REQUIRE(std::abs(m.vertex(1).quality() - (-8.0)) < 1e-6);
        REQUIRE(std::abs(m.vertex(2).quality() - 1.0) < 1e-6);
        REQUIRE(std::abs(m.vertex(3).quality() - 5.0) < 1e-6);
    }

    // --- 2. Mean Curvature: H = (k1 + k2) / 2 ---
    {
        vcl::setVertexQualityFromPrincipalCurvatureMean(m);
        REQUIRE(std::abs(m.vertex(0).quality() - 2.5) < 1e-6);
        REQUIRE(std::abs(m.vertex(1).quality() - 1.0) < 1e-6);
        REQUIRE(std::abs(m.vertex(2).quality() - 1.0) < 1e-6);
        REQUIRE(std::abs(m.vertex(3).quality() - (-3.0)) < 1e-6);
    }

    // --- 3. Min Value Curvature: k2 ---
    {
        vcl::setVertexQualityFromPrincipalCurvatureMinValue(m);
        REQUIRE(std::abs(m.vertex(0).quality() - 2.0) < 1e-6);
        REQUIRE(std::abs(m.vertex(1).quality() - (-2.0)) < 1e-6);
        REQUIRE(std::abs(m.vertex(2).quality() - 1.0) < 1e-6);
        REQUIRE(std::abs(m.vertex(3).quality() - (-5.0)) < 1e-6);
    }

    // --- 4. Max Value Curvature: k1 ---
    {
        vcl::setVertexQualityFromPrincipalCurvatureMaxValue(m);
        REQUIRE(std::abs(m.vertex(0).quality() - 3.0) < 1e-6);
        REQUIRE(std::abs(m.vertex(1).quality() - 4.0) < 1e-6);
        REQUIRE(std::abs(m.vertex(2).quality() - 1.0) < 1e-6);
        REQUIRE(std::abs(m.vertex(3).quality() - (-1.0)) < 1e-6);
    }

    // --- 5. Shape Index: S = (2 / pi) * atan2(k1 + k2, k1 - k2) ---
    {
        // For vertex 2: k1 = 1.0, k2 = 1.0 -> atan2(2, 0) = pi/2 -> S = 1.0
        // For vertex 3: test swap branch: set maxValue < minValue to verify
        // std::swap
        m.vertex(3).principalCurvature().maxValue() = -5.0;
        m.vertex(3).principalCurvature().minValue() = -1.0;

        vcl::setVertexQualityFromPrincipalCurvatureShapeIndex(m);

        // v0: k1 = 3, k2 = 2 -> atan2(5, 1)
        double exp0 = (2.0 / M_PI) * std::atan2(5.0, 1.0);
        REQUIRE(std::abs(m.vertex(0).quality() - exp0) < 1e-6);

        // v1: k1 = 4, k2 = -2 -> atan2(2, 6)
        double exp1 = (2.0 / M_PI) * std::atan2(2.0, 6.0);
        REQUIRE(std::abs(m.vertex(1).quality() - exp1) < 1e-6);

        // v2: spherical cap -> S = 1.0
        REQUIRE(std::abs(m.vertex(2).quality() - 1.0) < 1e-6);

        // v3: after swap k1 = -1, k2 = -5 -> atan2(-6, 4)
        double exp3 = (2.0 / M_PI) * std::atan2(-6.0, 4.0);
        REQUIRE(std::abs(m.vertex(3).quality() - exp3) < 1e-6);
    }

    // --- 6. Curvedness: C = sqrt((k1^2 + k2^2) / 2) ---
    {
        m.vertex(0).principalCurvature().maxValue() = 3.0;
        m.vertex(0).principalCurvature().minValue() = 4.0;

        m.vertex(1).principalCurvature().maxValue() = 0.0;
        m.vertex(1).principalCurvature().minValue() = 0.0;

        m.vertex(2).principalCurvature().maxValue() = 2.0;
        m.vertex(2).principalCurvature().minValue() = 2.0;

        m.vertex(3).principalCurvature().maxValue() = -3.0;
        m.vertex(3).principalCurvature().minValue() = -3.0;

        vcl::setVertexQualityFromPrincipalCurvatureCurvedness(m);

        // v0: sqrt((9 + 16) / 2) = sqrt(12.5)
        REQUIRE(std::abs(m.vertex(0).quality() - std::sqrt(12.5)) < 1e-6);
        // v1: sqrt(0) = 0.0
        REQUIRE(std::abs(m.vertex(1).quality() - 0.0) < 1e-6);
        // v2: sqrt((4 + 4) / 2) = 2.0
        REQUIRE(std::abs(m.vertex(2).quality() - 2.0) < 1e-6);
        // v3: sqrt((9 + 9) / 2) = 3.0
        REQUIRE(std::abs(m.vertex(3).quality() - 3.0) < 1e-6);
    }
}

TEST_CASE("Update Quality - Update Quality Exceptions")
{
    // --- 1. Vertex Quality Missing ---
    {
        vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
        // Quality is initially disabled in TriMesh

        assertThrowsMissingQualityComponent([&]() {
            vcl::setElementQuality<vcl::ElemId::VERTEX>(tet, 1.0);
        });
        assertThrowsMissingQualityComponent(
            [&]() { vcl::setVertexQuality(tet, 1.0); });

        assertThrowsMissingQualityComponent([&]() {
            vcl::clampElementQuality<vcl::ElemId::VERTEX>(tet, 0.0, 1.0);
        });
        assertThrowsMissingQualityComponent(
            [&]() { vcl::clampVertexQuality(tet); });
        assertThrowsMissingQualityComponent(
            [&]() { vcl::clampVertexQuality(tet, 0.0, 1.0); });

        assertThrowsMissingQualityComponent([&]() {
            vcl::normalizeElementQuality<vcl::ElemId::VERTEX>(tet);
        });
        assertThrowsMissingQualityComponent(
            [&]() { vcl::normalizeVertexQuality(tet); });
        assertThrowsMissingQualityComponent(
            [&]() { vcl::normalizeVertexQuality(tet, 0.0, 1.0); });

        assertThrowsMissingQualityComponent(
            [&]() { vcl::setVertexQualityFromVertexValence(tet); });
    }

    // --- 2. Face Quality Missing ---
    {
        vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();

        assertThrowsMissingQualityComponent(
            [&]() { vcl::setElementQuality<vcl::ElemId::FACE>(tet, 1.0); });
        assertThrowsMissingQualityComponent(
            [&]() { vcl::setFaceQuality(tet, 1.0); });

        assertThrowsMissingQualityComponent([&]() {
            vcl::clampElementQuality<vcl::ElemId::FACE>(tet, 0.0, 1.0);
        });
        assertThrowsMissingQualityComponent(
            [&]() { vcl::clampFaceQuality(tet); });
        assertThrowsMissingQualityComponent(
            [&]() { vcl::clampFaceQuality(tet, 0.0, 1.0); });

        assertThrowsMissingQualityComponent(
            [&]() { vcl::normalizeElementQuality<vcl::ElemId::FACE>(tet); });
        assertThrowsMissingQualityComponent(
            [&]() { vcl::normalizeFaceQuality(tet); });
        assertThrowsMissingQualityComponent(
            [&]() { vcl::normalizeFaceQuality(tet, 0.0, 1.0); });

        assertThrowsMissingQualityComponent(
            [&]() { vcl::setFaceQualityFromFaceArea(tet); });
    }

    // --- 3. Curvature Functions: Missing Quality or Principal Curvature ---
    {
        // Case A: Missing both quality and principal curvature
        vcl::TriMesh tetA = vcl::createTetrahedron<vcl::TriMesh>();
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureGaussian(tetA);
        });
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureMean(tetA);
        });
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureMinValue(tetA);
        });
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureMaxValue(tetA);
        });
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureShapeIndex(tetA);
        });
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureCurvedness(tetA);
        });

        // Case B: Quality enabled, but Principal Curvature missing
        vcl::TriMesh tetB = vcl::createTetrahedron<vcl::TriMesh>();
        tetB.enableVertexQuality();
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureGaussian(tetB);
        });
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureMean(tetB);
        });
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureMinValue(tetB);
        });
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureMaxValue(tetB);
        });
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureShapeIndex(tetB);
        });
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureCurvedness(tetB);
        });

        // Case C: Principal Curvature enabled, but Quality missing
        vcl::TriMesh tetC = vcl::createTetrahedron<vcl::TriMesh>();
        tetC.enableVertexPrincipalCurvature();
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureGaussian(tetC);
        });
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureMean(tetC);
        });
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureMinValue(tetC);
        });
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureMaxValue(tetC);
        });
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureShapeIndex(tetC);
        });
        assertThrowsMissingQualityComponent([&]() {
            vcl::setVertexQualityFromPrincipalCurvatureCurvedness(tetC);
        });
    }
}
