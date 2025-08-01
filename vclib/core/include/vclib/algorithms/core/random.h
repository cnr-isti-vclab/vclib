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
 * @param L
 * @param gen
 * @return
 *
 * @ingroup algorithms_core
 */
inline int poissonRatioOfUniformsInteger(double L, std::mt19937& gen)
{
    // constants
    const double SHAT1 = 2.943035529371538573;  // 8/e
    const double SHAT2 = 0.8989161620588987408; // 3-sqrt(12/e)

    double u;  // uniform random
    double lf; // ln(f(x))
    double x;  // real sample
    int    k;  // integer sample

    double pois_a     = L + 0.5; // hat center
    int    mode       = (int) L; // mode
    double pois_g     = std::log(L);
    double pois_f0    = mode * pois_g - lnOfFactorial(mode);  // value at mode
    double pois_h     = std::sqrt(SHAT1 * (L + 0.5)) + SHAT2; // hat width
    double pois_bound = (int) (pois_a + 6.0 * pois_h);        // safety-bound

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
}

inline int poissonRatioOfUniformsInteger(double L)
{
    static std::random_device rd;
    static std::mt19937       gen(rd());
    return poissonRatioOfUniformsInteger(L, gen);
}

} // namespace detail


/**
 * @brief algorithm poisson random number (Knuth):
 * init:
 *   Let L ← e^−λ, k ← 0 and p ← 1.
 *   do:
 *     k ← k + 1.
 *     Generate uniform random number u in [0,1] and let p ← p × u.
 *   while p > L.
 *   return k − 1.
 * @param lambda
 * @param gen
 * @return
 *
 * @ingroup algorithms_core
 */
inline int poissonRandomNumber(double lambda, std::mt19937& gen)
{
    if (lambda > 50)
        return detail::poissonRatioOfUniformsInteger(lambda, gen);

    std::uniform_real_distribution<double> unif(0, 1);

    double L = exp(-lambda);
    int    k = 0;
    double p = 1.0;
    do {
        k = k + 1;
        p = p * unif(gen);
    } while (p > L);

    return k - 1;
}

inline int poissonRandomNumber(double lambda)
{
    static std::random_device rd;
    static std::mt19937       gen(rd());
    return detail::poissonRatioOfUniformsInteger(lambda, gen);
}

/**
 * @brief Generate the barycentric coords of a random point over a triangle,
 * with a uniform distribution over the triangle.
 * It uses the parallelogram folding trick.
 *
 * @param gen
 * @return
 *
 * @ingroup algorithms_core
 */
template<Point3Concept PointType>
PointType randomTriangleBarycentricCoordinate(std::mt19937& gen)
{
    using ScalarType = PointType::ScalarType;

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
}

template<Point3Concept PointType>
PointType randomTriangleBarycentricCoordinate()
{
    static std::random_device rd;
    static std::mt19937       gen(rd());
    return randomTriangleBarycentricCoordinate<PointType>(gen);
}

template<typename ScalarType>
std::vector<ScalarType> randomPolygonBarycentricCoordinate(
    uint          polySize,
    std::mt19937& gen)
{
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
}

template<typename ScalarType>
std::vector<ScalarType> randomPolygonBarycentricCoordinate(uint polySize)
{
    static std::random_device rd;
    static std::mt19937       gen(rd());
    return randomPolygonBarycentricCoordinate<ScalarType>(polySize, gen);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_RANDOM_H
