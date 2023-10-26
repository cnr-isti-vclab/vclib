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

#ifndef VCL_SPACE_QUATERNION_H
#define VCL_SPACE_QUATERNION_H

#include "matrix.h"
#include "point.h"

namespace vcl {

template<typename Scalar>
class Quaternion : public Point4<Scalar>
{
    using Base = Point4<Scalar>;

public:
    Quaternion() = default;

    Quaternion(
        const Scalar& w,
        const Scalar& x,
        const Scalar& y,
        const Scalar& z) :
            Base(w, x, y, z)
    {
    }

    Quaternion(const Point4<Scalar>& p) : Base(p) {}

    Quaternion(const Scalar& s, const Point3<Scalar>& v) :
            Base(s, v(0), v(1), v(2))
    {
    }

    Scalar& w() { return Base::x(); }

    const Scalar& w() const { return Base::x(); }

    Scalar& x() { return Base::y(); }

    const Scalar& x() const { return Base::y(); }

    Scalar& y() { return Base::z(); }

    const Scalar& y() const { return Base::z(); }

    Scalar& z() { return Base::w(); }

    const Scalar& z() const { return Base::w(); }
};

} // namespace vcl

#endif // VCL_SPACE_QUATERNION_H
