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

#ifndef VCL_ALGORITHMS_INTERSECTION_MISC_H
#define VCL_ALGORITHMS_INTERSECTION_MISC_H

#include <vclib/space/box.h>
#include <vclib/space/plane.h>
#include <vclib/space/segment.h>
#include <vclib/space/sphere.h>
#include <vclib/space/triangle.h>

namespace vcl {

namespace detail {

// triangle box intersect functions
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
    const PointType& bHalfSixe)
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
    ScalarType rad = fa * bHalfSixe.y() + fb * bHalfSixe.z();
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
    const PointType& bHalfSixe)
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
    ScalarType rad = fa * bHalfSixe.y() + fb * bHalfSixe.z();
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
    const PointType& bHalfSixe)
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
    ScalarType rad = fa * bHalfSixe.x() + fb * bHalfSixe.z();
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
    const PointType& bHalfSixe)
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
    ScalarType rad = fa * bHalfSixe.x() + fb * bHalfSixe.z();
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
    const PointType& bHalfSixe)
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
    ScalarType rad = fa * bHalfSixe.x() + fb * bHalfSixe.y();
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
    const PointType& bHalfSixe)
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
    ScalarType rad = fa * bHalfSixe.x() + fb * bHalfSixe.y();
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
 * @tparam PlaneType: The type of plane used in the intersection check
 * @tparam BoxType: The type of box used in the intersection check, must satisfy
 * the Box3Concept
 *
 * @param[in] p: The plane to check intersection with
 * @param[in] box: The box to check intersection with
 * @return True if the plane intersects with the box, false otherwise
 */
template<PlaneConcept PlaneType, Box3Concept BoxType>
bool planeBoxIntersect(const PlaneType& p, const BoxType& box)
{
    using PointType  = BoxType::PointType;
    using ScalarType = PointType::ScalarType;

    // Convert AABB to center-extents representation
    PointType c = (box.max() + box.min()) * 0.5f; // Compute AABB center
    PointType e = box.max() - c;                  // Compute positive extents

    PointType n = p.direction();
    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    ScalarType r =
        e[0] * std::abs(n[0]) + e[1] * std::abs(n[1]) + e[2] * std::abs(n[2]);

    // Compute distance of box center from plane
    ScalarType s = n.dot(c) - p.offset();

    // Intersection occurs when distance s falls within [-r,+r] interval
    return std::abs(s) <= r;
}

/**
 * @brief This function computes the intersection between a plane and a 3D
 * segment. If the intersection exists, it returns true and stores the
 * intersection point in the output parameter intersection, otherwise, it
 * returns false.
 *
 * @param[in] p: the plane to compute the intersection with.
 * @param[in] s: the 3D segment to compute the intersection with.
 * @param[out] intersection: A reference to the point that stores the resulting
 * intersection between the plane and the segment. This parameter is only
 * written to if the function returns true.
 * @return A boolean value indicating whether an intersection was found or not.
 */
template<PlaneConcept PlaneType, Segment3Concept SegmentType>
bool planeSegmentIntersect(
    const PlaneType&                 p,
    const SegmentType&               s,
    typename SegmentType::PointType& intersection)
{
    using ScalarType = SegmentType::ScalarType;

    // Compute the projection of the segment endpoints onto the plane.
    ScalarType p1_proj = s.p1() * p.direction() - p.offset();
    ScalarType p0_proj = s.p0() * p.direction() - p.offset();

    // If both endpoints are on the same side of the plane, there is no
    // intersection.
    if ((p1_proj > 0) - (p0_proj < 0))
        return false;

    // If both endpoints have the same projection onto the plane, there is no
    // intersection.
    if (p0_proj == p1_proj)
        return false;

    // check that we perform the computation in a way that is independent with
    // v0 v1 swaps
    if (p0_proj < p1_proj)
        intersection = s.p0() + (s.p1() - s.p0()) *
                                    std::abs(p0_proj / (p1_proj - p0_proj));
    if (p0_proj > p1_proj)
        intersection = s.p1() + (s.p0() - s.p1()) *
                                    std::abs(p1_proj / (p0_proj - p1_proj));

    return true;
}

/**
 * @brief This function computes the intersection between a plane and a 3D
 * segment. If the intersection exists, it returns true, otherwise, it returns
 * false.
 *
 * @param[in] p: the plane to compute the intersection with.
 * @param[in] s: the 3D segment to compute the intersection with.
 * @return A boolean value indicating whether an intersection was found or not.
 */
