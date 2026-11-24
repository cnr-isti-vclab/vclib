// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/space.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

TEST_CASE("Point sorting and min, max operations")
{
    vcl::Point2d a = {0.0, 0.0};
    vcl::Point2d b = {1.0, 0.0};
    vcl::Point2d c = {1.0, 1.0};
    vcl::Point2d d = {0.0, 1.0};

    std::vector<vcl::Point2d> points = {c, b, a, d};
    std::sort(points.begin(), points.end());

    REQUIRE(points[0] == a);
    REQUIRE(points[1] == d);
    REQUIRE(points[2] == b);
    REQUIRE(points[3] == c);

    auto minPoint = vcl::min(b, d);
    auto maxPoint = vcl::max(b, d);

    REQUIRE(minPoint == a);
    REQUIRE(maxPoint == c);
}
