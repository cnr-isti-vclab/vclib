/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_MATH_FIBONACCI_H
#define VCL_MATH_FIBONACCI_H

#include <vclib/space/point.h>

namespace vcl {

namespace internal {

template<Point3Concept PointType>
PointType sphericalFibonacciPoint(uint i, uint n)
{
	using ScalarType = PointType::ScalarType;

	const ScalarType Phi      = ScalarType(std::sqrt(ScalarType(5)) * 0.5 + 0.5);
	const ScalarType phi      = 2.0 * M_PI * (i / Phi - std::floor(i / Phi));
	ScalarType       cosTheta = 1.0 - (2 * i + 1.0) / ScalarType(n);
	ScalarType       sinTheta = 1 - cosTheta * cosTheta;
	sinTheta = std::sqrt(std::min(ScalarType(1), std::max(ScalarType(0), sinTheta)));
	return PointType(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
}

} // namespace vcl::internal

/**
 * @brief Returns a vector of `n` points distributed in a unit sphere.
 *
 * This function returns a vector of `n` points that are uniformly distributed on a unit sphere,
 * using the Spherical Fibonacci Point Sets algorithm described in the paper "Spherical Fibonacci
 * Mapping" by Benjamin Keinert, Matthias Innmann, Michael Sanger, and Marc Stamminger (TOG 2015).
 *
 * @tparam PointType The type of the point to generate. This type must satisfy the `Point3Concept`
 * concept.
 *
 * @param[in] n: The number of points to generate.
 * @return A vector of `n` points distributed in a unit sphere.
 */
template<Point3Concept PointType>
std::vector<PointType> sphericalFibonacciPointSet(uint n)
{
	using ScalarType = PointType::ScalarType;

	std::vector<PointType> v(n);
	for (uint i = 0; i < n; ++i)
		v[i] = internal::sphericalFibonacciPoint<PointType>(i, n);

	return v;
}

} // namespace vcl

#endif // VCL_MATH_FIBONACCI_H
