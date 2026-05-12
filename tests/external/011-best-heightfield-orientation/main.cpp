/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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
#include <vclib/io.h>
#include <vclib/meshes.h>

#include <vclib/embree/heightfield_exterior_volume.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

static const vcl::Point2d CELL_SIDE_LENGTHS = {0.184, 0.234};
static const vcl::uint    NUM_DIRECTIONS    = 2000;

static const double ANGLE_TOLERANCE_DEGREES = 2.0;

TEMPLATE_TEST_CASE(
    "Find best Orientation by Heightfield Exterior Volume",
    "",
    vcl::TriMesh,
    vcl::PolyMesh,
    vcl::TriMeshIndexed,
    vcl::PolyMeshIndexed)
{
    using TestMesh = TestType;

    using namespace vcl;

    SECTION(vcl::meshTypeName<TestMesh>().c_str())
    {
        TestMesh m = loadMesh<TestMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");

        scale(m, 100.0);
        updatePerVertexAndFaceNormals(m);

        Point3d bestNormal =
            embree::findBestOrientationByHeightfieldExteriorVolume(
                m, CELL_SIDE_LENGTHS, NUM_DIRECTIONS);

        // the best orientation for the bunny mesh
        const Point3d EXPECTED_RESULT = Point3d(0.0, 1.0, 0.0);

        // require that the angle between bestNormal and EXPECTED_RESULT is less
        // than ANGLE_TOLERANCE_DEGREES
        const double angle = std::acos(
            std::clamp(
                bestNormal.normalized().dot(EXPECTED_RESULT), -1.0, 1.0));
        REQUIRE(angle < ANGLE_TOLERANCE_DEGREES * M_PI / 180.0);
    }
}
