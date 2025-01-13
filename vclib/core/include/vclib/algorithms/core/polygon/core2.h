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

#ifndef VCL_ALGORITHMS_CORE_POLYGON_CORE2_H
#define VCL_ALGORITHMS_CORE_POLYGON_CORE2_H

#include <vclib/space/core/polygon.h>

namespace vcl {

/**
 * @brief Computes the collinearity test between three points. The test returns
 * a positive value if the points are counter-clockwise, a negative value if the
 * points are clockwise, and zero if the points are collinear.
 *
 * The function computes the z coordinate of the cross product between the
 * vectors p1 - p0 and p2 - p0:
 * - If the result is 0, the points are collinear;
 * - if the result is positive, the three points constitute a "left turn" or
 *   counter-clockwise orientation (p0 is at the left of the line p1-p2);
 * - if the result is negative, a "right turn" or clockwise orientation
 *   (p0 is at the right of the line p1-p2).
 *
 * @tparam PointType: the type of the points that satisfies the Point2Concept.
 * @param[in] p0: the first point.
 * @param[in] p1: the second point.
 * @param[in] p2: the third point.
 * @return 0 if the points are collinear, a positive value if the points are
 * counter-clockwise, and a negative value if the points are clockwise.
 *
 * @ingroup core_polygon
 */
template<Point2Concept PointType>
auto collinearityTest(
    const PointType& p0,
    const PointType& p1,
    const PointType& p2)
{
    return (
        (p1.x() - p0.x()) * (p2.y() - p0.y()) -
        (p2.x() - p0.x()) * (p1.y() - p0.y()));
}

/**
 * @brief Checks if the three points are counter-clockwise.
 *
 * @tparam PointType: the type of the points that satisfies the Point2Concept.
 * @param[in] p0: the first point.
 * @param[in] p1: the second point.
 * @param[in] p2: the third point.
 * @return true if the points are counter-clockwise, false otherwise.
 *
 * @ingroup core_polygon
 */
template<Point2Concept PointType>
bool areCounterClockwise(
    const PointType& p0,
    const PointType& p1,
    const PointType& p2)
{
    return collinearityTest(p0, p1, p2) > 0;
}

/**
 * @brief Checks if a set of points that form a polygon are in counter-clockwise
 * order.
 *
 * @tparam Iterator: the type of iterator that iterates over a range of points
 * that satisfy the Point2Concept.
 * @param[in] begin: the iterator pointing to the first point.
 * @param[in] end: the iterator pointing to one past the last point.
 * @return true if the points are in counter-clockwise order, false otherwise.
 *
 * @ingroup core_polygon
 */
template<Point2IteratorConcept Iterator>
bool isCounterClockWise(Iterator begin, Iterator end)
{
    using PointType  = std::decay_t<decltype(*begin)>;
    using ScalarType = PointType::ScalarType;

    if (begin == end)
        return false;

    auto       latest = begin;
    auto       it     = begin;
    ScalarType sum    = 0;
    for (++it; it != end; ++it) {
        sum += (it->x() - latest->x()) * (it->y() + latest->y());
        latest = it;
    }

    sum += (latest->x() - begin->x()) * (latest->y() + begin->y());

    return sum < 0;
}

/**
 * @brief Sorts the vertices of a convex polygon in counter-clockwise order.
 *
 * Given a set of points that form a convex polygon, this function sorts the
 * points in counter-clockwise order. The function assumes that the input points
 * form a convex polygon, and it sorts the points in counter-clockwise order
 * with respect to the point with the lowest y-coordinate. The function uses the
 * atan2 function to compute the angle of each point with respect to the point
 * with the lowest y-coordinate, and it sorts the points based on these angles.
 *
 * @tparam Iterator: the type of iterator that iterates over a range of points
 * that satisfy the Point2Concept.
 * @param[in] begin: the iterator pointing to the first point.
 * @param[in] end: the iterator pointing to one past the last point.
 *
 * @ingroup core_polygon
 */
template<Point2IteratorConcept Iterator>
void sortConvexPolygonVertices(Iterator begin, Iterator end)
{
    using PointType = std::decay_t<decltype(*begin)>;

    // Find the point with the lowest x-coordinate
    PointType minPoint = *std::min_element(
        begin, end, [](const PointType& a, const PointType& b) {
            return a.y() < b.y();
        });

    // Sort the points based on the angle with respect to minPoint
    std::sort(begin, end, [minPoint](const PointType& a, const PointType& b) {
        double angleA = std::atan2(a.y() - minPoint.y(), a.x() - minPoint.x());
        double angleB = std::atan2(b.y() - minPoint.y(), b.x() - minPoint.x());
        return angleA < angleB;
    });
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_POLYGON_CORE2_H
