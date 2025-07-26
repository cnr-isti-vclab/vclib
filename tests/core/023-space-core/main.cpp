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
