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
#include <vclib/io/read.h>
#include <vclib/io/write.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>
#include <vclib/space.h>

#include <random>

template<typename Scalar>
using DistrType = std::conditional_t<
    std::is_integral_v<Scalar>,
    std::uniform_int_distribution<Scalar>,
    std::uniform_real_distribution<Scalar>>;

template<typename Scalar, unsigned int N>
vcl::Point<Scalar, N> randomPoint()
{
    std::random_device rd;
    std::mt19937       gen(rd());

    DistrType<Scalar> dis((Scalar) -100, (Scalar) 100);

    vcl::Point<Scalar, N> p;
    for (unsigned int i = 0; i < N; i++)
        p[i] = dis(gen);
    return p;
}

template<typename Scalar, unsigned int N>
vcl::Box<vcl::Point<Scalar, N>> randomBox()
{
    vcl::Box<vcl::Point<Scalar, N>> b(
        randomPoint<Scalar, N>(), randomPoint<Scalar, N>());
    return b;
}

vcl::Color randomColor()
{
    // generate random color using std::mt19937

    std::mt19937                                gen;
    std::uniform_int_distribution<unsigned int> dist(0, 255);
    return vcl::Color(dist(gen), dist(gen), dist(gen), dist(gen));
}

template<std::integral T>
vcl::BitSet<T> randomBitSet()
{
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    vcl::BitSet<T> bs;
    for (unsigned int i = 0; i < bs.size(); i++)
        bs.set(i, dis(gen));
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

    vcl::serialize(fo, c1, c2);
    fo.close();

    vcl::Color c3;
    vcl::Color c4;

    std::ifstream fi =
        vcl::openInputFileStream(VCLIB_RESULTS_PATH "/serialization/color.bin");

    vcl::deserialize(fi, c3, c4);
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
    vcl::Vector<double, -1>     vecDouble1;

    std::random_device                          rd;
    std::mt19937                                gen(rd());
    std::uniform_int_distribution<unsigned int> distInt(1, 10);
    std::uniform_real_distribution<double>      distDouble(0.0, 1.0);

    unsigned int randSizeCol = distInt(gen);
    unsigned int randSizeDbl = distInt(gen);
    for (unsigned int i = 0; i < randSizeCol; i++)
        vecColor1.pushBack(randomColor());

    for (unsigned int i = 0; i < randSizeDbl; i++)
        vecDouble1.pushBack(distDouble(gen));

    std::ofstream fo = vcl::openOutputFileStream(VCLIB_RESULTS_PATH
                                                 "/serialization/vectors.bin");

    vcl::serialize(fo, vecColor1, vecDouble1);
    fo.close();

    vcl::Vector<vcl::Color, -1> vecColor2;
    vcl::Vector<double, -1>     vecDouble2;

    std::ifstream fi = vcl::openInputFileStream(VCLIB_RESULTS_PATH
                                                "/serialization/vectors.bin");
    vcl::deserialize(fi, vecColor2, vecDouble2);
    fi.close();

    REQUIRE(vecColor1.size() == vecColor2.size());
    REQUIRE(vecDouble1.size() == vecDouble2.size());

    for (unsigned int i = 0; i < vecColor1.size(); i++)
        REQUIRE(vecColor1[i] == vecColor2[i]);

    for (unsigned int i = 0; i < vecDouble1.size(); i++)
        REQUIRE(vecDouble1[i] == vecDouble2[i]);
}

TEST_CASE("Array serialization")
{
    vcl::Array<double, 2> array2D1;
    vcl::Array<float, 3>  array3D1;

    std::random_device                          rd;
    std::mt19937                                gen(rd());
    std::uniform_int_distribution<unsigned int> distInt(1, 10);
    std::uniform_real_distribution<double>      distFloat(0.0, 1.0);
    std::uniform_real_distribution<double>      distDouble(0.0, 1.0);

    array2D1.resize(distInt(gen), distInt(gen));

    array3D1.resize(distInt(gen), distInt(gen), distInt(gen));

    for (unsigned int i = 0; i < array2D1.size(0); i++)
        for (unsigned int j = 0; j < array2D1.size(1); j++)
            array2D1(i, j) = distDouble(gen);

    for (unsigned int i = 0; i < array3D1.size(0); i++)
        for (unsigned int j = 0; j < array3D1.size(1); j++)
            for (unsigned int k = 0; k < array3D1.size(2); k++)
                array3D1(i, j, k) = distFloat(gen);

    std::ofstream fo = vcl::openOutputFileStream(VCLIB_RESULTS_PATH
                                                 "/serialization/arrays.bin");
    array2D1.serialize(fo);
    array3D1.serialize(fo);
    fo.close();

    vcl::Array<double, 2> array2D2;
    vcl::Array<float, 3>  array3D2;

    std::ifstream fi = vcl::openInputFileStream(VCLIB_RESULTS_PATH
                                                "/serialization/arrays.bin");
    array2D2.deserialize(fi);
    array3D2.deserialize(fi);
    fi.close();

    REQUIRE(array2D1.size(0) == array2D2.size(0));
    REQUIRE(array2D1.size(1) == array2D2.size(1));

    REQUIRE(array3D1.size(0) == array3D2.size(0));
    REQUIRE(array3D1.size(1) == array3D2.size(1));
    REQUIRE(array3D1.size(2) == array3D2.size(2));

    for (unsigned int i = 0; i < array2D1.size(0); i++)
        for (unsigned int j = 0; j < array2D1.size(1); j++)
            REQUIRE(array2D1(i, j) == array2D2(i, j));

    for (unsigned int i = 0; i < array3D1.size(0); i++)
        for (unsigned int j = 0; j < array3D1.size(1); j++)
            for (unsigned int k = 0; k < array3D1.size(2); k++)
                REQUIRE(array3D1(i, j, k) == array3D2(i, j, k));
}

