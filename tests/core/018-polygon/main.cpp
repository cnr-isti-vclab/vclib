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

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

#include <random>

#include <vclib/algorithms/core.h>
#include <vclib/space/core.h>

template<typename Scalar, unsigned int N>
vcl::Point<Scalar, N> randomPoint()
{
    std::random_device rd;
    std::mt19937       gen(rd());

    vcl::Point<Scalar, N> p;
    for (unsigned int i = 0; i < N; i++)
        p[i] = std::uniform_real_distribution<Scalar>(-10.0, 10.0)(gen);
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

    REQUIRE(vcl::areCounterClockwise(a, b, c) == true);
    REQUIRE(vcl::areCounterClockwise(b, a, c) == false);
    REQUIRE(vcl::areCounterClockwise(a, b, e) == false);
}

TEST_CASE("Convex hull")
{
    std::vector<vcl::Point2d> v = {
        {0.3215348546593775,    0.03629583077160248  }, // 0
        {0.02402358131857918,   -0.2356728797179394  }, // 1
        {0.04590851212470659,   -0.4156409924995536  }, // 2
        {0.3218384001607433,    0.1379850698988746   }, // 3
        {0.11506479756447,      -0.1059521474930943  }, // 4
        {0.2622539999543261,    -0.29702873322836    }, // 5
        {-0.161920957418085,    -0.4055339716426413  }, // 6
        {0.1905378631228002,    0.3698601009043493   }, // 7
        {0.2387090918968516,    -0.01629827079949742 }, // 8
        {0.07495888748668034,   -0.1659825110491202  }, // 9
        {0.3319341836794598,    -0.1821814101954749  }, // 10
        {0.07703635755650362,   -0.2499430638271785  }, // 11
        {0.2069242999022122,    -0.2232970760420869  }, // 12
        {0.04604079532068295,   -0.1923573186549892  }, // 13
        {0.05054295812784038,   0.4754929463150845   }, // 14
        {-0.3900589168910486,   0.2797829520700341   }, // 15
        {0.3120693385713448,    -0.0506329867529059  }, // 16
        {0.01138812723698857,   0.4002504701728471   }, // 17
        {0.009645149586391732,  0.1060251100976254   }, // 18
        {-0.03597933197019559,  0.2953639456959105   }, // 19
        {0.1818290866742182,    0.001454397571696298 }, // 20
        {0.444056063372694,     0.2502497166863175   }, // 21
        {-0.05301752458607545,  -0.06553921621808712 }, // 22
        {0.4823896228171788,    -0.4776170002088109  }, // 23
        {-0.3089226845734964,   -0.06356112199235814 }, // 24
        {-0.271780741188471,    0.1810810595574612   }, // 25
        {0.4293626522918815,    0.2980897964891882   }, // 26
        {-0.004796652127799228, 0.382663812844701    }, // 27
        {0.430695573269106,     -0.2995073500084759  }, // 28
        {0.1799668387323309,    -0.2973467472915973  }, // 29
        {0.4932166845474547,    0.4928094162538735   }, // 30
        {-0.3521487911717489,   0.4352656197131292   }, // 31
        {-0.4907368011686362,   0.1865826865533206   }, // 32
        {-0.1047924716070224,   -0.247073392148198   }, // 33
        {0.4374961861758457,    -0.001606279519951237}, // 34
        {0.003256207800708899,  -0.2729194320486108  }, // 35
        {0.04310378203457577,   0.4452604050238248   }, // 36
        {0.4916198379282093,    -0.345391701297268   }, // 37
        {0.001675087028811806,  0.1531837672490476   }, // 38
        {-0.4404289572876217,   -0.2894855991839297  }, // 39
    };

    auto ch = vcl::convexHull(v);

    REQUIRE(ch.size() == 8);
    REQUIRE(ch[0] == v[32]);
    REQUIRE(ch[1] == v[39]);
    REQUIRE(ch[2] == v[6]);
    REQUIRE(ch[3] == v[23]);
    REQUIRE(ch[4] == v[37]);
    REQUIRE(ch[5] == v[30]);
    REQUIRE(ch[6] == v[14]);
    REQUIRE(ch[7] == v[31]);

    REQUIRE(vcl::isCounterClockWise(ch.begin(), ch.end()) == true);
}

TEST_CASE("Sort points of convex polygon")
{
    std::vector<vcl::Point2d> v;

    for (int i = 0; i < 100; i++)
        v.push_back(randomPoint<double, 2>());

    auto ch = vcl::convexHull(v);

    auto shuffled = ch;
    std::shuffle(
        shuffled.begin(),
        shuffled.end(),
        std::mt19937 {std::random_device {}()});

    vcl::sortConvexPolygonVertices(shuffled.begin(), shuffled.end());

    REQUIRE(shuffled.size() == ch.size());

    // find ch[0] in shuffled
    auto it = std::find(shuffled.begin(), shuffled.end(), ch[0]);
    REQUIRE(it != shuffled.end());
    unsigned int k = std::distance(shuffled.begin(), it);

    for (unsigned int i = 0; i < ch.size(); i++)
        REQUIRE(shuffled[(i + k) % ch.size()] == ch[i]);
}
