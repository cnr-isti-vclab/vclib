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

#ifndef VCL_SPACE_SPHERE_H
#define VCL_SPACE_SPHERE_H

#include <vclib/concepts/space/sphere.h>

#include "box.h"

namespace vcl {

template<typename Scalar>
class Sphere
{
public:
	using ScalarType = Scalar;
	using PointType = vcl::Point3<Scalar>;

	Sphere();
	Sphere(const vcl::Point3<Scalar>& center, Scalar radius);

	const Point3<Scalar>& center() const;
	Point3<Scalar>& center();

	const Scalar& radius() const;
	Scalar& radius();

	template<typename S>
	Sphere<S> cast() const;

	Scalar diameter() const;
	Scalar circumference() const;
	Scalar surfaceArea() const;
	Scalar volume() const;

	bool isInside(const vcl::Point3<Scalar>& p) const;
	bool intersects(const Box3<Scalar>& b) const;

private:
	vcl::Point3<Scalar> c;
	Scalar r;
};

// deduction guides
template<Point3Concept P, typename T>
Sphere(P, T) -> Sphere<typename P::ScalarType>;

using Spheref = Sphere<float>;
using Sphered = Sphere<double>;

} // namespace vcl

#include "sphere.cpp"

#endif // VCL_SPACE_SPHERE_H