template<PlaneConcept PlaneType, Segment3Concept SegmentType>
bool planeSegmentIntersect(const PlaneType& p, const SegmentType& s)
{
    typename SegmentType::PointType intersection;

    return planeSegmentIntersect(p, s, intersection);
}

/**
 * Checks if a sphere intersects with a box in 3D space.
 *
 * @tparam SphereType: A type that satisfies the SphereConcept concept.
 * @tparam BoxType: A type that satisfies the Box3Concept concept.
 *
 * @param[in] s: The sphere to check for intersection.
 * @param[in] box: The box to check for intersection.
 * @return true if the sphere intersects with the box, false otherwise.
 */
template<SphereConcept SphereType, Box3Concept BoxType>
bool sphereBoxIntersect(const SphereType& s, const BoxType& box)
{
    return s.intersects(box);
}

/**
 * Checks if a 2D point intersects with/is inside a given 2D triangle having its
 * points in counterclockwise order.
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
 * @param[in] tr: The triangle to test for intersection.
 * @param[in] p: The point to test for intersection with the triangle.
 * @return True if the point intersects with/is inside the triangle, false
 * otherwise.
 */
template<ConstTriangle2Concept TriangleType, Point2Concept PointType>
bool trianglePointIntersect(const TriangleType& tr, const PointType& p)
{
    using TP         = TriangleType::PointType;
    using ScalarType = TP::ScalarType;

    const TP& p0 = tr.point(0);
    const TP& p1 = tr.point(1);
    const TP& p2 = tr.point(2);

    ScalarType A    = tr.area();
    ScalarType sign = A < 0 ? -1 : 1;

    ScalarType s = (p0.y() * p2.x() - p0.x() * p2.y() +
                    (p2.y() - p0.y()) * p.x() + (p0.x() - p2.x()) * p.y()) *
                   sign;
    ScalarType t = (p0.x() * p1.y() - p0.y() * p1.x() +
                    (p0.y() - p1.y()) * p.x() + (p1.x() - p0.x()) * p.y()) *
                   sign;

    return s > 0 && t > 0 && (s + t) < 2 * A * sign;
}

/**
 * Checks if a 3D point intersects/is inside a 3D triangle having its points in
 * counterclockwise order.
 *
 * @tparam TriangleType: A type that satisfies the ConstTriangle3Concept
 * concept.
 * @tparam PointType: A type that satisfies the Point3Concept concept.
 *
 * @param[in] t: The triangle to test for intersection.
 * @param[in] p: The point to test for intersection with the triangle.
 * @return True if the point intersects with/is inside the triangle, false
 * otherwise.
 */
template<ConstTriangle3Concept TriangleType, Point3Concept PointType>
bool trianglePointIntersect(const TriangleType& t, const PointType& p)
{
    PointType v1 = t.point(1) - t.point(0);
    PointType v2 = t.point(2) - t.point(0);
    PointType v3 = p - t.point(0);

    return v1.dot(v2.cross(v3)) > 0;
}

/**
 * @brief Checks if a triangle intersects with a box.
 * https://gist.github.com/jflipts/fc68d4eeacfcc04fbdb2bf38e0911850
 */
