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

#include <vclib/algorithms/core/bounding_box.h>
#include <vclib/concepts/space/triangle.h>
#include <vclib/space/core/plane.h>
#include <vclib/space/core/segment.h>

namespace vcl {

/**
 * @brief Compute the distance between two Points of any dimension.
 *
 * @tparam PointType: The type of the input points.
 *
 * @param[in] point0: The first input point.
 * @param[in] point1: The second input point.
 *
 * @return The distance between the two points.
 *
 * @ingroup core_distance
 */
template<PointConcept PointType>
auto distance(const PointType& point0, const PointType& point1)
{
    return point0.dist(point1);
}

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
 * @ingroup core_distance
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
 * @ingroup core_distance
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
 * @ingroup core_distance
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
 * @ingroup core_distance
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
 * @ingroup core_distance
 */
template<SegmentConcept SegmentType, PointConcept PointType>
auto distance(const SegmentType& segment, const PointType& point)
    requires (PointType::DIM == SegmentType::DIM)
{
    return distance(point, segment);
}

/**
 * @brief Compute the bounded distance between a 3D point and a 3D triangle.
 *
 * Compute the distance between a 3D point and a 3D triangle. The distance can
 * be signed or unsigned, depending on the value of the signedDist parameter.
 *
 * The actual distance is computed only if the absolute value of the distance is
 * less than the maxDist parameter. If the distance is greater than maxDist, the
 * function returns immediately, and the returned value is a value greater than
 * maxDist.
 *
 * @tparam PointType: The type of point.
 * @tparam TriangleType: The type of triangle.
 *
 * @param[in] p: The point to calculate the distance from.
 * @param[in] triangle: The triangle to calculate the distance to.
 * @param[in] maxDist: The (absolute) maximum distance to consider. If the
 * absolute value of the distance is greater than this value, the function
 * returns immediately. Default is std::numeric_limits<ScalarType>::max().
 * @param[out] closest: The closest point on the triangle to the given point.
 * @param[in] signedDist: Whether to calculate the signed distance. Default is
 * false.
 * @return The distance between the point and the triangle.
 *
 * @ingroup core_distance
 */
template<
    Point3Concept    PointType,
    Triangle3Concept TriangleType,
    typename ScalarType>
auto boundedDistance(
    const PointType&    p,
    const TriangleType& triangle,
    ScalarType          maxDist,
    PointType&          closest,
    bool                signedDist = false)
{
    using TPointType = TriangleType::PointType;

    ScalarType dist;

    // Extract the coordinates of the vertices of the face.
    const TPointType& tp0 = triangle.point(0);
    const TPointType& tp1 = triangle.point(1);
    const TPointType& tp2 = triangle.point(2);

    // If the face is degenerate (i.e. its normal vector has zero length),
    // consider it as a segment.
    if (triangle.normal().norm() == 0) {
        // Calculate the bounding box of the face.
        Box3<ScalarType>     box = vcl::boundingBox(triangle);
        Segment3<ScalarType> s(box.min(), box.max());
        // If the diagonal of the bounding box is greater than zero, calculate
        // the distance between the query point and the segment.
        if (box.diagonal() > 0) {
            dist = vcl::distance(p, s, closest);
        }
        else {
            // If the diagonal of the bounding box is zero, set the closest
            // point to be the minimum corner of the bounding box and calculate
            // the distance between the query point and this point.
            closest = box.min();
            dist    = p.dist(closest);
        }
    }
    else {
        // Calculate the distance between the query point and the plane of the
        // triangle.
        Plane<ScalarType> fPlane(tp0, triangle.normal());
        dist = distance(p, fPlane, true);

        if (std::abs(dist) >= maxDist)
            return std::abs(dist);

        // Project the query point onto the triangle plane to obtain the closest
        // point on the triangle.
        closest = p - fPlane.direction() * dist;

        // Calculate the three edges of the triangle.
        Point3<ScalarType> fEdge[3];
        fEdge[0] = tp1 - tp0;
        fEdge[1] = tp2 - tp1;
        fEdge[2] = tp0 - tp2;

        // Determine the best axis to use for projection by finding the axis
        // with the largest component of the normal vector.
        int bestAxis;
        if (std::abs(triangle.normal()[0]) > std::abs(triangle.normal()[1])) {
            if (std::abs(triangle.normal()[0]) > std::abs(triangle.normal()[2]))
                bestAxis = 0;
            else
                bestAxis = 2;
        }
        else {
            if (std::abs(triangle.normal()[1]) > std::abs(triangle.normal()[2]))
                bestAxis = 1; /* 1 > 0 ? 2 */
            else
                bestAxis = 2; /* 2 > 1 ? 2 */
        }

        // Scale the edges by the inverse of the projection direction on the
        // best axis.
        ScalarType scaleFactor = 1 / fPlane.direction()[bestAxis];
        fEdge[0] *= scaleFactor;
        fEdge[1] *= scaleFactor;
        fEdge[2] *= scaleFactor;

        // Compute barycentric coordinates of closest point
        ScalarType b0, b1, b2;
        int        ba = (bestAxis + 2) % 3;
        int        bb = (bestAxis + 1) % 3;

        Segment3<ScalarType> s0(tp1, tp2), s1(tp2, tp0), s2(tp0, tp1);
        b0 = fEdge[1][bb] * (closest[ba] - tp1[ba]) -
             fEdge[1][ba] * (closest[bb] - tp1[bb]);
        if (b0 <= 0) {
            return distance(p, s0, closest);
        }
        b1 = fEdge[2][bb] * (closest[ba] - tp2[ba]) -
             fEdge[2][ba] * (closest[bb] - tp2[bb]);
        if (b1 <= 0) {
            return distance(p, s1, closest);
        }
        b2 = fEdge[0][bb] * (closest[ba] - tp0[ba]) -
             fEdge[0][ba] * (closest[bb] - tp0[bb]);
        if (b2 <= 0) {
            return distance(p, s2, closest);
        }

        const ScalarType EPS = ScalarType(0.000001);
        ScalarType       b   = vcl::min(b0, b1, b2);
        if (b < EPS * triangle.area()) {
            ScalarType bt;
            if (b == b0)
                bt = distance(p, s0, closest);
            else if (b == b1)
                bt = distance(p, s1, closest);
            else {
                assert(b == b2);
                bt = distance(p, s2, closest);
            }
            dist = bt;
        }

        if (!signedDist)
            dist = std::abs(dist);
    }

    return dist;
}

/**
 * @brief Compute the distance between a 3D point and a 3D triangle.
 *
 * @tparam PointType: The type of point.
 * @tparam TriangleType: The type of triangle.
 *
 * @param[in] p: The point to calculate the distance from.
 * @param[in] triangle: The triangle to calculate the distance to.
 * @param[in] maxDist: The maximum distance to consider. If the distance is
 * greater than this value, the function returns immediately. Default is
 * std::numeric_limits<ScalarType>::max().
 * @param[in] signedDist: Whether to calculate the signed distance. Default is
 * false.
 * @return The distance between the point and the triangle.
 *
 * @ingroup core_distance
 */
template<
    Point3Concept    PointType,
    Triangle3Concept TriangleType,
    typename ScalarType>
auto boundedDistance(
    const PointType&    p,
    const TriangleType& triangle,
    ScalarType          maxDist,
    bool                signedDist = false)
{
    PointType closest;
    return boundedDistance(p, triangle, maxDist, closest, signedDist);
}

/**
 * @brief Compute the distance between a 3D point and a 3D triangle.
 *
 * @tparam PointType: The type of point.
 * @tparam TriangleType: The type of triangle.
 *
 * @param[in] p: The point to calculate the distance from.
 * @param[in] triangle: The triangle to calculate the distance to.
 * @param[out] closest: The closest point on the triangle to the given point.
 * @param[in] signedDist: Whether to calculate the signed distance. Default is
 * false.
 * @return The distance between the point and the triangle.
 *
 * @ingroup core_distance
 */
template<
    Point3Concept    PointType,
    Triangle3Concept TriangleType,
    typename ScalarType>
auto distance(
    const PointType&    p,
    const TriangleType& triangle,
    PointType&          closest,
    bool                signedDist = false)
{
    ScalarType maxDist = std::numeric_limits<ScalarType>::max();
    return boundedDistance(p, triangle, maxDist, closest, signedDist);
}

/**
 * @brief Compute the distance between a 3D point and a 3D triangle.
 *
 * @tparam PointType: The type of point.
 * @tparam TriangleType: The type of triangle.
 *
 * @param[in] p: The point to calculate the distance from.
 * @param[in] triangle: The triangle to calculate the distance to.
 * @param[in] signedDist: Whether to calculate the signed distance. Default is
 * false.
 * @return The distance between the point and the triangle.
 *
 * @ingroup core_distance
 */
template<
    Point3Concept    PointType,
    Triangle3Concept TriangleType,
    typename ScalarType>
auto distance(
    const PointType&    p,
    const TriangleType& triangle,
    bool                signedDist = false)
{
    PointType closest;

    ScalarType maxDist = std::numeric_limits<ScalarType>::max();
    return boundedDistance(p, triangle, maxDist, closest, signedDist);
}

/**
 * @copydoc vcl::distance(const PointType&, const TriangleType&, bool)
 *
 * @ingroup core_distance
 */
template<
    Triangle3Concept TriangleType,
    Point3Concept    PointType,
    typename ScalarType>
auto distance(
    const TriangleType& triangle,
    const PointType&    p,
    bool                signedDist = false)
{
    return distance(p, triangle, signedDist);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_DISTANCE_MISC_H
