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
    return vcl::Color(
        GENERATE(take(1, random(0, 255))),
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

    std::ofstream fo =
        vcl::openOutputFileStream(VCLIB_RESULTS_PATH "/serialization/box3.bin");

    vcl::Box3<Scalar> b1 = randomBox<Scalar, 3>();

    b1.serialize(fo);
    fo.close();

    vcl::Box3<Scalar> b2;

    std::ifstream fi =
        vcl::openInputFileStream(VCLIB_RESULTS_PATH "/serialization/box3.bin");

    b2.deserialize(fi);
    fi.close();

    REQUIRE(b1 == b2);
}

TEST_CASE("Colors Serialization")
{
    std::ofstream fo = vcl::openOutputFileStream(VCLIB_RESULTS_PATH
                                                 "/serialization/color.bin");

    vcl::Color c1 = randomColor();
    vcl::Color c2 = randomColor();

    c1.serialize(fo);
    c2.serialize(fo);
    fo.close();

    vcl::Color c3;
    vcl::Color c4;

    std::ifstream fi =
        vcl::openInputFileStream(VCLIB_RESULTS_PATH "/serialization/color.bin");

    c3.deserialize(fi);
    c4.deserialize(fi);
    fi.close();

    REQUIRE(c1 == c3);
    REQUIRE(c2 == c4);
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

    std::ofstream fo = vcl::openOutputFileStream(VCLIB_RESULTS_PATH
                                                 "/serialization/bitset.bin");

    vcl::BitSet<T> bs1 = randomBitSet<T>();

    bs1.serialize(fo);
    fo.close();

    vcl::BitSet<T> bs2;

    std::ifstream fi = vcl::openInputFileStream(VCLIB_RESULTS_PATH
                                                "/serialization/bitset.bin");

    bs2.deserialize(fi);
    fi.close();

    REQUIRE(bs1 == bs2);
}

TEST_CASE("Vector serialization")
{
    vcl::Vector<vcl::Color, -1> vecColor1;
    vcl::Vector<double, -1> vecDouble;

    uint randSizeCol = GENERATE(take(1, random(1, 10)));
    uint randSizeDbl = GENERATE(take(1, random(1, 10)));
    for (uint i = 0; i < randSizeCol; i++)
        vecColor1.pushBack(randomColor());

    for (uint i = 0; i < randSizeDbl; i++)
        vecDouble.pushBack(GENERATE(take(1, random(0.0, 1.0))));

    std::ofstream fo = vcl::openOutputFileStream(
        VCLIB_RESULTS_PATH "/serialization/vectors.bin");
    vecColor1.serialize(fo);
    vecDouble.serialize(fo);
    fo.close();

    vcl::Vector<vcl::Color, -1> vecColor2;
    vcl::Vector<double, -1> vecDouble2;

    std::ifstream fi = vcl::openInputFileStream(
        VCLIB_RESULTS_PATH "/serialization/vectors.bin");
    vecColor2.deserialize(fi);
    vecDouble2.deserialize(fi);
    fi.close();

    REQUIRE(vecColor1.size() == vecColor2.size());
    REQUIRE(vecDouble.size() == vecDouble2.size());

    for (uint i = 0; i < vecColor1.size(); i++)
        REQUIRE(vecColor1[i] == vecColor2[i]);

    for (uint i = 0; i < vecDouble.size(); i++)
        REQUIRE(vecDouble[i] == vecDouble2[i]);
}

TEST_CASE("Array serialization")
{
    vcl::Array<double, 2> array2D1;
    vcl::Array<float, 3> array3D1;

    array2D1.resize(
        GENERATE(take(1, random(1, 10))), GENERATE(take(1, random(1, 10))));

    array3D1.resize(
        GENERATE(take(1, random(1, 10))),
        GENERATE(take(1, random(1, 10))),
        GENERATE(take(1, random(1, 10))));

    for (uint i = 0; i < array2D1.size(0); i++)
        for (uint j = 0; j < array2D1.size(1); j++)
            array2D1(i, j) = GENERATE(take(1, random(0.0f, 1.0f)));

    for (uint i = 0; i < array3D1.size(0); i++)
        for (uint j = 0; j < array3D1.size(1); j++)
            for (uint k = 0; k < array3D1.size(2); k++)
                array3D1(i, j, k) = GENERATE(take(1, random(0.0f, 1.0f)));

    std::ofstream fo = vcl::openOutputFileStream(
        VCLIB_RESULTS_PATH "/serialization/arrays.bin");
    array2D1.serialize(fo);
    array3D1.serialize(fo);
    fo.close();

    vcl::Array<double, 2> array2D2;
    vcl::Array<float, 3> array3D2;

    std::ifstream fi = vcl::openInputFileStream(
        VCLIB_RESULTS_PATH "/serialization/arrays.bin");
    array2D2.deserialize(fi);
    array3D2.deserialize(fi);
    fi.close();

    REQUIRE(array2D1.size(0) == array2D2.size(0));
    REQUIRE(array2D1.size(1) == array2D2.size(1));

    REQUIRE(array3D1.size(0) == array3D2.size(0));
    REQUIRE(array3D1.size(1) == array3D2.size(1));
    REQUIRE(array3D1.size(2) == array3D2.size(2));

    for (uint i = 0; i < array2D1.size(0); i++)
        for (uint j = 0; j < array2D1.size(1); j++)
            REQUIRE(array2D1(i, j) == array2D2(i, j));

    for (uint i = 0; i < array3D1.size(0); i++)
        for (uint j = 0; j < array3D1.size(1); j++)
            for (uint k = 0; k < array3D1.size(2); k++)
                REQUIRE(array3D1(i, j, k) == array3D2(i, j, k));

}

TEST_CASE("std vector of strings serialization") {
    std::vector<std::string> vecStr1;
    vecStr1.push_back("Hello");
    vecStr1.push_back("World");
    vecStr1.push_back("!");

    std::ofstream fo = vcl::openOutputFileStream(
        VCLIB_RESULTS_PATH "/serialization/vecStr.bin");
    vcl::serialize(fo, vecStr1);
    fo.close();

    std::vector<std::string> vecStr2;
    std::ifstream fi = vcl::openInputFileStream(
        VCLIB_RESULTS_PATH "/serialization/vecStr.bin");
    vcl::deserialize(fi, vecStr2);
    fi.close();

    REQUIRE(vecStr1.size() == vecStr2.size());
    for (uint i = 0; i < vecStr1.size(); i++)
        REQUIRE(vecStr1[i] == vecStr2[i]);
}

