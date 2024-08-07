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

#ifndef VCL_ALGORITHMS_CORE_DISTANCE_MISC_H
#define VCL_ALGORITHMS_CORE_DISTANCE_MISC_H

#include <vclib/space/core/plane.h>
#include <vclib/space/core/segment.h>

namespace vcl {

/**
 * @brief Compute the distance between a point and a plane.
 *
 * Compute the distance between a point and a plane. The distance can be signed
 * or unsigned, depending on the value of the signedDist parameter.
 *
 * @tparam PointType: The type of the input point.
 * @tparam PlaneType: The type of the input plane.
 * 
 * @param[in] point: The input point.
 * @param[in] plane: The input plane.
 * @param[in] signedDist: If true, the distance is signed, otherwise it is
 * unsigned.
 * @return The distance between the point and the plane.
 * 
 * @ingroup distance_core
 */
template<Point3Concept PointType, PlaneConcept PlaneType>
auto distance(
    const PointType& point,
    const PlaneType& plane,
    bool             signedDist = false)
{
    auto dist = plane.direction().dot(point) - plane.offset();
    if (!signedDist)
        dist = std::abs(dist);
    return dist;
}

/**
 * @copydoc vcl::distance(const PointType&, const PlaneType&, bool)
 * 
 * @ingroup distance_core
 */
template<PlaneConcept PlaneType, Point3Concept PointType>
auto distance(
    const PlaneType& plane,
    const PointType& point,
    bool             signedDist = false)
{
    return distance(point, plane, signedDist);
}

/**
 * @brief Compute the distance between a point and a segment.
 *
 * Compute the distance between a point and a segment of any dimension. The
 * closest point on the segment is also computed, and stored in the closestPoint
 * parameter.
 *
 * @tparam PointType: The type of the input point.
 * @tparam SegmentType: The type of the input segment.
 *
 * @param[in] point: The input point.
 * @param[in] segment: The input segment.
 * @param[out] closestPoint: The closest point on the segment.
 * @return The distance between the point and the segment.
 *
 * @ingroup distance_core
 */
template<PointConcept PointType, SegmentConcept SegmentType>
auto distance(
    const PointType&   point,
    const SegmentType& segment,
    PointType& closestPoint) requires (PointType::DIM == SegmentType::DIM)
{
    using ScalarType = PointType::ScalarType;

    ScalarType dist;

    PointType  dir = segment.direction();
    ScalarType esn = dir.squaredNorm();

    if (esn < std::numeric_limits<ScalarType>::min()) {
        closestPoint = segment.midPoint();
    }
    else {
        ScalarType t = ((point - segment.p0()).dot(dir)) / esn;
        if (t < 0)
            t = 0;
        else if (t > 1)
            t = 1;

        closestPoint = segment.p0() * (1 - t) + segment.p1() * t;
    }
    dist = point.dist(closestPoint);
    return dist;
}

/**
 * @brief Compute the distance between a point and a segment.
 *
 * Compute the distance between a point and a segment of any dimension.
 * 
 * @tparam PointType: The type of the input point.
 * @tparam SegmentType: The type of the input segment.
 * 
 * @param[in] point: The input point.
 * @param[in] segment: The input segment.
 * @return The distance between the point and the segment.
 * 
 * @ingroup distance_core
 */
template<PointConcept PointType, SegmentConcept SegmentType>
auto distance(const PointType& point, const SegmentType& segment)
    requires (PointType::DIM == SegmentType::DIM)
{
    PointType closestPoint;
    return distance(point, segment, closestPoint);
}

/**
 * @copydoc vcl::distance(const PointType&, const SegmentType&)
 * 
 * @ingroup distance_core
 */
template<SegmentConcept SegmentType, PointConcept PointType>
auto distance(const SegmentType& segment, const PointType& point)
    requires (PointType::DIM == SegmentType::DIM)
{
    return distance(point, segment);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_DISTANCE_MISC_H