TEST_CASE("std vector of strings serialization")
{
    std::vector<std::string> vecStr1;
    vecStr1.push_back("Hello");
    vecStr1.push_back("World");
    vecStr1.push_back("!");

    std::ofstream fo = vcl::openOutputFileStream(VCLIB_RESULTS_PATH
                                                 "/serialization/vecStr.bin");
    vcl::serialize(fo, vecStr1);
    fo.close();

    std::vector<std::string> vecStr2;
    std::ifstream            fi = vcl::openInputFileStream(VCLIB_RESULTS_PATH
                                                "/serialization/vecStr.bin");
    vcl::deserialize(fi, vecStr2);
    fi.close();

    REQUIRE(vecStr1.size() == vecStr2.size());
    for (unsigned int i = 0; i < vecStr1.size(); i++)
        REQUIRE(vecStr1[i] == vecStr2[i]);
}

TEMPLATE_TEST_CASE("Matrix serialization", "", int, float, double)
{
    using Scalar = TestType;

    std::random_device rd;
    std::mt19937       gen(rd());

    DistrType<Scalar> dis(0.0, 1.0);

    vcl::Matrix<Scalar, 2, 2> mat1;

    for (unsigned int i = 0; i < 2; i++)
        for (unsigned int j = 0; j < 2; j++)
            mat1(i, j) = dis(gen);

    std::ofstream fo =
        vcl::openOutputFileStream(VCLIB_RESULTS_PATH "/serialization/mat.bin");
    mat1.serialize(fo);
    fo.close();

    vcl::Matrix<Scalar, 2, 2> mat2;
    std::ifstream             fi =
        vcl::openInputFileStream(VCLIB_RESULTS_PATH "/serialization/mat.bin");
    mat2.deserialize(fi);
    fi.close();

    for (unsigned int i = 0; i < 2; i++)
        for (unsigned int j = 0; j < 2; j++)
            REQUIRE(mat1(i, j) == mat2(i, j));
}

TEMPLATE_TEST_CASE("Mesh serialization", "", vcl::PolyMesh, vcl::TriMesh)
{
    using Mesh = TestType;

    Mesh mesh1 = vcl::load<Mesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");

    mesh1.enablePerVertexColor();
    for (unsigned int i = 0; i < mesh1.vertexNumber(); i++)
        mesh1.vertex(i).color() = randomColor();

    std::ofstream fo =
        vcl::openOutputFileStream(VCLIB_RESULTS_PATH "/serialization/mesh.bin");
    mesh1.serialize(fo);
    fo.close();

    Mesh          mesh2;
    std::ifstream fi =
        vcl::openInputFileStream(VCLIB_RESULTS_PATH "/serialization/mesh.bin");
    mesh2.deserialize(fi);
    fi.close();

    REQUIRE(mesh1.vertexNumber() == mesh2.vertexNumber());
    REQUIRE(mesh1.faceNumber() == mesh2.faceNumber());
    REQUIRE(mesh2.isPerVertexColorEnabled());

    for (unsigned int i = 0; i < mesh1.vertexNumber(); i++) {
        REQUIRE(mesh1.vertex(i).coord() == mesh2.vertex(i).coord());
        REQUIRE(mesh1.vertex(i).color() == mesh2.vertex(i).color());
    }

    for (unsigned int i = 0; i < mesh1.faceNumber(); i++) {
        REQUIRE(mesh1.face(i).vertexNumber() == mesh2.face(i).vertexNumber());
        for (unsigned int j = 0; j < mesh1.face(i).vertexNumber(); j++)
            REQUIRE(
                mesh1.face(i).vertexIndex(j) == mesh2.face(i).vertexIndex(j));
    }
}
