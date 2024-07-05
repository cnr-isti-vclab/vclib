/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_SPACE_CORE_SEGMENT_H
#define VCL_SPACE_CORE_SEGMENT_H

#include <vclib/concepts/space/segment.h>

#include "point.h"

namespace vcl {

/**
 * @brief A class representing a line segment in n-dimensional space. The class
 * is parameterized by a `PointConcept`, which must provide the `DIM` constant
 * and the `[]` operator for accessing the point coordinates.
 *
 * @tparam PointT The type of the two endpoint points of the segment.
 *
 * @ingroup space
 */
template<PointConcept PointT>
class Segment
{
    PointT mPoint0;
    PointT mPoint1;

public:
    /**
     * @brief The type of point used to represent the endpoint points of the
     * segment.
     */
    using PointType = PointT;

    /**
     * @brief The scalar type of the endpoint points.
     */
    using ScalarType = PointT::ScalarType;

    /**
     * @brief The dimensionality of the segment.
     */
    static const uint DIM = PointT::DIM;

    /**
     * @brief Default constructor. Creates a segment with endpoints at the
     * origin.
     */
    Segment() {}

    /**
     * @brief Creates a segment with the given endpoints.
     *
     * @param[in] p0: The first endpoint of the segment.
     * @param[in] p1: The second endpoint of the segment.
     */
    Segment(const PointT& p0, const PointT& p1) : mPoint0(p0), mPoint1(p1) {}

    /**
     * @brief Returns the first endpoint of the segment.
     *
     * @return A reference to the first endpoint of the segment.
     */
    PointT& p0() { return mPoint0; }

    /**
     * @brief Returns the first endpoint of the segment.
     *
     * @return A const reference to the first endpoint of the segment.
     */
    const PointT& p0() const { return mPoint0; }

    /**
     * @brief Returns the second endpoint of the segment.
     *
     * @return A reference to the second endpoint of the segment.
     */
    PointT& p1() { return mPoint1; }

    /**
     * @brief Returns the second endpoint of the segment.
     *
     * @return A reference to the second endpoint of the segment.
     */
    const PointT& p1() const { return mPoint1; }

    PointT midPoint() const { return (mPoint0 + mPoint1) / 2.0; }

    PointT direction() const { return mPoint1 - mPoint0; }

    PointT normalizedDirection() const
    {
        return (mPoint1 - mPoint0).normalize();
    }

    ScalarType length() const { (mPoint0 - mPoint1).norm(); }

    ScalarType squaredLength() const { (mPoint0 - mPoint1).squaredNorm(); }

    void flip() { std::swap(mPoint0, mPoint1); }

    bool operator==(const Segment<PointT>& s) const = default;

    bool operator!=(const Segment<PointT>& s) const = default;

    Segment<PointT> operator+(const Segment<PointT>& s) const
    {
        return Segment<PointT>(mPoint0 + s.mPoint0, mPoint1 + s.mPoint1);
    }

    Segment<PointT> operator-(const Segment<PointT>& s) const
    {
        return Segment<PointT>(mPoint0 - s.mPoint0, mPoint1 - s.mPoint1);
    }

    Segment<PointT> operator*(const ScalarType& s) const
    {
        return Segment<PointT>(mPoint0 * s, mPoint1 * s);
    }

    Segment<PointT> operator/(const ScalarType& s) const
    {
        return Segment<PointT>(mPoint0 / s, mPoint1 / s);
    }

    Segment<PointT>& operator+=(const Segment<PointT>& s) const
    {
        mPoint0 += s.mPoint0;
        mPoint1 += s.mPoint1;
        return *this;
    }

    Segment<PointT>& operator-=(const Segment<PointT>& s) const
    {
        mPoint0 -= s.mPoint0;
        mPoint1 -= s.mPoint1;
        return *this;
    }

    Segment<PointT>& operator*=(const ScalarType& s) const
    {
        mPoint0 *= s;
        mPoint1 *= s;
        return *this;
    }

    Segment<PointT>& operator/=(const ScalarType& s) const
    {
        mPoint0 /= s;
        mPoint1 /= s;
        return *this;
    }
};

/* Specialization Aliases */

template<typename S>
using Segment2 = Segment<Point2<S>>;

using Segment2i = Segment<Point2i>;
using Segment2f = Segment<Point2f>;
using Segment2d = Segment<Point2d>;

template<typename S>
using Segment3 = Segment<Point3<S>>;

using Segment3i = Segment<Point3i>;
using Segment3f = Segment<Point3f>;
using Segment3d = Segment<Point3d>;

} // namespace vcl

#endif // VCL_SPACE_CORE_SEGMENT_H
