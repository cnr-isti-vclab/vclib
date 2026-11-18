// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/meshes.h>
#include <vclib/space.h>

#include <Eigen/Geometry>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

#include <cmath>
#include <type_traits>

TEMPLATE_TEST_CASE(
    "Mesh inertia of an asymmetric tetrahedron",
    "[mesh-inertia]",
    vcl::TriMesh,
    vcl::TriMeshf)
{
    using Scalar           = TestType::VertexType::PositionType::ScalarType;
    using Point            = vcl::Point3<Scalar>;
    const double tolerance = std::is_same_v<Scalar, float> ? 3e-5 : 1e-12;

    TestType mesh;
    mesh.addVertex(Point(0, 0, 0));
    mesh.addVertex(Point(2, 0, 0));
    mesh.addVertex(Point(0, 3, 0));
    mesh.addVertex(Point(0, 0, 4));
    mesh.addFace(0, 2, 1);
    mesh.addFace(0, 1, 3);
    mesh.addFace(0, 3, 2);
    mesh.addFace(1, 2, 3);

    Eigen::Vector3d expectedCenter(0.5, 0.75, 1.0);
    Eigen::Matrix3d expected;
    expected << 3.75, 0.3, 0.4, 0.3, 3.0, 0.6, 0.4, 0.6, 1.95;
    double expectedVolume = 4.0;

    SECTION("Original")
    {
    }
    SECTION("Rotated and translated")
    {
        const Eigen::Matrix3d rotation =
            Eigen::AngleAxisd(0.6, Eigen::Vector3d(1, 2, 3).normalized())
                .toRotationMatrix();
        const Eigen::Vector3d translation(5, -3, 2);
        for (auto& vertex : mesh.vertices()) {
            const Eigen::Vector3d position(
                vertex.position()[0],
                vertex.position()[1],
                vertex.position()[2]);
            vertex.position() = Point(
                (rotation * position + translation).template cast<Scalar>());
        }
        expectedCenter = rotation * expectedCenter + translation;
        expected       = (rotation * expected * rotation.transpose()).eval();
    }
    SECTION("Reversed winding")
    {
        for (auto& face : mesh.faces()) {
            const auto first = face.vertexIndex(0);
            face.setVertex(0, face.vertexIndex(1));
            face.setVertex(1, first);
        }
        expectedVolume = -expectedVolume;
        expected       = -expected;
    }
    SECTION("Degenerate and deleted faces")
    {
        mesh.addFace(0, 0, 0);
        mesh.addFace(0, 1, 1);
        mesh.deleteFace(mesh.addFace(0, 1, 2));
    }

    const vcl::MeshInertia inertia(mesh);
    REQUIRE(
        inertia.volume() == Catch::Approx(expectedVolume).epsilon(tolerance));
    const auto center = inertia.centerOfMass();
    for (vcl::uint axis = 0; axis < 3; ++axis) {
        REQUIRE(
            center[axis] ==
            Catch::Approx(expectedCenter[axis]).epsilon(tolerance));
    }
    const auto tensor = inertia.template inertiaTensor<Eigen::Matrix3d>();
    REQUIRE(tensor.isApprox(expected, tolerance));

    Eigen::Matrix3d eigenvectors;
    Point           eigenvalues;
    inertia.inertiaTensorEigen(eigenvectors, eigenvalues);
    REQUIRE((eigenvectors * eigenvectors.transpose())
                .isApprox(Eigen::Matrix3d::Identity(), tolerance));
    for (vcl::uint axis = 0; axis < 3; ++axis) {
        const Eigen::Vector3d direction = eigenvectors.row(axis).transpose();
        REQUIRE((tensor * direction)
                    .isApprox(eigenvalues[axis] * direction, tolerance));
    }
    REQUIRE(eigenvalues[0] <= eigenvalues[1]);
    REQUIRE(eigenvalues[1] <= eigenvalues[2]);
}

TEMPLATE_TEST_CASE(
    "Mesh inertia of a polygonal box",
    "[mesh-inertia]",
    vcl::PolyMesh,
    vcl::PolyMeshf)
{
    using Scalar = TestType::VertexType::PositionType::ScalarType;
    using Point  = vcl::Point3<Scalar>;
    TestType mesh;
    mesh.addVertices(
        Point(0, 0, 0),
        Point(2, 0, 0),
        Point(2, 3, 0),
        Point(0, 3, 0),
        Point(0, 0, 4),
        Point(2, 0, 4),
        Point(2, 3, 4),
        Point(0, 3, 4));
    mesh.addFace(0, 3, 2, 1);
    mesh.addFace(4, 5, 6, 7);
    mesh.addFace(0, 1, 5, 4);
    mesh.addFace(1, 2, 6, 5);
    mesh.addFace(2, 3, 7, 6);
    mesh.addFace(3, 0, 4, 7);

    const vcl::MeshInertia inertia(mesh);
    REQUIRE(inertia.volume() == Catch::Approx(24.0));
    REQUIRE(inertia.centerOfMass() == Point(1, 1.5, 2));
    Eigen::Matrix3d expected = Eigen::Matrix3d::Zero();
    expected.diagonal() << 50, 40, 26;
    REQUIRE(inertia.template inertiaTensor<Eigen::Matrix3d>().isApprox(
        expected, 1e-12));
}

TEST_CASE("Mesh inertia at small scales", "[mesh-inertia]")
{
    const double scale = 1e-20;
    vcl::TriMesh mesh;
    mesh.addVertices(
        vcl::Point3d(0, 0, 0),
        vcl::Point3d(2 * scale, 0, 0),
        vcl::Point3d(0, 3 * scale, 0),
        vcl::Point3d(0, 0, 4 * scale));
    mesh.addFace(0, 2, 1);
    mesh.addFace(0, 1, 3);
    mesh.addFace(0, 3, 2);
    mesh.addFace(1, 2, 3);
    const vcl::MeshInertia inertia(mesh);
    REQUIRE(inertia.volume() / (scale * scale * scale) == Catch::Approx(4.0));
    const vcl::Point3d center = inertia.centerOfMass() / scale;
    REQUIRE(center[0] == Catch::Approx(0.5));
    REQUIRE(center[1] == Catch::Approx(0.75));
    REQUIRE(center[2] == Catch::Approx(1.0));
    Eigen::Matrix3d expected;
    expected << 3.75, 0.3, 0.4, 0.3, 3.0, 0.6, 0.4, 0.6, 1.95;
    REQUIRE((inertia.inertiaTensor<Eigen::Matrix3d>() / std::pow(scale, 5))
                .isApprox(expected, 1e-12));
}

TEST_CASE("Mesh inertia with zero volume", "[mesh-inertia]")
{
    vcl::TriMesh mesh;
    SECTION("Empty")
    {
    }
    SECTION("Planar")
    {
        mesh.addVertices(
            vcl::Point3d(0, 0, 0),
            vcl::Point3d(1, 0, 0),
            vcl::Point3d(0, 1, 0));
        mesh.addFace(0, 1, 2);
    }
    const vcl::MeshInertia inertia(mesh);
    REQUIRE(inertia.volume() == 0);
}
