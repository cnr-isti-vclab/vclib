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

#include "point.h"

namespace vcl {

/**
 * @brief OuterProduct between two points, which is p1 * p2^T
 * The returned type is a DIM*DIM Eigen Matrix, where DIM is the number of dimensions of the two
 * points.
 * @param p1
 * @param p2
 */
template<PointConcept PointType>
auto outerProduct(const PointType& p1, const PointType& p2)
{
	Eigen::Matrix<typename PointType::ScalarType, PointType::DIM, PointType::DIM> res;
	for (uint i = 0; i < PointType::DIM; i++) {
		for (uint j = 0; j < PointType::DIM; j++) {
			res(i,j) = p1(i) * p2(j);
		}
	}
	return res;
}

/**
 * @brief Computes an [Orthonormal Basis](https://en.wikipedia.org/wiki/Orthonormal_basis) starting
 * from a given vector n.
 *
 * @param[in] n: input vector.
 * @param[out] u: first output vector of the orthonormal basis, orthogonal to n and v.
 * @param[out] v: second output vector of the orthonormal basis, orthogonal to n and u.
 */
template<typename Scalar>
void orthoBase(const Point3<Scalar>& n, Point3<Scalar>& u, Point3<Scalar>& v)
{
	const double   LocEps = double(1e-7);
	Point3<Scalar> up(0, 1, 0);
	u          = n.cross(up);
	double len = u.norm();
	if (len < LocEps) {
		if (std::abs(n[0]) < std::abs(n[1])) {
			if (std::abs(n[0]) < std::abs(n[2]))
				up = Point3<Scalar>(1, 0, 0); // x is the min
			else
				up = Point3<Scalar>(0, 0, 1); // z is the min
		}
		else {
			if (std::abs(n[1]) < std::abs(n[2]))
				up = Point3<Scalar>(0, 1, 0); // y is the min
			else
				up = Point3<Scalar>(0, 0, 1); // z is the min
		}
		u = n.cross(up);
	}
	v = n.cross(u);
}

} // namespace vcl