template<ConstTriangle3Concept TriangleType, Box3Concept BoxType>
bool triangleBoxIntersect(const TriangleType& t, const BoxType& box)
{
    using PointType  = TriangleType::PointType;
    using ScalarType = PointType::ScalarType;

    PointType boxcenter = box.center();
    PointType bHalfSixe = box.size() / 2;

    /* use separating axis theorem to test overlap between triangle and box
     * need to test for overlap in these directions:
     *    1) the {x,y,z}-directions (actually, since we use the AABB of the
     *       triangle we do not even need to test these)
     *    2) normal of the triangle
     *    3) crossproduct(edge from tri, {x,y,z}-directin)
     *       this gives 3x3=9 more tests
     */
    ScalarType min, max;
    PointType  normal;

    /* This is the fastest branch on Sun */
    /* move everything so that the boxcenter is in (0,0,0) */
    PointType v0 = t.point(0) - boxcenter;
    PointType v1 = t.point(1) - boxcenter;
    PointType v2 = t.point(2) - boxcenter;

    /* compute triangle edges */
    PointType e0 = v1 - v0;
    PointType e1 = v2 - v1;
    PointType e2 = v0 - v2;

    /* Bullet 3:  */
    /*  test the 9 tests first (this was faster) */
    ScalarType fex = std::abs(e0.x());
    ScalarType fey = std::abs(e0.y());
    ScalarType fez = std::abs(e0.z());

    if (!detail::axisTestX01(e0.z(), e0.y(), fez, fey, v0, v2, bHalfSixe))
        return false;
    if (!detail::axisTestY02(e0.z(), e0.x(), fez, fex, v0, v2, bHalfSixe))
        return false;
    if (!detail::axisTestZ12(e0.y(), e0.x(), fey, fex, v1, v2, bHalfSixe))
        return false;

    fex = std::abs(e1.x());
    fey = std::abs(e1.y());
    fez = std::abs(e1.z());

    if (!detail::axisTestX01(e1.z(), e1.y(), fez, fey, v0, v2, bHalfSixe))
        return false;
    if (!detail::axisTestY02(e1.z(), e1.x(), fez, fex, v0, v2, bHalfSixe))
        return false;
    if (!detail::axisTestZ0(e1.y(), e1.x(), fey, fex, v0, v1, bHalfSixe))
        return false;

    fex = std::abs(e2.x());
    fey = std::abs(e2.y());
    fez = std::abs(e2.z());
    if (!detail::axisTestX2(e2.z(), e2.y(), fez, fey, v0, v1, bHalfSixe))
        return false;
    if (!detail::axisTestY1(e2.z(), e2.x(), fez, fex, v0, v1, bHalfSixe))
        return false;
    if (!detail::axisTestZ12(e2.y(), e2.x(), fey, fex, v1, v2, bHalfSixe))
        return false;

    /* Bullet 1:
     *  first test overlap in the {x,y,z}-directions
     *  find min, max of the triangle each direction, and test for overlap in
     *  that direction -- this is equivalent to testing a minimal AABB around
     *  the triangle against the AABB
     */

    /* test in X-direction */
    detail::findMinMax(v0.x(), v1.x(), v2.x(), min, max);
    if (min > bHalfSixe.x() || max < -bHalfSixe.x())
        return false;

    /* test in Y-direction */
    detail::findMinMax(v0.y(), v1.y(), v2.y(), min, max);
    if (min > bHalfSixe.y() || max < -bHalfSixe.y())
        return false;

    /* test in Z-direction */
    detail::findMinMax(v0.z(), v1.z(), v2.z(), min, max);
    if (min > bHalfSixe.z() || max < -bHalfSixe.z())
        return false;

    /* Bullet 2:
     *  test if the box intersects the plane of the triangle
     *  compute plane equation of triangle: normal*x+d=0
     */
    normal = e0.cross(e1);
    vcl::Plane<ScalarType> plane(t.point(0), t.point(1), t.point(2));
    if (!planeBoxIntersect(plane, box))
        return false;

    return true; /* box and triangle overlaps */
}

/**
 * @brief Compute the intersection between a sphere and a triangle.
 * @param[in] t: the 3D input triangle
 * @param[in] sphere: the input sphere
 * @param[out] witness: the point on the triangle nearest to the center of the
 * sphere (even when there isn't intersection)
 * @param[out] res: if not null, in the first item is stored the minimum
 * distance between the triangle and the sphere, while in the second item is
 * stored the penetration depth
 * @return true iff there is an intersection between the sphere and the triangle
 */
template<
    ConstTriangle3Concept TriangleType,
    SphereConcept         SphereType,
    Point3Concept         PointType,
    typename ScalarType>
bool triangleSphereItersect(
    const TriangleType&                t,
    const SphereType&                  sphere,
    PointType&                         witness,
    std::pair<ScalarType, ScalarType>& res)
{
    bool penetrationDetected = false;

    ScalarType radius = sphere.radius();
    PointType  center = sphere.center();
    PointType  p0     = t.point0() - center;
    PointType  p1     = t.point1() - center;
    PointType  p2     = t.point2() - center;

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
 * @param[in] t: the 3D input triangle
 * @param[in] sphere: the input sphere
 * @return true iff there is an intersection between the sphere and the triangle
 */
template<ConstTriangle3Concept TriangleType, SphereConcept SphereType>
bool triangleSphereItersect(const TriangleType& t, const SphereType& sphere)
{
    using SScalar = SphereType::ScalarType;
    typename TriangleType::PointType witness;
    std::pair<SScalar, SScalar>      res;
    return triangleBoxIntersect(t, sphere, witness, res);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_INTERSECTION_MISC_H
