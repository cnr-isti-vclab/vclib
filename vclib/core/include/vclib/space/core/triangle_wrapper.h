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

#ifndef VCL_SPACE_CORE_TRIANGLE_WRAPPER_H
#define VCL_SPACE_CORE_TRIANGLE_WRAPPER_H

#include "triangle.h"

namespace vcl {

/**
 * @brief The TriangleWrapper class is a wrapper around a N-Dimensional
 * triangle.
 *
 * The dimension of the triangle is determined by the dimension of the points
 * it is composed of.
 *
 * The class does not store the triangle points, but it provides a wrapper
 * around them. This class is useful when the triangle points are already
 * stored somewhere else and the user does not want to copy them, but have
 * access to the a triangle interface.
 *
 * This means that the TriangleWrapper object must live as long as the points
 * it wraps. If the points are destroyed, the TriangleWrapper object becomes
 * invalid.
 *
 * This class cannot be copied or moved, and has just one constructor that
 * takes three points as input. The points are passed by const reference, and
 * their references are stored inside the class.
 *
 * @ingroup space_core
 */
template<PointConcept PointT>
class TriangleWrapper
{
    const PointT& mPoint0;
    const PointT& mPoint1;
    const PointT& mPoint2;

public:
    using ScalarType = PointT::ScalarType;
    using PointType  = PointT;

    /**
     * @brief Dimension of the triangle.
     */
    static const uint DIM = PointT::DIM;

    /**
     * @brief Constructs a triangle wrapper around the three input points.
     *
     * The input points **are not copied**, and this class just stores their
     * references.
     *
     * @param[in] p0: first point of the triangle.
     * @param[in] p1: second point of the triangle.
     * @param[in] p2: third point of the triangle.
     */
    TriangleWrapper(const PointT& p0, const PointT& p1, const PointT& p2) :
            mPoint0(p0), mPoint1(p1), mPoint2(p2)
    {
    }

    /**
     * @copydoc Triangle::size()
     */
    constexpr uint size() const { return 3; }

    /**
     * @copydoc Triangle::point(uint)
     */
    const PointT& point(uint i) const
    {
        switch (i) {
        case 0: return mPoint0;
        case 1: return mPoint1;
        case 2: return mPoint2;
        default:
            throw std::out_of_range(
                "TriangleWrapper::point: index out of range");
        }
    }

    /**
     * @copydoc Triangle::point0()
     */
    const PointT& point0() const { return mPoint0; }

    /**
     * @copydoc Triangle::point1()
     */
    const PointT& point1() const { return mPoint1; }

    /**
     * @copydoc Triangle::point2()
     */
    const PointT& point2() const { return mPoint2; }

    /**
     * @copydoc Triangle::sideLength()
     */
    ScalarType sideLength(uint i) const
    {
        return Triangle<PointT>::sideLength(mPoint0, mPoint1, mPoint2, i);
    }

    /**
     * @copydoc Triangle::sideLength0()
     */
    ScalarType sideLength0() const { return mPoint0.dist(mPoint1); }

    /**
     * @copydoc Triangle::sideLength1()
     */
    ScalarType sideLength1() const { return mPoint1.dist(mPoint2); }

    /**
     * @copydoc Triangle::sideLength2()
     */
    ScalarType sideLength2() const { return mPoint2.dist(mPoint0); }

    /**
     * @copydoc Triangle::normal()
     */
    PointT normal() const requires (PointT::DIM == 3)
    {
        return Triangle<PointT>::normal(mPoint0, mPoint1, mPoint2);
    }

    /**
     * @copydoc Triangle::barycenter()
     */
    PointT barycenter() const
    {
        return Triangle<PointT>::barycenter(mPoint0, mPoint1, mPoint2);
    }

    /**
     * @copydoc Triangle::weightedBarycenter()
     */
    PointT weightedBarycenter(ScalarType w0, ScalarType w1, ScalarType w2) const
    {
        return Triangle<PointT>::weightedBarycenter(
            mPoint0, mPoint1, mPoint2, w0, w1, w2);
    }

    /**
     * @copydoc Triangle::weightedBarycenter()
     */
    PointT weightedBarycenter(const Point3<ScalarType>& w) const
    {
        return Triangle<PointT>::weightedBarycenter(
            mPoint0, mPoint1, mPoint2, w(0), w(1), w(2));
    }

    /**
     * @copydoc Triangle::barycentricCoordinatePoint()
     */
    PointT barycentricCoordinatePoint(
        ScalarType b0,
        ScalarType b1,
        ScalarType b2) const
    {
        return Triangle<PointT>::barycentricCoordinatePoint(
            mPoint0, mPoint1, mPoint2, b0, b1, b2);
    }

    /**
     * @copydoc Triangle::barycentricCoordinatePoint()
     */
    PointT barycentricCoordinatePoint(const Point3<ScalarType>& b) const
    {
        return Triangle<PointT>::barycentricCoordinatePoint(
            mPoint0, mPoint1, mPoint2, b(0), b(1), b(2));
    }

    /**
     * @copydoc Triangle::circumcenter()
     */
    PointT circumcenter() const
    {
        return Triangle<PointT>::circumcenter(mPoint0, mPoint1, mPoint2);
    }

    /**
     * @copydoc Triangle::perimeter()
     */
    ScalarType perimeter() const
    {
        return Triangle<PointT>::perimeter(mPoint0, mPoint1, mPoint2);
    }

    /**
     * @copydoc Triangle::area()
     */
    ScalarType area() const
    {
        return Triangle<PointT>::area(mPoint0, mPoint1, mPoint2);
    }

    /**
     * @copydoc Triangle::quality()
     */
    ScalarType quality() const
    {
        return Triangle<PointT>::quality(mPoint0, mPoint1, mPoint2);
    }

    /**
     * @copydoc Triangle::qualityRadii()
     */
    ScalarType qualityRadii() const
    {
        return Triangle<PointT>::qualityRadii(mPoint0, mPoint1, mPoint2);
    }

    /**
     * @copydoc Triangle::qualityMeanRatio()
     */
    ScalarType qualityMeanRatio() const
    {
        return Triangle<PointT>::qualityMeanRatio(mPoint0, mPoint1, mPoint2);
    }
};

/* Specialization Aliases */

template<typename Scalar>
using TriangleWrapper2 = TriangleWrapper<Point2<Scalar>>;

using TriangleWrapper2f = TriangleWrapper<Point2f>;
using TriangleWrapper2d = TriangleWrapper<Point2d>;

template<typename Scalar>
using TriangleWrapper3 = TriangleWrapper<Point3<Scalar>>;

using TriangleWrapper3f = TriangleWrapper<Point3f>;
using TriangleWrapper3d = TriangleWrapper<Point3d>;

} // namespace vcl

#endif // VCL_SPACE_CORE_TRIANGLE_WRAPPER_H
