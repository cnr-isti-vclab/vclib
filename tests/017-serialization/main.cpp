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
#include <vclib/io/read.h>
#include <vclib/io/write.h>
#include <vclib/space.h>

template<typename Scalar, uint N>
vcl::Point<Scalar, N> randomPoint()
{
    vcl::Point<Scalar, N> p;
    for (uint i = 0; i < N; i++)
        p[i] = GENERATE(take(1, random<Scalar>(-100, 100)));
    return p;
}

template<typename Scalar, uint N>
vcl::Box<vcl::Point<Scalar, N>> randomBox()
{
    vcl::Box<vcl::Point<Scalar, N>> b(
        randomPoint<Scalar, N>(), randomPoint<Scalar, N>());
    return b;
}

vcl::Color randomColor()
{
    return vcl::Color(GENERATE(take(1, random(0, 255))),
                      GENERATE(take(1, random(0, 255))),
                      GENERATE(take(1, random(0, 255))),
                      GENERATE(take(1, random(0, 255))));
}

template<std::integral T>
vcl::BitSet<T> randomBitSet()
{
    vcl::BitSet<T> bs;
    for (uint i = 0; i < bs.size(); i++)
        bs.set(i, GENERATE(take(1, random(0, 1))));
    return bs;
}

TEMPLATE_TEST_CASE("Point Serialization", "", int, float, double)
{
    using Scalar = TestType;

    std::ofstream fo = vcl::openOutputFileStream(VCLIB_RESULTS_PATH
                                                 "/serialization/point3.bin");

    vcl::Point3<Scalar> p1 = randomPoint<Scalar, 3>();

    p1.serialize(fo);
    fo.close();

    vcl::Point3<Scalar> p2;

    std::ifstream fi = vcl::openInputFileStream(VCLIB_RESULTS_PATH
                                                "/serialization/point3.bin");

    p2.deserialize(fi);
    fi.close();

    REQUIRE(p1 == p2);
}

TEMPLATE_TEST_CASE("Box Serialization", "", int, float, double)
{
    using Scalar = TestType;

    std::ofstream fo = vcl::openOutputFileStream(
        VCLIB_RESULTS_PATH "/serialization/box3.bin");

    vcl::Box3<Scalar> b1 = randomBox<Scalar, 3>();

    b1.serialize(fo);
    fo.close();

    vcl::Box3<Scalar> b2;

    std::ifstream fi = vcl::openInputFileStream(
        VCLIB_RESULTS_PATH "/serialization/box3.bin");

    b2.deserialize(fi);
    fi.close();

    REQUIRE(b1 == b2);
}

TEST_CASE("Color Serialization")
{
    std::ofstream fo = vcl::openOutputFileStream(
        VCLIB_RESULTS_PATH "/serialization/color.bin");

    vcl::Color c1 = randomColor();

    c1.serialize(fo);
    fo.close();

    vcl::Color c2;

    std::ifstream fi = vcl::openInputFileStream(
        VCLIB_RESULTS_PATH "/serialization/color.bin");

    c2.deserialize(fi);
    fi.close();

    REQUIRE(c1 == c2);
}

TEMPLATE_TEST_CASE(
    "BitSet Serialization",
    "",
    uint8_t,
    uint16_t,
    uint32_t,
    uint64_t)
{
    using T = TestType;

    std::ofstream fo = vcl::openOutputFileStream(
        VCLIB_RESULTS_PATH "/serialization/bitset.bin");

    vcl::BitSet<T> bs1 = randomBitSet<T>();

    bs1.serialize(fo);
    fo.close();

    vcl::BitSet<T> bs2;

    std::ifstream fi = vcl::openInputFileStream(
        VCLIB_RESULTS_PATH "/serialization/bitset.bin");

    bs2.deserialize(fi);
    fi.close();

    REQUIRE(bs1 == bs2);
}
