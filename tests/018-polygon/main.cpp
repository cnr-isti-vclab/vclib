/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

#include <vclib/algorithms/core.h>
#include <vclib/space/core.h>

template<typename Scalar, unsigned int N>
vcl::Point<Scalar, N> randomPoint()
{
    vcl::Point<Scalar, N> p;
    for (unsigned int i = 0; i < N; i++)
        p[i] = GENERATE(take(1, random<Scalar>(-100, 100)));
    return p;
}

TEST_CASE("Collinearity and Points Order")
{
    vcl::Point2d a = {0.0, 0.0};
    vcl::Point2d b = {1.0, 0.0};
    vcl::Point2d c = {1.0, 1.0};
    vcl::Point2d d = {0.0, 1.0};
    vcl::Point2d e = {2.0, 0.0};

    REQUIRE(vcl::collinearityTest(a, b, c) > 0);
    REQUIRE(vcl::collinearityTest(a, d, b) < 0);
    REQUIRE(vcl::collinearityTest(a, b, e) == 0);
    REQUIRE(vcl::collinearityTest(a, c, e) < 0);

    REQUIRE(vcl::arePointsCounterClockwise(a, b, c) == true);
    REQUIRE(vcl::arePointsCounterClockwise(b, a, c) == false);
    REQUIRE(vcl::arePointsCounterClockwise(a, b, e) == false);
}

