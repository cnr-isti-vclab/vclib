// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_TRANSFORM_H
#define TEST_TRANSFORM_H

#include "types.h"

#include <benchmark_runner.h>

#include <vclib/algorithms.h>
#include <vclib/space/core.h>

#include <cmath>

namespace bench {

inline vcl::Matrix44d createSampleTransformMatrix()
{
    vcl::Matrix44d m = vcl::Matrix44d::Identity();
    vcl::Point3d   axis(1.0, 1.0, 1.0);
    axis.normalize();

    constexpr double pi    = 3.14159265358979323846;
    double           angle = 30.0 * pi / 180.0;
    double           c     = std::cos(angle);
    double           s     = std::sin(angle);
    double           q     = 1.0 - c;

    m(0, 0) = axis[0] * axis[0] * q + c;
    m(0, 1) = axis[0] * axis[1] * q - axis[2] * s;
    m(0, 2) = axis[0] * axis[2] * q + axis[1] * s;
    m(0, 3) = 10.0;

    m(1, 0) = axis[1] * axis[0] * q + axis[2] * s;
    m(1, 1) = axis[1] * axis[1] * q + c;
    m(1, 2) = axis[1] * axis[2] * q - axis[0] * s;
    m(1, 3) = 20.0;

    m(2, 0) = axis[2] * axis[0] * q - axis[1] * s;
    m(2, 1) = axis[2] * axis[1] * q + axis[0] * s;
    m(2, 2) = axis[2] * axis[2] * q + c;
    m(2, 3) = 30.0;

    return m;
}

inline void benchmarkTransform(
    BenchmarkRunner& runner,
    Mesh&            m,
    int              iterations = -1)
{
    const vcl::Matrix44d mat = createSampleTransformMatrix();

    runner.run(
        "apply_transform_matrix",
        [&]() { vcl::applyTransformMatrix(m, mat, true); },
        iterations);
}

} // namespace bench

#endif // TEST_TRANSFORM_H
