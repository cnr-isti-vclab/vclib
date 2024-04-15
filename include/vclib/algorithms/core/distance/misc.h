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

#include <vclib/space/plane.h>
#include <vclib/space/segment.h>

namespace vcl {

template<Point3Concept PointType, PlaneConcept PlaneType>
auto pointPlaneDistance(
    const PointType& p,
    const PlaneType& plane,
    bool             signedDist = false)
{
    auto dist = plane.direction().dot(p) - plane.offset();
    if (!signedDist)
        dist = std::abs(dist);
    return dist;
}

template<PointConcept PointType, SegmentConcept SegmentType>
auto pointSegmentDistance(
    const PointType&   p,
    const SegmentType& s,
    PointType&         closest) requires (PointType::DIM == SegmentType::DIM)
{
    using ScalarType = PointType::ScalarType;

    ScalarType dist;

    PointType  dir = s.direction();
    ScalarType esn = dir.squaredNorm();

    if (esn < std::numeric_limits<ScalarType>::min()) {
        closest = s.midPoint();
    }
    else {
        ScalarType t = ((p - s.p0()) * dir) / esn;
        if (t < 0)
            t = 0;
        else if (t > 1)
            t = 1;

        closest = s.p0() * (1 - t) + s.p1() * t;
    }
    dist = p.dist(closest);
    return dist;
}

template<PointConcept PointType, SegmentConcept SegmentType>
auto pointSegmentDistance(const PointType& p, const SegmentType& s)
    requires (PointType::DIM == SegmentType::DIM)
{
    PointType closest;
    return pointSegmentDistance(p, s, closest);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_DISTANCE_MISC_H
