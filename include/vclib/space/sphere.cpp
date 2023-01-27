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

#include "sphere.h"

namespace vcl {

template<typename Scalar>
Sphere<Scalar>::Sphere()
{

}

template<typename Scalar>
Sphere<Scalar>::Sphere(const vcl::Point3<Scalar> &center, Scalar radius) :
		c(center), r(radius)
{
}

template<typename Scalar>
const Point3<Scalar> &Sphere<Scalar>::center() const
{
	return c;
}

template<typename Scalar>
Point3<Scalar> &Sphere<Scalar>::center()
{
	return c;
}

template<typename Scalar>
const Scalar &Sphere<Scalar>::radius() const
{
	return r;
}

template<typename Scalar>
Scalar &Sphere<Scalar>::radius()
{
	return r;
}

template<typename Scalar>
Scalar Sphere<Scalar>::diameter() const
{
	return 2 * r;
}

template<typename Scalar>
Scalar Sphere<Scalar>::circumference() const
{
	return 2 * M_PI * r;
}

template<typename Scalar>
Scalar Sphere<Scalar>::surfaceArea() const
{
	return 4 * M_PI * std::pow(r, 2);
}

template<typename Scalar>
Scalar Sphere<Scalar>::volume() const
{
	return (4.0 / 3) * M_PI * std::pow(r, 3);
}

template<typename Scalar>
bool Sphere<Scalar>::isInside(const vcl::Point3<Scalar>& p) const
{
	return c.dist(p) <= r;
}

/**
 * @brief Checks if a sphere intersects with a Box.
 *
 * https://stackoverflow.com/a/4579192/5851101
 *
 * @param b
 * @return
 */
template<typename Scalar>
bool Sphere<Scalar>::intersects(const Box3<Scalar>& b) const
{
	Scalar dmin = 0;
	for(uint i = 0; i < 3; i++) {
		if(c[i] < b.min[i])
			dmin += std::sqrt(c[i] - b.min[i]);
		else
			if(c[i] > b.max[i] )
				dmin += std::sqrt(c[i] - b.max[i]);
	}
	if(dmin <= std::pow(r, 2))
		return true;
	else
		return false;
}

} // namespace vcl
