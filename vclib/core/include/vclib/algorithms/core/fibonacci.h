// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_CORE_FIBONACCI_H
#define VCL_ALGORITHMS_CORE_FIBONACCI_H

#include <vclib/space/core.h>

#include <cmath>
#include <numbers>

namespace vcl {

namespace detail {

template<Point3Concept PointType>
PointType sphericalFibonacciPoint(uint i, uint n)
{
    using ScalarType = PointType::ScalarType;

    constexpr ScalarType PI = std::numbers::pi_v<ScalarType>;

    const ScalarType Phi = ScalarType(std::sqrt(ScalarType(5)) * 0.5 + 0.5);
    const ScalarType phi = 2.0 * PI * (i / Phi - std::floor(i / Phi));
    ScalarType       cosTheta = 1.0 - (2 * i + 1.0) / ScalarType(n);
    ScalarType       sinTheta = 1 - cosTheta * cosTheta;
    sinTheta =
        std::sqrt(std::min(ScalarType(1), std::max(ScalarType(0), sinTheta)));
    return PointType(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
}

} // namespace detail

/**
 * @brief Returns a vector of `n` points distributed in a unit sphere.
 *
 * This function returns a vector of `n` points that are uniformly distributed
 * on a unit sphere, using the Spherical Fibonacci Point Sets algorithm
 * described in the paper "Spherical Fibonacci Mapping" by Benjamin Keinert,
 * Matthias Innmann, Michael Sanger, and Marc Stamminger (TOG 2015).
 *
 * @tparam PointType The type of the point to generate. This type must satisfy
 * the `Point3Concept` concept.
 *
 * @param[in] n: The number of points to generate.
 * @return A vector of `n` points distributed in a unit sphere.
 *
 * @ingroup algorithms_core
 */
template<Point3Concept PointType>
std::vector<PointType> sphericalFibonacciPointSet(uint n)
{
    using ScalarType = PointType::ScalarType;

    std::vector<PointType> v(n);
    for (uint i = 0; i < n; ++i)
        v[i] = detail::sphericalFibonacciPoint<PointType>(i, n);

    return v;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_FIBONACCI_H
