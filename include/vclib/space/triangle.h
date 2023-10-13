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

#ifndef VCL_SPACE_TRIANGLE_H
#define VCL_SPACE_TRIANGLE_H

#include "triangle_wrapper.h"

namespace vcl {

template<PointConcept PointT>
class Triangle : public TriangleWrapper<PointT>
{
    std::array<PointT, 3> p;

public:
    using ScalarType = PointT::ScalarType;

    using PointType = PointT;

    static const uint DIM = PointT::DIM;

    Triangle() : TriangleWrapper<PointT>(p[0], p[1], p[2]) {}

    Triangle(const PointT& p0, const PointT& p1, const PointT& p2) :
            p(p0, p1, p2), TriangleWrapper<PointT>(p[0], p[1], p[2])
    {
    }

    using TriangleWrapper<PointT>::point;
    using TriangleWrapper<PointT>::point0;
    using TriangleWrapper<PointT>::point1;
    using TriangleWrapper<PointT>::point2;

    PointT& point(uint i) { return p[i]; }

    PointT& point0() { return p[0]; }

    PointT& point1() { return p[1]; }

    PointT& point2() { return p[2]; }
};

/* Specialization Aliases */

template<typename Scalar>
using Triangle2 = Triangle<Point2<Scalar>>;

using Triangle2f = Triangle<Point2f>;
using Triangle2d = Triangle<Point2d>;

template<typename Scalar>
using Triangle3 = Triangle<Point3<Scalar>>;

using Triangle3f = Triangle<Point3f>;
using Triangle3d = Triangle<Point3d>;

} // namespace vcl

#endif // VCL_SPACE_TRIANGLE_H
