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

#include <vclib/concepts.h>

#include <random>

namespace vcl {

/**
 * @brief Generate the barycentric coords of a random point over a triangle,
 * with a uniform distribution over the triangle.
 * It uses the parallelogram folding trick.
 *
 * @param gen
 * @return
 *
 * @ingroup math
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
