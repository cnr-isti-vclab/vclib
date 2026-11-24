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

TEST_CASE("Stat Quality - Quality Min Max")
{
    // 1. TriMesh (double)
    vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
    tet.enableVertexQuality();
    tet.enableFaceQuality();

    tet.vertex(0).quality() = 1.0;
    tet.vertex(1).quality() = 4.5;
    tet.vertex(2).quality() = -2.0;
    tet.vertex(3).quality() = 3.0;

    auto [vMin, vMax] = vcl::vertexQualityMinMax(tet);
    REQUIRE(vMin == -2.0);
    REQUIRE(vMax == 4.5);

    auto [elemVMin, elemVMax] =
        vcl::elementQualityMinMax<vcl::ElemId::VERTEX>(tet);
    REQUIRE(elemVMin == -2.0);
    REQUIRE(elemVMax == 4.5);

    tet.face(0).quality() = 100.0;
    tet.face(1).quality() = 50.0;
    tet.face(2).quality() = 200.0;
    tet.face(3).quality() = 150.0;

    auto [fMin, fMax] = vcl::faceQualityMinMax(tet);
    REQUIRE(fMin == 50.0);
    REQUIRE(fMax == 200.0);

    auto [elemFMin, elemFMax] =
        vcl::elementQualityMinMax<vcl::ElemId::FACE>(tet);
    REQUIRE(elemFMin == 50.0);
    REQUIRE(elemFMax == 200.0);

    // 2. QualityMesh (float, vertex quality non-optional, face quality
    // optional)
    QualityMesh qm;
    qm.enableFaceQuality();

    vcl::uint v0 = qm.addVertex(Point3f(0.0f, 0.0f, 0.0f));
    vcl::uint v1 = qm.addVertex(Point3f(1.0f, 0.0f, 0.0f));
    vcl::uint v2 = qm.addVertex(Point3f(0.0f, 1.0f, 0.0f));

    qm.vertex(v0).quality() = 10.0f;
    qm.vertex(v1).quality() = 5.0f;
    qm.vertex(v2).quality() = 25.0f;

    auto [qmMin, qmMax] = vcl::vertexQualityMinMax(qm);
    REQUIRE(qmMin == 5.0f);
    REQUIRE(qmMax == 25.0f);

    vcl::uint f0          = qm.addFace(v0, v1, v2);
    qm.face(f0).quality() = 42.0f;

    auto [qmFMin, qmFMax] = vcl::faceQualityMinMax(qm);
    REQUIRE(qmFMin == 42.0f);
    REQUIRE(qmFMax == 42.0f);
}

TEST_CASE("Stat Quality - Quality Average")
{
    // 1. TriMesh (double)
    vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
    tet.enableVertexQuality();
    tet.enableFaceQuality();

    tet.vertex(0).quality() = 1.0;
    tet.vertex(1).quality() = 2.0;
    tet.vertex(2).quality() = 3.0;
    tet.vertex(3).quality() = 4.0;

    double vAvg = vcl::vertexQualityAverage(tet);
    REQUIRE(std::abs(vAvg - 2.5) < 1e-6);

    double elemVAvg = vcl::elementQualityAverage<vcl::ElemId::VERTEX>(tet);
    REQUIRE(std::abs(elemVAvg - 2.5) < 1e-6);

    tet.face(0).quality() = 10.0;
    tet.face(1).quality() = 20.0;
    tet.face(2).quality() = 30.0;
    tet.face(3).quality() = 40.0;

    double fAvg = vcl::faceQualityAverage(tet);
    REQUIRE(std::abs(fAvg - 25.0) < 1e-6);

    double elemFAvg = vcl::elementQualityAverage<vcl::ElemId::FACE>(tet);
    REQUIRE(std::abs(elemFAvg - 25.0) < 1e-6);

    // 2. QualityMesh (float)
    QualityMesh qm;
    vcl::uint   v0 = qm.addVertex(Point3f(0.0f, 0.0f, 0.0f));
    vcl::uint   v1 = qm.addVertex(Point3f(1.0f, 0.0f, 0.0f));
    vcl::uint   v2 = qm.addVertex(Point3f(0.0f, 1.0f, 0.0f));

    qm.vertex(v0).quality() = 10.0f;
    qm.vertex(v1).quality() = 20.0f;
    qm.vertex(v2).quality() = 60.0f;

    auto qmVAvg = vcl::vertexQualityAverage(qm);
    REQUIRE(std::abs(qmVAvg - 30.0) < 1e-5);
}

