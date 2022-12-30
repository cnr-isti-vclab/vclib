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

#ifndef VCL_SPACE_POINT_POINT4_H
#define VCL_SPACE_POINT_POINT4_H

#include "point_t.h"

namespace vcl {

/**
 * @brief The Point4 class represents a 4-dimensional point containing 4 ScalarType values.
 * Specializes the Point class, providing some additional member functions useful for 4D points.
 */
template<typename ScalarType>
class Point4 : public Point<ScalarType, 4>
{
public:
	using Point<ScalarType, 4>::Point;

	Point4() = default; // default empty constructor

	Point4(const Point<ScalarType, 4>& p1); // from base class - will include all its constructors
	Point4(const ScalarType& x, const ScalarType& y, const ScalarType& z, const ScalarType& w);

	ScalarType x() const;
	ScalarType y() const;
	ScalarType z() const;
	ScalarType w() const;
	ScalarType& x();
	ScalarType& y();
	ScalarType& z();
	ScalarType& w();
};

using Point4i = Point4<int>;
using Point4f = Point4<float>;
using Point4d = Point4<double>;

} // namespace vcl

// inject vcl::Point4 hash function in std namespace
namespace std {
template <typename Scalar>
struct hash<vcl::Point4<Scalar> >
{
	size_t operator()(const vcl::Point4<Scalar>& id) const noexcept;
};
} // namespace std

#include "point4.cpp"

#endif // VCL_SPACE_POINT_POINT4_H
