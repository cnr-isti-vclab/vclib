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

#include <random>

namespace vcl {

namespace detail {

/**
 * @brief This subfunction generates a integer with the poisson distribution
 * using the ratio-of-uniforms rejection method (PRUAt). This approach is STABLE
 * even for large L (e.g. it does not suffer from the overflow limit of the
 * classical Knuth implementation) Execution time does not depend on L, except
 * that it matters whether is within the range where ln(n!) is tabulated.
 *
 * Reference:
 *
 * E. Stadlober
 * "The ratio of uniforms approach for generating discrete random variates".
 * Journal of Computational and Applied Mathematics,
 * vol. 31, no. 1, 1990, pp. 181-189.
 *
 * Partially adapted/inspired from some subfunctions of the Agner Fog stocc
 * library ( www.agner.org/random ) Same licensing scheme.
 *
 * @param[in] L
 * @param[in] config: RandomConfig that determines how to provide the random
 * number generator.
 * @return
 *
 * @ingroup algorithms_core
 */
inline int poissonRatioOfUniformsInteger(
    double       L,
    RandomConfig config = std::monostate())
{
    return callWithRandomGenerator(config, [&](std::mt19937& gen) {
        // constants
        const double SHAT1 = 2.943035529371538573;  // 8/e
        const double SHAT2 = 0.8989161620588987408; // 3-sqrt(12/e)

        double u;  // uniform random
        double lf; // ln(f(x))
        double x;  // real sample
        int    k;  // integer sample

        double pois_a  = L + 0.5; // hat center
        int    mode    = (int) L; // mode
        double pois_g  = std::log(L);
        double pois_f0 = mode * pois_g - lnOfFactorial(mode);  // value at mode
        double pois_h  = std::sqrt(SHAT1 * (L + 0.5)) + SHAT2; // hat width
        double pois_bound = (int) (pois_a + 6.0 * pois_h);     // safety-bound

        std::uniform_real_distribution<double> unif(0, 1);

        while (1) {
            u = unif(gen);
            if (u == 0)
                continue; // avoid division by 0
            x = pois_a + pois_h * (unif(gen) - 0.5) / u;
            if (x < 0 || x >= pois_bound)
                continue; // reject if outside valid range
            k  = (int) (x);
            lf = k * pois_g - lnOfFactorial(k) - pois_f0;
            if (lf >= u * (4.0 - u) - 3.0)
                break; // quick acceptance
            if (u * (u - lf) > 1.0)
                continue; // quick rejection
            if (2.0 * log(u) <= lf)
                break; // final acceptance
        }
        return k;
    });
}

} // namespace detail

/**
 * @brief Generate a random number with a specified distribution.
 *
 * This function generates a random number based on the provided distribution
 * configuration and random generator configuration. The distribution can be
 * specified as a pair of bounds for a uniform distribution or as a custom
 * function that takes a random generator and returns a random number.
 *
 * @tparam T: Numeric type for the random number.
 * @param[in] distConf: Distribution configuration, which can be:
 *   - std::monostate: Use default uniform distribution based on type T.
 *   - std::pair<T, T>: Specify bounds for a uniform distribution.
 *   - std::function<T(std::mt19937&)>: Custom distribution function.
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
 * @param[in] distConf: Distribution configuration, which can be:
 *   - std::monostate: Use default uniform distribution based on PointType's
 *     ScalarType.
 *   - std::pair<PointType::ScalarType, PointType::ScalarType>: Specify bounds
 *     for a uniform distribution.
 *   - std::function<PointType::ScalarType(std::mt19937&)>: Custom distribution
 *     function.
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
    using ScalarType = typename PointType::ScalarType;

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
 * @brief Generate the barycentric coords of a random point over a triangle,
 * with a uniform distribution over the triangle.
 * It uses the parallelogram folding trick.
 *
 * @param[in] config: RandomConfig that determines how to provide the random
 * number generator.
 * @return
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

template<typename ScalarType>
std::vector<ScalarType> randomPolygonBarycentricCoordinate(
    uint         polySize,
    RandomConfig config = std::monostate())
{
    return callWithRandomGenerator(config, [&](std::mt19937& gen) {
        std::vector<ScalarType> barCoord(polySize);
        ScalarType              sum = 0;

        std::uniform_real_distribution<ScalarType> unif(0, 100);

        for (uint i = 0; i < polySize; i++) {
            barCoord[i] = unif(gen);
            sum += barCoord[i];
        }
        for (uint i = 0; i < polySize; i++) {
            barCoord[i] /= sum;
        }
        return barCoord;
    });
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_RANDOM_H
