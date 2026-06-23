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

#ifndef VCL_ALGORITHMS_CORE_RANDOM_H
#define VCL_ALGORITHMS_CORE_RANDOM_H

#include <vclib/space/core.h>

#include <cassert>
#include <random>

namespace vcl {

/**
 * @brief Generate a random number with a specified distribution.
 *
 * This function generates a random number based on the provided distribution
 * configuration and random generator configuration. The distribution can be
 * specified as a pair of bounds for a uniform distribution or as a custom
 * function that takes a random generator and returns a random number.
 *
 * When distConf is std::monostate (default), the numeric type determines how
 * the distribution range is chosen:
 *   - Floating-point types (float, double): uniform in [0.0, 1.0)
 *   - Unsigned integer types: uniform in [0, numeric_limits<T>::max()]
 *   - Signed integer types: uniform in [numeric_limits<T>::min(),
 *     numeric_limits<T>::max()], which includes negative values for two's
 *     complement representations (e.g., INT32_MIN to INT32_MAX).
 *
 * Example usage:
 * @code{.cpp}
 * // Integer with explicit bounds [0, 100]
 * int x = vcl::random<int>(std::pair{0, 100});
 * // Double with deterministic seed
 * double y = vcl::random<double>(std::pair{0.0, 1.0}, 42);
 * @endcode
 *
 * @tparam T: Numeric type for the random number. Must satisfy the Numeric concept.
 * @param[in] distConf: DistConfig<T> that determines how to provide the random
 * distribution.
 * @param[in] config: RandomConfig that determines how to provide the random
 * number generator.
 * @return A random number of type T based on the specified distribution.
 *
 * @ingroup algorithms_core
 */
template<Numeric T>
T random(
    DistConfig<T> distConf = std::monostate(),
    RandomConfig  config   = std::monostate())
{
    return callWithDistribution(distConf, [&](auto&& distFunc) {
        return callWithRandomGenerator(config, [&](std::mt19937& gen) {
            return distFunc(gen);
        });
    });
}

/**
 * @brief Generate a random point of a given PointType.
 *
 * This function generates a random point of the specified PointType based on
 * the provided distribution configuration and random generator configuration.
 * The distribution can be specified as a pair of bounds for a uniform
 * distribution or as a custom function that takes a random generator and
 * returns a random number.
 *
 * @tparam PointType: Type of the point to generate, must satisfy PointConcept.
 * @param[in] distConf: DistConfig that determines how to provide the random
 * distribution.
 * @param[in] config: RandomConfig that determines how to provide the random
 * number generator.
 * @return A random point of type PointType based on the specified distribution.
 *
 * @ingroup algorithms_core
 */
template<PointConcept PointType>
PointType random(
    DistConfig<typename PointType::ScalarType> distConf = std::monostate(),
    RandomConfig                               config   = std::monostate())
{
    return callWithDistribution(distConf, [&](auto&& distFunc) {
        return callWithRandomGenerator(config, [&](std::mt19937& gen) {
            PointType point;
            for (uint i = 0; i < PointType::DIM; ++i) {
                point[i] = distFunc(gen);
            }
            return point;
        });
    });
}

/**
 * @brief Generate a random box of a given BoxType.
 *
 * This function generates a random box of the specified BoxType based on the
 * provided distribution configuration and random generator configuration. The
 * distribution can be specified as a pair of bounds for a uniform distribution
 * or as a custom function that takes a random generator and returns a random
 * number.
 *
 * Generated boxes will have their min and max points randomly generated,
 * ensuring that the min point is less than or equal to the max point for each
 * dimension.
 *
 * @tparam BoxType: Type of the box to generate, must satisfy BoxConcept.
 * @param[in] distConf: DistConfig that determines how to provide the random
 * distribution.
 * @param[in] config: RandomConfig that determines how to provide the random
 * number generator.
 * @return A random box of type BoxType based on the specified distribution.
 *
 * @ingroup algorithms_core
 */
template<BoxConcept BoxType>
BoxType random(
    DistConfig<typename BoxType::ScalarType> distConf = std::monostate(),
    RandomConfig                             config   = std::monostate())
{
    using PointType  = BoxType::PointType;

    return callWithDistribution(distConf, [&](auto&& distFunc) {
        return callWithRandomGenerator(config, [&](std::mt19937& gen) {
            PointType minPoint, maxPoint;
            for (uint i = 0; i < PointType::DIM; ++i) {
                minPoint[i] = distFunc(gen);
                maxPoint[i] = distFunc(gen);
                if (minPoint[i] > maxPoint[i]) {
                    std::swap(minPoint[i], maxPoint[i]);
                }
            }
            return BoxType(minPoint, maxPoint);
        });
    });
}

/**
 * @brief Generate a random plane of a given PlaneType.
 *
 * This function generates a random plane of the specified PlaneType based on
 * the provided distribution configuration and random generator configuration.
 * The distribution can be specified as a pair of bounds for a uniform
 * distribution or as a custom function that takes a random generator and
 * returns a random number.
 *
 * @tparam PlaneType: Type of the plane to generate, must satisfy PlaneConcept.
 * @param[in] distConf: DistConfig that determines how to provide the random
 * distribution.
 * @param[in] config: RandomConfig that determines how to provide the random
 * number generator.
 * @return A random plane of type PlaneType based on the specified distribution.
 *
 * @ingroup algorithms_core
 */
template<PlaneConcept PlaneType>
PlaneType random(
    DistConfig<typename PlaneType::ScalarType> distConf = std::monostate(),
    RandomConfig                               config   = std::monostate())
{
    using ScalarType = PlaneType::ScalarType;
    using PointType  = PlaneType::PointType;

    return callWithDistribution(distConf, [&](auto&& distFunc) {
        return callWithRandomGenerator(config, [&](std::mt19937& gen) {
            return PlaneType(
                random<PointType>(distConf, config),
                random<ScalarType>(distConf, config));
        });
    });
}

/**
 * @brief Generate a random bitset of a given BitSetType.
 *
 * This function generates a random bitset of the specified BitSetType based on
 * the provided distribution configuration and random generator configuration.
 * The distribution can be specified as a pair of bounds for a uniform
 * distribution or as a custom function that takes a random generator and
 * returns a random number.
 *
 * The function performs a single random draw from the specified distribution
 * and sets the underlying value of the bitset accordingly. The resulting bitset
 * will have its bits set based on the random value generated.
 *
 * @tparam BitSetType: Type of the bitset to generate, must satisfy
 * BitSetConcept.
 * @param[in] distConf: DistConfig that determines how to provide the random
 * distribution.
 * @param[in] config: RandomConfig that determines how to provide the random
 * number generator.
 * @return A random bitset of type BitSetType based on the specified
 * distribution.
 *
 * @ingroup algorithms_core
 */
template<BitSetConcept BitSetType>
BitSetType random(
    DistConfig<typename BitSetType::UnderlyingType> distConf = std::monostate(),
    RandomConfig                                    config   = std::monostate())
{
    return callWithDistribution(distConf, [&](auto&& distFunc) {
        return callWithRandomGenerator(config, [&](std::mt19937& gen) {
            BitSetType bitset;
            bitset.setUnderlying(distFunc(gen));
            return bitset;
        });
    });
}

/**
 * @brief Generate the barycentric coordinates of a random point over a
 * triangle, with a uniform distribution over the triangle.
 *
 * It uses the parallelogram folding trick.
 *
 * @tparam PointType: Type of the point to generate. It must satisfy the
 * Point3Concept. The ScalarType is used as the distribution numeric type.
 * @param[in] config: RandomConfig that determines how to provide the random
 * number generator.
 * @return A Point3 of type PointType whose components are normalized
 * barycentric coordinates (they sum to 1 and each component is in [0, 1]).
 *
 * @ingroup algorithms_core
 */
template<Point3Concept PointType>
PointType randomTriangleBarycentricCoordinate(
    RandomConfig config = std::monostate())
{
    using ScalarType = PointType::ScalarType;

    return callWithRandomGenerator(config, [&](std::mt19937& gen) {
        PointType                                  interp;
        std::uniform_real_distribution<ScalarType> unif(0, 1);

        interp[1] = unif(gen);
        interp[2] = unif(gen);
        if (interp[1] + interp[2] > 1.0) {
            interp[1] = 1.0 - interp[1];
            interp[2] = 1.0 - interp[2];
        }

        interp[0] = 1.0 - (interp[1] + interp[2]);
        return interp;
    });
}

/**
 * @brief Generate barycentric coordinates of a random point over a regular
 * polygon in 2D, with uniform distribution over its vertices.
 *
 * The algorithm generates N independent and identically distributed random
 * values, then normalizes them so they sum to 1. This produces a point
 * uniformly distributed on the (N-1)-simplex — equivalent to a random convex
 * combination of the polygon vertices.
 *
 * For triangles (polySize == 3), prefer @ref
 * randomTriangleBarycentricCoordinate which uses the exact parallelogram
 * folding trick and may offer better control over distribution bounds.
 *
 * @tparam ScalarType: Numeric type for the barycentric coordinates.
 * @param[in] polySize: Number of vertices in the polygon. Must be greater than
 * 0.
 * @param[in] config: RandomConfig that determines how to provide the random
 * number generator.
 * @return A std::vector<ScalarType> of size polySize with values in [0, 1) that
 * sum to exactly 1 (within floating-point precision). The i-th element is the
 * weight for the i-th vertex of the polygon.
 *
 * @note For perfect uniformity on triangles, the barycentric coordinate method
 * used by randomTriangleBarycentricCoordinate is exact. This function uses
 * normalization which is an approximation that becomes arbitrarily close to
 * uniform as polySize increases.
 *
 * @ingroup algorithms_core
 */
template<typename ScalarType>
std::vector<ScalarType> randomPolygonBarycentricCoordinate(
    uint                               polySize,
    RandomConfig                       config   = std::monostate())
{
    return callWithRandomGenerator(config, [&](std::mt19937& gen) {
        std::vector<ScalarType> barCoord(polySize);
        ScalarType              sum = static_cast<ScalarType>(0);

        std::uniform_real_distribution<ScalarType> unif(0, 1);

        for (uint i = 0; i < polySize; ++i) {
            barCoord[i] = unif(gen);
            sum += barCoord[i];
        }

        // Normalize so all weights sum to 1.
        assert(sum != static_cast<ScalarType>(0));
        for (uint i = 0; i < polySize; ++i) {
            barCoord[i] /= sum;
        }
        return barCoord;
    });
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_RANDOM_H
