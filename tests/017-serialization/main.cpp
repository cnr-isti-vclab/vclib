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

TEMPLATE_TEST_CASE("Point Serialization", "", int, float, double)
{
    using Scalar = TestType;

    std::ofstream fo = vcl::openOutputFileStream(
        VCLIB_RESULTS_PATH "/serialization/point3_serialization.bin");

    vcl::Point3<Scalar> p(
        GENERATE(take(1, random<Scalar>(-100, 100))),
        GENERATE(take(1, random<Scalar>(-100, 100))),
        GENERATE(take(1, random<Scalar>(-100, 100))));

    p.serialize(fo);
    fo.close();

    vcl::Point3<Scalar> p2;
    std::ifstream       fi = vcl::openInputFileStream(
        VCLIB_RESULTS_PATH "/serialization/point3_serialization.bin");

    p2.deserialize(fi);
    fi.close();

    REQUIRE(p == p2);
}
