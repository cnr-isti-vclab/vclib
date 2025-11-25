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

#include <vclib/algorithms.h>
#include <vclib/meshes.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <random>

TEMPLATE_TEST_CASE(
    "Convex Hull from Point Set",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh = TestType;
    using Scalar  = TriMesh::VertexType::PositionType::ScalarType;
    using Point   = vcl::Point3<Scalar>;

    SECTION("Cube with internal point")
    {
        // Define the 8 corners of a unit cube plus an internal point
        std::vector<Point> points = {
            {0,   0,   0  }, // vertex 0
            {1,   0,   0  }, // vertex 1
            {0,   1,   0  }, // vertex 2
            {0,   0,   1  }, // vertex 3
            {1,   1,   0  }, // vertex 4
            {1,   0,   1  }, // vertex 5
            {0,   1,   1  }, // vertex 6
            {1,   1,   1  }, // vertex 7
            {0.5, 0.5, 0.5}  // internal point (should not be in hull)
        };

        // Generate a random seed
        std::random_device rd;
        vcl::uint          seed = rd();

        // Print the seed before computing the convex hull
        // This way, if the convex hull crashes, the seed will be printed
        std::cout << "Using seed: " << seed << std::endl;

        // Compute convex hull with the seed
        TriMesh hull = vcl::convexHull<TriMesh>(points, seed);

        // The convex hull of a cube should have 8 vertices (the corners)
        // and 12 triangular faces (6 quad faces of the cube, each split into 2
        // triangles)
        REQUIRE(hull.vertexNumber() == 8);
        REQUIRE(hull.faceNumber() == 12);

        // Verify that the hull is valid and closed
        REQUIRE(vcl::isWaterTight(hull));

        // Verify that all vertices of the hull are on the boundary
        // (i.e., none should be at position {0.5, 0.5, 0.5})
        bool hasInternalPoint = false;
        for (const auto& v : hull.vertices()) {
            if (v.position().x() == Scalar(0.5) &&
                v.position().y() == Scalar(0.5) &&
                v.position().z() == Scalar(0.5)) {
                hasInternalPoint = true;
                break;
            }
        }
        REQUIRE(!hasInternalPoint);

        // Verify that all original boundary points are present in the hull
        // (the 8 corners of the cube)
        int cornersFound = 0;
        for (vcl::uint i = 0; i < 8; ++i) {
            for (const auto& v : hull.vertices()) {
                if ((v.position() - points[i]).norm() < 1e-6) {
                    cornersFound++;
                    break;
                }
            }
        }
        REQUIRE(cornersFound == 8);
    }

    SECTION("Simple tetrahedron")
    {
        // Define the 4 vertices of a tetrahedron
        std::vector<Point> points = {
            {0, 0, 0},
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1}
        };

        // Generate a random seed
        std::random_device rd;
        vcl::uint          seed = rd();

        // Print the seed before computing the convex hull
        std::cout << "Using seed: " << seed << std::endl;

        // Compute convex hull with the seed
        TriMesh hull = vcl::convexHull<TriMesh>(points, seed);

        // The convex hull of a tetrahedron should have 4 vertices and 4 faces
        REQUIRE(hull.vertexNumber() == 4);
        REQUIRE(hull.faceNumber() == 4);

        // Verify that the hull is valid and closed
        REQUIRE(vcl::isWaterTight(hull));
    }
}