TEST_CASE("Stat Quality - Quality Histogram")
{
    vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
    tet.enableVertexQuality();
    tet.enableFaceQuality();

    tet.vertex(0).quality() = 0.0;
    tet.vertex(1).quality() = 10.0;
    tet.vertex(2).quality() = 20.0;
    tet.vertex(3).quality() = 30.0;

    // Full vertex histogram
    auto vHist = vcl::vertexQualityHistogram(tet, false, 10);
    REQUIRE(vHist.binCount() == 10);
    REQUIRE(vHist.valueCount() == 4);
    REQUIRE(vHist.valueMin() == 0.0);
    REQUIRE(vHist.valueMax() == 30.0);
    REQUIRE(std::abs(vHist.average() - 15.0) < 1e-6);

    // Selected vertices only
    tet.vertex(0).selected() = true;
    tet.vertex(1).selected() = false;
    tet.vertex(2).selected() = true;
    tet.vertex(3).selected() = false;

    auto vHistSel = vcl::vertexQualityHistogram(tet, true, 10);
    REQUIRE(vHistSel.valueCount() == 2);
    REQUIRE(vHistSel.valueMin() == 0.0);
    REQUIRE(vHistSel.valueMax() == 20.0);
    REQUIRE(std::abs(vHistSel.average() - 10.0) < 1e-6);

    // Generic element quality histogram
    auto elemVHist =
        vcl::elementQualityHistogram<vcl::ElemId::VERTEX>(tet, false, 10);
    REQUIRE(elemVHist.valueCount() == 4);

    // Faces histogram
    tet.face(0).quality() = 100.0;
    tet.face(1).quality() = 200.0;
    tet.face(2).quality() = 300.0;
    tet.face(3).quality() = 400.0;

    auto fHist = vcl::faceQualityHistogram(tet, false, 10);
    REQUIRE(fHist.valueCount() == 4);
    REQUIRE(fHist.valueMin() == 100.0);
    REQUIRE(fHist.valueMax() == 400.0);
    REQUIRE(std::abs(fHist.average() - 250.0) < 1e-6);

    tet.face(0).selected() = false;
    tet.face(1).selected() = true;
    tet.face(2).selected() = false;
    tet.face(3).selected() = true;

    auto fHistSel = vcl::faceQualityHistogram(tet, true, 10);
    REQUIRE(fHistSel.valueCount() == 2);
    REQUIRE(fHistSel.valueMin() == 200.0);
    REQUIRE(fHistSel.valueMax() == 400.0);
    REQUIRE(std::abs(fHistSel.average() - 300.0) < 1e-6);

    auto elemFHist =
        vcl::elementQualityHistogram<vcl::ElemId::FACE>(tet, true, 10);
    REQUIRE(elemFHist.valueCount() == 2);
}

TEST_CASE("Stat Quality - Vertex Radius From Quality")
{
    vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
    tet.enableVertexQuality();

    tet.vertex(0).quality() = 0.0;
    tet.vertex(1).quality() = 10.0;
    tet.vertex(2).quality() = 20.0;
    tet.vertex(3).quality() = 30.0;

    double diskRadius     = 2.0;
    double radiusVariance = 4.0;
    // minRad = 2.0, maxRad = 8.0, deltaRad = 6.0, deltaQ = 30.0
    // invert = false:
    // v0: 2.0 + 6.0 * (0.0 / 30.0) = 2.0
    // v1: 2.0 + 6.0 * (10.0 / 30.0) = 4.0
    // v2: 2.0 + 6.0 * (20.0 / 30.0) = 6.0
    // v3: 2.0 + 6.0 * (30.0 / 30.0) = 8.0
    auto rad =
        vcl::vertexRadiusFromQuality(tet, diskRadius, radiusVariance, false);
    REQUIRE(rad.size() == tet.vertexContainerSize());
    REQUIRE(std::abs(rad[0] - 2.0) < 1e-6);
    REQUIRE(std::abs(rad[1] - 4.0) < 1e-6);
    REQUIRE(std::abs(rad[2] - 6.0) < 1e-6);
    REQUIRE(std::abs(rad[3] - 8.0) < 1e-6);

    // invert = true:
    // v0: 2.0 + 6.0 * (30.0 / 30.0) = 8.0
    // v1: 2.0 + 6.0 * (20.0 / 30.0) = 6.0
    // v2: 2.0 + 6.0 * (10.0 / 30.0) = 4.0
    // v3: 2.0 + 6.0 * (0.0 / 30.0) = 2.0
    auto radInv =
        vcl::vertexRadiusFromQuality(tet, diskRadius, radiusVariance, true);
    REQUIRE(radInv.size() == tet.vertexContainerSize());
    REQUIRE(std::abs(radInv[0] - 8.0) < 1e-6);
    REQUIRE(std::abs(radInv[1] - 6.0) < 1e-6);
    REQUIRE(std::abs(radInv[2] - 4.0) < 1e-6);
    REQUIRE(std::abs(radInv[3] - 2.0) < 1e-6);
}

TEST_CASE("Stat Quality - Quality Exceptions")
{
    // In TriMesh, quality is optional and initially disabled
    vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();

    bool caught = false;
    try {
        [[maybe_unused]] auto res = vcl::vertexQualityMinMax(tet);
    }
    catch (const vcl::MissingComponentException&) {
        caught = true;
    }
    REQUIRE(caught);

    caught = false;
    try {
        [[maybe_unused]] auto res = vcl::vertexQualityAverage(tet);
    }
    catch (const vcl::MissingComponentException&) {
        caught = true;
    }
    REQUIRE(caught);

    caught = false;
    try {
        [[maybe_unused]] auto res = vcl::vertexQualityHistogram(tet);
    }
    catch (const vcl::MissingComponentException&) {
        caught = true;
    }
    REQUIRE(caught);

    caught = false;
    try {
        [[maybe_unused]] auto res = vcl::vertexRadiusFromQuality(tet, 1.0, 2.0);
    }
    catch (const vcl::MissingComponentException&) {
        caught = true;
    }
    REQUIRE(caught);

    caught = false;
    try {
        [[maybe_unused]] auto res = vcl::faceQualityMinMax(tet);
    }
    catch (const vcl::MissingComponentException&) {
        caught = true;
    }
    REQUIRE(caught);

    caught = false;
    try {
        [[maybe_unused]] auto res = vcl::faceQualityAverage(tet);
    }
    catch (const vcl::MissingComponentException&) {
        caught = true;
    }
    REQUIRE(caught);

    caught = false;
    try {
        [[maybe_unused]] auto res = vcl::faceQualityHistogram(tet);
    }
    catch (const vcl::MissingComponentException&) {
        caught = true;
    }
    REQUIRE(caught);
}
