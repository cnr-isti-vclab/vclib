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

#include <vclib/algorithms/core/random.h>

#include <catch2/catch_test_macros.hpp>

// ============================================================================
//  DistConfig — integer
// ============================================================================

TEST_CASE("Random int with dist bounds [lo, hi] stays in range")
{
    static const int lo = -100;
    static const int hi = 200;
    std::mt19937     gen(42);

    static const int N = 5000;
    for (int i = 0; i < N; ++i) {
        int val = vcl::random<int>(std::pair<int, int> {lo, hi}, gen);
        REQUIRE(val >= lo);
        REQUIRE(val <= hi);
    }
}

TEST_CASE("Random int with custom DistConfig")
{
    std::mt19937 gen(42);

           // Custom dist: always return 5 when seed is used — deterministic.
    auto custom = [](std::mt19937&) -> int {
        return 5;
    };

    int val = vcl::random<int>(custom, gen);
    REQUIRE(val == 5);
}

// ============================================================================
//  DistConfig — float
// ============================================================================

TEST_CASE("Random float with monostate dist generates values in [0, 1)")
{
    std::mt19937 gen(42);

    static const int N = 1000;
    for (int i = 0; i < N; ++i) {
        double val = vcl::random<double>(std::monostate(), gen);
        REQUIRE(val >= 0.0);
        REQUIRE(val <= 1.0);
    }
}

TEST_CASE("Random float with dist bounds [lo, hi] stays in range")
{
    static const double lo = -50.5;
    static const double hi = 123.7;
    std::mt19937        gen(42);

    static const int N = 5000;
    for (int i = 0; i < N; ++i) {
        double val =
            vcl::random<double>(std::pair<double, double> {lo, hi}, gen);
        REQUIRE(val >= lo - 1e-12);
        REQUIRE(val <= hi + 1e-12);
    }
}

TEST_CASE("Random float with custom DistConfig")
{
    std::mt19937 gen(42);

    auto custom = [](std::mt19937&) -> double {
        return 3.14;
    };

    double val = vcl::random<double>(custom, gen);
    REQUIRE(vcl::epsilonEquals(val, 3.14));
}

// ============================================================================
//  RandomConfig determinism — integer
// ============================================================================

TEST_CASE("Same int seed always produces the same value")
{
    REQUIRE(
        vcl::random<int>(std::pair<int, int> {0, 1000}, uint(12345)) ==
        vcl::random<int>(std::pair<int, int> {0, 1000}, uint(12345)));
}

TEST_CASE("Different seeds produce different values")
{
    auto v1 = vcl::random<int>(std::pair<int, int> {0, INT32_MAX}, uint(1));
    auto v2 = vcl::random<int>(std::pair<int, int> {0, INT32_MAX}, uint(2));
    REQUIRE(v1 != v2);
}

TEST_CASE("Same seeded mt19937 produces a deterministic sequence")
{
    std::mt19937 gen1(99);
    std::mt19937 gen2(99);

    static const int N = 20;
    for (int i = 0; i < N; ++i) {
        REQUIRE(
            vcl::random<int>(std::pair<int, int> {0, 100}, gen1) ==
            vcl::random<int>(std::pair<int, int> {0, 100}, gen2));
    }
}

TEST_CASE("Different seeded mt19937 produce different sequences")
{
    std::mt19937 gen1(1);
    std::mt19937 gen2(2);

    auto v1 = vcl::random<int>(std::pair<int, int> {0, INT32_MAX}, gen1);
    auto v2 = vcl::random<int>(std::pair<int, int> {0, INT32_MAX}, gen2);
    REQUIRE(v1 != v2);
}

// ============================================================================
//  RandomConfig determinism — float
// ============================================================================

TEST_CASE("Same float seed always produces the same value")
{
    REQUIRE(
        vcl::random<double>(
            std::pair<double, double> {0.0, 1.0}, uint(77777)) ==
        vcl::random<double>(std::pair<double, double> {0.0, 1.0}, uint(77777)));
}

TEST_CASE("Same seeded mt19937 produces a deterministic float sequence")
{
    std::mt19937 gen1(42);
    std::mt19937 gen2(42);

    static const int N = 20;
    for (int i = 0; i < N; ++i) {
        REQUIRE(
            vcl::random<double>(std::pair<double, double> {-1.0, 1.0}, gen1) ==
            vcl::random<double>(std::pair<double, double> {-1.0, 1.0}, gen2));
    }
}

// ============================================================================
//  RandomConfig — custom function integration
// ============================================================================

TEST_CASE("Random with custom Dist and monostate Config works")
{
    auto dist = [](std::mt19937&) -> int {
        return 42;
    };
    REQUIRE(vcl::random<int>(dist) == 42);
}

TEST_CASE("Random with pair dist and seeded RandomConfig is deterministic")
{
    REQUIRE(
        vcl::random<double>(
            std::pair<double, double> {0.0, 100.0}, uint(555)) ==
        vcl::random<double>(std::pair<double, double> {0.0, 100.0}, uint(555)));
}
