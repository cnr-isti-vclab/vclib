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

#ifndef VCL_ALGORITHMS_CORE_INTERSECTION_MISC_H
#define VCL_ALGORITHMS_CORE_INTERSECTION_MISC_H

#include <optional>

#include <vclib/space/core/box.h>
#include <vclib/space/core/plane.h>
#include <vclib/space/core/segment.h>
#include <vclib/space/core/sphere.h>
#include <vclib/space/core/triangle.h>

namespace vcl {

namespace detail {

/*
 * ===== plane segment intersection functions =====
 */

template<PlaneConcept PlaneType, Segment3Concept SegmentType>
void projectSegmentEndPoints(
    const PlaneType&                  p,
    const SegmentType&                s,
    typename SegmentType::ScalarType& p0Proj,
    typename SegmentType::ScalarType& p1Proj)
{
    using ScalarType = SegmentType::ScalarType;

    // Compute the projection of the segment endpoints onto the plane.
    p0Proj = s.p1() * p.direction() - p.offset();
    p1Proj = s.p0() * p.direction() - p.offset();
}

/*
 * ===== triangle box intersection functions =====
 */

template<typename ScalarType>
inline void findMinMax(
    ScalarType  x0,
    ScalarType  x1,
    ScalarType  x2,
    ScalarType& min,
    ScalarType& max)
{
    min = max = x0;
    if (x1 < min)
        min = x1;
    if (x1 > max)
        max = x1;
    if (x2 < min)
        min = x2;
    if (x2 > max)
        max = x2;
}

/*======================== X-tests ========================*/
template<typename ScalarType, PointConcept PointType>
inline bool axisTestX01(
    ScalarType       a,
    ScalarType       b,
    ScalarType       fa,
    ScalarType       fb,
    const PointType& v0,
    const PointType& v2,
    const PointType& bHalfSize)
{
    ScalarType p0 = a * v0.y() - b * v0.z();
    ScalarType p2 = a * v2.y() - b * v2.z();
    ScalarType min, max;
    if (p0 < p2) {
        min = p0;
        max = p2;
    }
    else {
        min = p2;
        max = p0;
    }
    ScalarType rad = fa * bHalfSize.y() + fb * bHalfSize.z();
    if (min > rad || max < -rad)
        return false;
    return true;
}

template<typename ScalarType, PointConcept PointType>
inline bool axisTestX2(
    ScalarType       a,
    ScalarType       b,
    ScalarType       fa,
    ScalarType       fb,
    const PointType& v0,
    const PointType& v1,
    const PointType& bHalfSize)
{
    ScalarType p0 = a * v0.y() - b * v0.z();
    ScalarType p1 = a * v1.y() - b * v1.z();
    ScalarType min, max;
    if (p0 < p1) {
        min = p0;
        max = p1;
    }
    else {
        min = p1;
        max = p0;
    }
    ScalarType rad = fa * bHalfSize.y() + fb * bHalfSize.z();
    if (min > rad || max < -rad)
        return false;
    return true;
}

/*======================== Y-tests ========================*/
template<typename ScalarType, PointConcept PointType>
inline bool axisTestY02(
    ScalarType       a,
    ScalarType       b,
    ScalarType       fa,
    ScalarType       fb,
    const PointType& v0,
    const PointType& v2,
    const PointType& bHalfSize)
{
    ScalarType p0 = -a * v0.x() + b * v0.z();
    ScalarType p2 = -a * v2.x() + b * v2.z();
    ScalarType min, max;
    if (p0 < p2) {
        min = p0;
        max = p2;
    }
    else {
        min = p2;
        max = p0;
    }
    ScalarType rad = fa * bHalfSize.x() + fb * bHalfSize.z();
    if (min > rad || max < -rad)
        return false;
    return true;
}

template<typename ScalarType, PointConcept PointType>
inline bool axisTestY1(
    ScalarType       a,
    ScalarType       b,
    ScalarType       fa,
    ScalarType       fb,
    const PointType& v0,
    const PointType& v1,
    const PointType& bHalfSize)
{
    ScalarType p0 = -a * v0.x() + b * v0.z();
    ScalarType p1 = -a * v1.x() + b * v1.z();
    ScalarType min, max;
    if (p0 < p1) {
        min = p0;
        max = p1;
    }
    else {
        min = p1;
        max = p0;
    }
    ScalarType rad = fa * bHalfSize.x() + fb * bHalfSize.z();
    if (min > rad || max < -rad)
        return false;
    return true;
}

/*======================== Z-tests ========================*/
template<typename ScalarType, PointConcept PointType>
inline bool axisTestZ12(
    ScalarType       a,
    ScalarType       b,
    ScalarType       fa,
    ScalarType       fb,
    const PointType& v1,
    const PointType& v2,
    const PointType& bHalfSize)
{
    ScalarType p1 = a * v1.x() - b * v1.y();
    ScalarType p2 = a * v2.x() - b * v2.y();
    ScalarType min, max;
    if (p1 < p2) {
        min = p1;
        max = p2;
    }
    else {
        min = p2;
        max = p1;
    }
    ScalarType rad = fa * bHalfSize.x() + fb * bHalfSize.y();
    if (min > rad || max < -rad)
        return false;
    return true;
}

template<typename ScalarType, PointConcept PointType>
inline bool axisTestZ0(
    ScalarType       a,
    ScalarType       b,
    ScalarType       fa,
    ScalarType       fb,
    const PointType& v0,
    const PointType& v1,
    const PointType& bHalfSize)
{
    ScalarType p0 = a * v0.x() - b * v0.y();
    ScalarType p1 = a * v1.x() - b * v1.y();
    ScalarType min, max;
    if (p0 < p1) {
        min = p0;
        max = p1;
    }
    else {
        min = p1;
        max = p0;
    }
    ScalarType rad = fa * bHalfSize.x() + fb * bHalfSize.y();
    if (min > rad || max < -rad)
        return false;
    return true;
}

} // namespace detail

/**
 * @brief Checks if a plane intersects with a box.
 *
 * Uses the algorithm from
 * https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
 *
 * @tparam PlaneType: The type of plane used in the intersection check.
 * @tparam BoxType: The type of box used in the intersection check.
 *
 * @param[in] plane: The plane to check intersection with.
 * @param[in] box: The box to check intersection with.
 * @return True if the plane intersects with the box, false otherwise.
 *
 * @ingroup intersection_core
 */
template<PlaneConcept PlaneType, Box3Concept BoxType>
bool intersect(const PlaneType& plane, const BoxType& box)
{
    using PointType  = BoxType::PointType;
    using ScalarType = PointType::ScalarType;

    // Convert AABB to center-extents representation
    PointType c = box.center(); // Compute AABB center
    PointType e = box.max() - c; // Compute positive extents

    PointType n = plane.direction();
    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    ScalarType r =
        e[0] * std::abs(n[0]) + e[1] * std::abs(n[1]) + e[2] * std::abs(n[2]);

    // Compute distance of box center from plane
    ScalarType s = n.dot(c) - plane.offset();

    // Intersection occurs when distance s falls within [-r,+r] interval
    return std::abs(s) <= r;
}

/**
 * @copydoc vcl::intersect(const PlaneType&, const BoxType&)
 *
 * @ingroup intersection_core
 */
template<Box3Concept BoxType, PlaneConcept PlaneType>
bool intersect(const BoxType& box, const PlaneType& p)
{
    return intersect(p, box);
}

/**
 * @brief Checks if a plane intersects with a segment.
 *
 * The function computes the projection of the segment endpoints onto the plane
 * and checks if the endpoints are on opposite sides of the plane. If the
 * endpoints are on opposite sides of the plane, the function returns true,
 * indicating that an intersection exists.
 *
 * @tparam PlaneType: The type of plane used in the intersection check.
 * @tparam SegmentType: The type of segment used in the intersection check.
 *
 * @param[in] plane: the plane to compute the intersection with.
 * @param[in] segment: the 3D segment to compute the intersection with.
 * @return A boolean value indicating whether an intersection was found or not.
 *
 * @ingroup intersection_core
 */
template<PlaneConcept PlaneType, Segment3Concept SegmentType>
bool intersect(const PlaneType& plane, const SegmentType& segment)
{
    using ScalarType = SegmentType::ScalarType;

    ScalarType p0Proj;
    ScalarType p1Proj;

    detail::projectSegmentEndPoints(plane, segment, p1Proj, p0Proj);

    // If both endpoints are on the same side of the plane, there is no
    // intersection.
    if ((p0Proj > 0) - (p1Proj < 0))
        return false;

    // If both endpoints have the same projection onto the plane, there is no
    // intersection.
    if (p1Proj == p0Proj)
        return false;

    return true;
}

/**
 * @copydoc vcl::intersect(const PlaneType&, const SegmentType&)
 *
 * @ingroup intersection_core
 */
template<Segment3Concept SegmentType, PlaneConcept PlaneType>
bool intersect(const SegmentType& segment, const PlaneType& plane)
{
    return intersect(plane, segment);
}

/**
 * @brief Returns the intersection point between a plane and a segment, if it
 * exists.
 *
 * The function computes the projection of the segment endpoints onto the plane
 * and checks if the endpoints are on opposite sides of the plane. If the
 * endpoints are on opposite sides of the plane, the function computes and
 * returns the intersection point between the plane and the segment.
 *
 * If the endpoints are on the same side of the plane, or if the endpoints have
 * the same projection onto the plane, the function returns an empty optional.
 *
 * @tparam PlaneType: The type of plane used in the intersection check.
 * @tparam SegmentType: The type of segment used in the intersection check.
 *
 * @param[in] plane: the plane to compute the intersection with.
 * @param[in] segment: the 3D segment to compute the intersection with.
 * @return An optional point that represents the intersection point between the
 * plane and the box, if it exists.
 *
 * @ingroup intersection_core
 */
template<PlaneConcept PlaneType, Segment3Concept SegmentType>
std::optional<typename SegmentType::PointType> intersection(
    const PlaneType&   plane,
    const SegmentType& segment)
{
    std::optional<typename SegmentType::PointType> intersection;

    using ScalarType = SegmentType::ScalarType;

    ScalarType p0Proj;
    ScalarType p1Proj;

    detail::projectSegmentEndPoints(plane, segment, p1Proj, p0Proj);

    // If either endpoint is on the opposite side of the plane, there is an
    // intersection.
    if ((p0Proj > 0) != (p1Proj > 0) || p1Proj != p0Proj) {
        // check that we perform the computation in a way that is independent
        // with v0 v1 swaps
        if (p1Proj < p0Proj)
            intersection = segment.p0() + (segment.p1() - segment.p0()) *
                                        std::abs(p1Proj / (p0Proj - p1Proj));
        if (p1Proj > p0Proj)
            intersection = segment.p1() + (segment.p0() - segment.p1()) *
                                        std::abs(p0Proj / (p1Proj - p0Proj));
    }

    return intersection;
}

/**
 * @brief Checks if a sphere intersects with a 3D box.
 *
 * @tparam SphereType: A type that satisfies the SphereConcept concept.
 * @tparam BoxType: A type that satisfies the Box3Concept concept.
 *
 * @param[in] sphere: The sphere to check for intersection.
 * @param[in] box: The box to check for intersection.
 * @return true if the sphere intersects with the box, false otherwise.
 *
 * @ingroup intersection_core
 */
template<SphereConcept SphereType, Box3Concept BoxType>
bool intersect(const SphereType& sphere, const BoxType& box)
{
    return sphere.intersects(box);
}

/**
 * @copydoc vcl::intersect(const SphereType&, const BoxType&)
 *
 * @ingroup intersection_core
 */
template<Box3Concept BoxType, SphereConcept SphereType>
bool intersect(const BoxType& box, const SphereType& sphere)
{
    return sphere.intersects(box);
}

/**
 * @brief Checks if a 2D point intersects with/is inside a given 2D triangle
 * having its points in counterclockwise order.
 *
 * The function checks if a point lies within a given triangle using barycentric
 * coordinates. It first calculates the area of the triangle, and then
 * calculates the barycentric coordinates of the point with respect to the
 * triangle. If the barycentric coordinates satisfy certain conditions, then the
 * point is considered to intersect with the triangle.
 *
 * @tparam TriangleType: A type that satisfies the ConstTriangle2Concept
 * concept.
 * @tparam PointType: A type that satisfies the Point2Concept concept.
 *
 * @param[in] triangle: The triangle to test for intersection.
 * @param[in] point: The point to test for intersection with the triangle.
 * @return True if the point intersects with/is inside the triangle, false
 * otherwise.
 *
 * @ingroup intersection_core
 */
template<ConstTriangle2Concept TriangleType, Point2Concept PointType>
bool intersect(const TriangleType& triangle, const PointType& point)
{
    using TP         = TriangleType::PointType;
    using ScalarType = TP::ScalarType;

    const TP& p0 = triangle.point(0);
    const TP& p1 = triangle.point(1);
    const TP& p2 = triangle.point(2);

    ScalarType A    = triangle.area();
    ScalarType sign = A < 0 ? -1 : 1;

    ScalarType s =
        (p0.y() * p2.x() - p0.x() * p2.y() + (p2.y() - p0.y()) * point.x() +
         (p0.x() - p2.x()) * point.y()) *
        sign;
    ScalarType tt =
        (p0.x() * p1.y() - p0.y() * p1.x() + (p0.y() - p1.y()) * point.x() +
         (p1.x() - p0.x()) * point.y()) *
        sign;

    return s > 0 && tt > 0 && (s + tt) < 2 * A * sign;
}

/**
 * @copydoc vcl::intersect(const TriangleType&, const PointType&)
 *
 * @ingroup intersection_core
 */
template<Point2Concept PointType, ConstTriangle2Concept TriangleType>
bool intersect(const PointType& point, const TriangleType& triangle)
{
    return intersect(triangle, point);
}

/**
 * @brief Checks if a 3D point intersects/is inside a 3D triangle having its
 * points in counterclockwise order.
 *
 * @tparam TriangleType: A type that satisfies the ConstTriangle3Concept
 * concept.
 * @tparam PointType: A type that satisfies the Point3Concept concept.
 *
 * @param[in] triangle: The triangle to test for intersection.
 * @param[in] point: The point to test for intersection with the triangle.
 * @return True if the point intersects with/is inside the triangle, false
 * otherwise.
 *
 * @ingroup intersection_core
 */
template<ConstTriangle3Concept TriangleType, Point3Concept PointType>
bool intersect(const TriangleType& triangle, const PointType& point)
{
    PointType v1 = triangle.point(1) - triangle.point(0);
    PointType v2 = triangle.point(2) - triangle.point(0);
    PointType v3 = point - triangle.point(0);

    return v1.dot(v2.cross(v3)) > 0;
}

/**
 * @copydoc vcl::intersect(const TriangleType&, const PointType&)
 *
 * @ingroup intersection_core
 */
template<Point3Concept PointType, ConstTriangle3Concept TriangleType>
bool intersect(const PointType& point, const TriangleType& triangle)
{
    return intersect(triangle, point);
}

/**
 * @brief Checks if a triangle intersects with a box.
 *
 * The function uses the separating axis theorem to test the overlap between a
 * triangle and a box. It tests for overlap in the following directions:
 * 1) the {x,y,z}-directions (actually, since we use the AABB of the triangle we
 * do not even need to test these)
 * 2) normal of the triangle
 * 3) cross product(edge from tri, {x,y,z}-direction)
 *
 * @tparam TriangleType: A type that satisfies the ConstTriangle3Concept
 * concept.
 * @tparam BoxType: A type that satisfies the Box3Concept concept.
 *
 * @param[in] triangle: The triangle to test for intersection.
 * @param[in] box: The box to test for intersection with the triangle.
 *
 * @return True if the box and triangle overlap, false otherwise.
 *
 * @private https://gist.github.com/jflipts/fc68d4eeacfcc04fbdb2bf38e0911850
 *
 * @ingroup intersection_core
 */
template<ConstTriangle3Concept TriangleType, Box3Concept BoxType>
bool intersect(const TriangleType& triangle, const BoxType& box)
{
    using PointType  = TriangleType::PointType;
    using ScalarType = PointType::ScalarType;

    PointType boxCenter = box.center();
    PointType bHalfSize = box.size() / 2;

    /* use separating axis theorem to test overlap between triangle and box
     * need to test for overlap in these directions:
     *    1) the {x,y,z}-directions (actually, since we use the AABB of the
     *       triangle we do not even need to test these)
     *    2) normal of the triangle
     *    3) cross product(edge from tri, {x,y,z}-direction)
     *       this gives 3x3=9 more tests
     */
    ScalarType min, max;
    PointType  normal;

    /* This is the fastest branch on Sun */
    /* move everything so that the boxCenter is in (0,0,0) */
    PointType v0 = triangle.point(0) - boxCenter;
    PointType v1 = triangle.point(1) - boxCenter;
    PointType v2 = triangle.point(2) - boxCenter;

    /* compute triangle edges */
    PointType e0 = v1 - v0;
    PointType e1 = v2 - v1;
    PointType e2 = v0 - v2;

    /* Bullet 3:  */
    /*  test the 9 tests first (this was faster) */
    ScalarType fex = std::abs(e0.x());
    ScalarType fey = std::abs(e0.y());
    ScalarType fez = std::abs(e0.z());

    if (!detail::axisTestX01(e0.z(), e0.y(), fez, fey, v0, v2, bHalfSize))
        return false;
    if (!detail::axisTestY02(e0.z(), e0.x(), fez, fex, v0, v2, bHalfSize))
        return false;
    if (!detail::axisTestZ12(e0.y(), e0.x(), fey, fex, v1, v2, bHalfSize))
        return false;

    fex = std::abs(e1.x());
    fey = std::abs(e1.y());
    fez = std::abs(e1.z());

    if (!detail::axisTestX01(e1.z(), e1.y(), fez, fey, v0, v2, bHalfSize))
        return false;
    if (!detail::axisTestY02(e1.z(), e1.x(), fez, fex, v0, v2, bHalfSize))
        return false;
    if (!detail::axisTestZ0(e1.y(), e1.x(), fey, fex, v0, v1, bHalfSize))
        return false;

    fex = std::abs(e2.x());
    fey = std::abs(e2.y());
    fez = std::abs(e2.z());
    if (!detail::axisTestX2(e2.z(), e2.y(), fez, fey, v0, v1, bHalfSize))
        return false;
    if (!detail::axisTestY1(e2.z(), e2.x(), fez, fex, v0, v1, bHalfSize))
        return false;
    if (!detail::axisTestZ12(e2.y(), e2.x(), fey, fex, v1, v2, bHalfSize))
        return false;

    /* Bullet 1:
     *  first test overlap in the {x,y,z}-directions
     *  find min, max of the triangle each direction, and test for overlap in
     *  that direction -- this is equivalent to testing a minimal AABB around
     *  the triangle against the AABB
     */

    /* test in X-direction */
    detail::findMinMax(v0.x(), v1.x(), v2.x(), min, max);
    if (min > bHalfSize.x() || max < -bHalfSize.x())
        return false;

    /* test in Y-direction */
    detail::findMinMax(v0.y(), v1.y(), v2.y(), min, max);
    if (min > bHalfSize.y() || max < -bHalfSize.y())
        return false;

    /* test in Z-direction */
    detail::findMinMax(v0.z(), v1.z(), v2.z(), min, max);
    if (min > bHalfSize.z() || max < -bHalfSize.z())
        return false;

    /* Bullet 2:
     *  test if the box intersects the plane of the triangle
     *  compute plane equation of triangle: normal*x+d=0
     */
    normal = e0.cross(e1);
    vcl::Plane<ScalarType> plane(
        triangle.point(0), triangle.point(1), triangle.point(2));
    if (!intersect(plane, box))
        return false;

    return true; /* box and triangle overlaps */
}

/**
 * @copydoc vcl::intersect(const TriangleType&, const BoxType&)
 *
 * @ingroup intersection_core
 */
template<Box3Concept BoxType, ConstTriangle3Concept TriangleType>
bool intersect(const BoxType& box, const TriangleType& triangle)
{
    return intersect(triangle, box);
}

/**
 * @brief Compute the intersection between a sphere and a triangle.
 *
 * @param[in] triangle: the 3D input triangle
 * @param[in] sphere: the input sphere
 * @param[out] witness: the point on the triangle nearest to the center of the
 * sphere (even when there isn't intersection)
 * @param[out] res: if not null, in the first item is stored the minimum
 * distance between the triangle and the sphere, while in the second item is
 * stored the penetration depth
 * @return true iff there is an intersection between the sphere and the triangle
 *
 * @ingroup intersection_core
 */
template<
    ConstTriangle3Concept TriangleType,
    SphereConcept         SphereType,
    Point3Concept         PointType,
    typename ScalarType>
bool intersect(
    const TriangleType&                triangle,
    const SphereType&                  sphere,
    PointType&                         witness,
    std::pair<ScalarType, ScalarType>& res)
{
    bool penetrationDetected = false;

    ScalarType radius = sphere.radius();
    PointType  center = sphere.center();
    PointType  p0     = triangle.point0() - center;
    PointType  p1     = triangle.point1() - center;
    PointType  p2     = triangle.point2() - center;

    PointType p10 = p1 - p0;
    PointType p21 = p2 - p1;
    PointType p20 = p2 - p0;

    ScalarType delta0_p01 = p10.dot(p1);
    ScalarType delta1_p01 = -p10.dot(p0);
    ScalarType delta0_p02 = p20.dot(p2);
    ScalarType delta2_p02 = -p20.dot(p0);
    ScalarType delta1_p12 = p21.dot(p2);
    ScalarType delta2_p12 = -p21.dot(p1);

    // the closest point can be one of the vertices of the triangle
    if (delta1_p01 <= ScalarType(0.0) && delta2_p02 <= ScalarType(0.0))
        witness = p0;
    else if (delta0_p01 <= ScalarType(0.0) && delta2_p12 <= ScalarType(0.0))
        witness = p1;
    else if (delta0_p02 <= ScalarType(0.0) && delta1_p12 <= ScalarType(0.0))
        witness = p2;
    else {
        ScalarType temp        = p10.dot(p2);
        ScalarType delta0_p012 = delta0_p01 * delta1_p12 + delta2_p12 * temp;
        ScalarType delta1_p012 = delta1_p01 * delta0_p02 - delta2_p02 * temp;
        ScalarType delta2_p012 =
            delta2_p02 * delta0_p01 - delta1_p01 * (p20.dot(p1));

        // otherwise, can be a point lying on same edge of the triangle
        if (delta0_p012 <= ScalarType(0.0)) {
            ScalarType denominator = delta1_p12 + delta2_p12;
            ScalarType mu1         = delta1_p12 / denominator;
            ScalarType mu2         = delta2_p12 / denominator;

            witness = (p1 * mu1 + p2 * mu2);
        }
        else if (delta1_p012 <= ScalarType(0.0)) {
            ScalarType denominator = delta0_p02 + delta2_p02;
            ScalarType mu0         = delta0_p02 / denominator;
            ScalarType mu2         = delta2_p02 / denominator;

            witness = (p0 * mu0 + p2 * mu2);
        }
        else if (delta2_p012 <= ScalarType(0.0)) {
            ScalarType denominator = delta0_p01 + delta1_p01;
            ScalarType mu0         = delta0_p01 / denominator;
            ScalarType mu1         = delta1_p01 / denominator;

            witness = (p0 * mu0 + p1 * mu1);
        }
        else {
            // or else can be an point detail to the triangle
            ScalarType denominator = delta0_p012 + delta1_p012 + delta2_p012;
            ScalarType lambda0     = delta0_p012 / denominator;
            ScalarType lambda1     = delta1_p012 / denominator;
            ScalarType lambda2     = delta2_p012 / denominator;

            witness = p0 * lambda0 + p1 * lambda1 + p2 * lambda2;
        }
    }

    ScalarType witness_norm = witness.norm();

    res.first  = std::max<ScalarType>(witness_norm - radius, ScalarType(0.0));
    res.second = std::max<ScalarType>(radius - witness_norm, ScalarType(0.0));

    penetrationDetected = (witness.squaredNorm() <= (radius * radius));
    witness += center;
    return penetrationDetected;
}

/**
 * @brief Compute the intersection between a sphere and a triangle.
 *
 * @param[in] triangle: the 3D input triangle
 * @param[in] sphere: the input sphere
 * @return true iff there is an intersection between the sphere and the triangle
 *
 * @ingroup intersection_core
 */
template<ConstTriangle3Concept TriangleType, SphereConcept SphereType>
bool intersect(const TriangleType& triangle, const SphereType& sphere)
{
    using SScalar = SphereType::ScalarType;
    typename TriangleType::PointType witness;
    std::pair<SScalar, SScalar>      res;
    return intersect(triangle, sphere, witness, res);
}

/**
 * @copydoc vcl::intersect(const TriangleType&, const SphereType&)
 *
 * @ingroup intersection_core
 */
template<SphereConcept SphereType, ConstTriangle3Concept TriangleType>
bool intersect(const SphereType& sphere, const TriangleType& t)
{
    return intersect(t, sphere);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_INTERSECTION_MISC_H
