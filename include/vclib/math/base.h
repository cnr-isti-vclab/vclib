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

#ifndef VCL_MATH_BASE_H
#define VCL_MATH_BASE_H

#include <cassert>
#include <cmath>
#include <limits>

#include <vclib/types.h>

namespace vcl {

template<typename Scalar>
bool isDegenerate(Scalar t)
{
    return std::isinf(t) || std::isnan(t);
}

template<typename Scalar>
bool epsilonEquals(
    Scalar p1,
    Scalar p2,
    Scalar epsilon = std::numeric_limits<Scalar>::epsilon())
{
    return (std::abs(p1 - p2) <= epsilon);
}

template<typename Scalar>
Scalar toRad(const Scalar& a)
{
    return M_PI * a / 180.0;
}

template<typename Scalar>
Scalar toDeg(const Scalar& a)
{
    return a * 180.0 / M_PI;
}

/**
 * @brief Computes and caches the result of the natural logarithm of n!
 *
 * If n > 1024, uses the Stirling approximation.
 * @param n
 * @return
 */
inline double lnOfFactorial(int n)
{
    static const int FAK_LEN = 1024;

    // define constants
    static const double            // coefficients in Stirling approximation
        C0 = 0.918938533204672722, // ln(sqrt(2*pi))
        C1 = 1. / 12., C3 = -1. / 360.;
    // C5 =  1./1260.,                  // use r^5 term if FAK_LEN < 50
    // C7 = -1./1680.;                  // use r^7 term if FAK_LEN < 20
    // static variables
    static double fac_table[FAK_LEN]; // table of ln(n!):
    // remember if fac_table has been initialized
    static bool initialized = false;

    if (n < FAK_LEN) {
        if (n <= 1) {
            if (n < 0)
                assert(0); //("Parameter negative in LnFac function");
            return 0;
        }
        if (!initialized) { // first time. Must initialize table
            // make table of ln(n!)
            double sum = fac_table[0] = 0.;
            for (uint i = 1; i < FAK_LEN; i++) {
                sum += std::log(double(i));
                fac_table[i] = sum;
            }
            initialized = true;
        }
        return fac_table[n];
    }
    else {
        // not found in table. use Stirling approximation
        double n1, r;
        n1 = n;
        r  = 1. / n1;
        return (n1 + 0.5) * std::log(n1) - n1 + C0 + r * (C1 + r * r * C3);
    }
}

} // namespace vcl

#endif // VCL_MATH_BASE_H
