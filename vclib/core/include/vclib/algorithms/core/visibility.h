// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_CORE_VISIBILITY_H
#define VCL_ALGORITHMS_CORE_VISIBILITY_H

#include <vclib/space/core.h>

namespace vcl {

/**
 * @brief Compute the determinant of the half-space defined by the triangle
 * (p1, p2, p3) and the point p.
 *
 * The triangle is defined by the points p1, p2, and p3, ordered in a
 * counter-clockwise manner.
 *
 * @tparam PointType: The type of the points.
 * @param[in] p0: The first point of the triangle.
 * @param[in] p1: The second point of the triangle.
 * @param[in] p2: The third point of the triangle.
 * @param[in] p: The point to test.
 * @return The determinant of the half-space.
 *
 * @ingroup algorithms_core
 */
template<Point3Concept PointType>
auto halfSpaceDeterminant(
    const PointType& p0,
    const PointType& p1,
    const PointType& p2,
    const PointType& p)
{
    return (p1 - p0).cross(p2 - p0).dot(p - p0);
}

/**
 * @brief Compute the determinant of the half-space defined by the triangle
 * and the point.
 *
 * @tparam TriangleType: The type of the triangle that defines the half-space.
 * @tparam PointType: The type of the point to test.
 *
 * @param[in] triangle: The triangle that defines the half-space.
 * @param[in] p: The point to test.
 * @return The determinant of the half-space.
 *
 * @ingroup algorithms_core
 */
template<Triangle3Concept TriangleType, Point3Concept PointType>
auto halfSpaceDeterminant(const TriangleType& triangle, const PointType& point)
{
    return halfSpaceDeterminant(
        triangle.point(0), triangle.point(1), triangle.point(2), point);
}

/**
 * @brief Checks if 3 points are collinear.
 *
 * @tparam PointType: The type of the points.
 * @param[in] p0: First point to test.
 * @param[in] p1: Second point to test.
 * @param[in] p2: Third point to test.
 * @return True if the points are collinear, false otherwise.
 *
 * @ingroup algorithms_core
 */
template<Point3Concept PointType>
bool arePointsCollinear(
    const PointType& p0,
    const PointType& p1,
    const PointType& p2)
{
    PointType c = (p1 - p0).cross(p2 - p0);
    return c.squaredNorm() == 0;
}

/**
 * @brief Checks if 4 points are coplanar.
 *
 * @tparam PointType: The type of the points.
 * @param[in] p0: First point to test.
 * @param[in] p1: Second point to test.
 * @param[in] p2: Third point to test.
 * @param[in] p3: Fourth point to test.
 * @return True if the points are coplanar, false otherwise.
 *
 * @ingroup algorithms_core
 */
template<Point3Concept PointType>
bool arePointsCoplanar(
    const PointType& p0,
    const PointType& p1,
    const PointType& p2,
    const PointType& p3)
{
    return halfSpaceDeterminant(p0, p1, p2, p3) == 0;
}

/**
 * @brief Checks if a point is visible from a triangle, i.e., if the point is
 * in the half-space defined by the triangle.
 *
 * @tparam TriangleType: The type of the triangle.
 * @tparam PointType: The type of the point.
 * @param[in] triangle: The input triangle.
 * @param[in] point: The point to test.
 * @return true if the point is visible from the triangle, false otherwise.
 *
 * @ingroup algorithms_core
 */
template<Triangle3Concept TriangleType, Point3Concept PointType>
bool trianglePointVisibility(
    const TriangleType& triangle,
    const PointType&    point)
{
    return halfSpaceDeterminant(triangle, point) > 0;
}

/**
 * @brief Checks if a point is visible from a triangle, i.e., if the point is
 * in the half-space defined by the triangle.
 *
 * @tparam PointType: The type of the points.
 * @param[in] p0: The first point of the triangle.
 * @param[in] p1: The second point of the triangle.
 * @param[in] p2: The third point of the triangle.
 * @param[in] p: The point to test.
 * @return true if the point is visible from the triangle, false otherwise.
 *
 * @ingroup algorithms_core
 */
template<Point3Concept PointType>
auto trianglePointVisibility(
    const PointType& p0,
    const PointType& p1,
    const PointType& p2,
    const PointType& p)
{
    return halfSpaceDeterminant(p0, p1, p2, p) > 0;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_VISIBILITY_H
