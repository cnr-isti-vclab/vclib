/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

/**
 * @brief Generate the barycentric coords of a random point over a triangle,
 * with a uniform distribution over the triangle.
 * It uses the parallelogram folding trick.
 *
 * @param gen
 * @return
 */
template<typename ScalarType>
vcl::Point3<ScalarType> randomTriangleBarycentricCoordinate(std::mt19937& gen)
{
	vcl::Point3<ScalarType> interp;
	std::uniform_real_distribution<ScalarType> unif(0, 1);

	interp[1] = unif(gen);
	interp[2] = unif(gen);
	if(interp[1] + interp[2] > 1.0) {
		interp[1] = 1.0 - interp[1];
		interp[2] = 1.0 - interp[2];
	}

	interp[0] = 1.0 - (interp[1] + interp[2]);
	return interp;
}

template<typename ScalarType>
vcl::Point3<ScalarType> randomTriangleBarycentricCoordinate()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return randomTriangleBarycentricCoordinate<ScalarType>(gen);
}


} // namespace vcl

#endif // VCL_MATH_RANDOM_H
