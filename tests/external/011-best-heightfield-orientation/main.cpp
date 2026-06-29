// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
    vcl::TriMeshf,
    vcl::PolyMesh,
    vcl::PolyMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf,
    vcl::PolyMeshIndexed,
    vcl::PolyMeshIndexedf)
{
    using TestMesh   = TestType;
    using ScalarType = typename TestMesh::ScalarType;

    using namespace vcl;

    const auto sectionName = vcl::meshTypeName<TestMesh>();
    SECTION(sectionName.c_str())
    {
        TestMesh m = loadMesh<TestMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");

        scale(m, 100.0);
        updatePerVertexAndFaceNormals(m);

        Point3<ScalarType> bestNormal =
            embree::findBestOrientationByHeightfieldExteriorVolume(
                m, CELL_SIDE_LENGTHS, NUM_DIRECTIONS);

        // the best orientation for the bunny mesh
        const Point3<ScalarType> EXPECTED_RESULT =
            Point3<ScalarType>(0.0, 1.0, 0.0);

        // require that the angle between bestNormal and EXPECTED_RESULT is less
        // than ANGLE_TOLERANCE_DEGREES
        const ScalarType angle = std::acos(
            std::clamp(
                bestNormal.normalized().dot(EXPECTED_RESULT),
                ScalarType(-1.0),
                ScalarType(1.0)));
        REQUIRE(angle < ANGLE_TOLERANCE_DEGREES * M_PI / 180.0);
    }
}
