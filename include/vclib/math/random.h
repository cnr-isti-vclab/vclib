/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
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

#ifndef VCL_MATH_RANDOM_H
#define VCL_MATH_RANDOM_H

#include <random>

#include <vclib/space/point.h>

namespace vcl {

int poissonRatioOfUniformsInteger(double L, std::mt19937& gen);

int poissonRatioOfUniformsInteger(double L);

template<typename ScalarType>
vcl::Point3<ScalarType> randomTriangleBarycentricCoordinate(std::mt19937& gen);

template<typename ScalarType>
vcl::Point3<ScalarType> randomTriangleBarycentricCoordinate();

template<typename ScalarType>
std::vector<ScalarType> randomPolygonBarycentricCoordinate(uint polySize, std::mt19937& gen);

template<typename ScalarType>
std::vector<ScalarType> randomPolygonBarycentricCoordinate(uint polySize);

} // namespace vcl

#include "random.cpp"

#endif // VCL_MATH_RANDOM_H
