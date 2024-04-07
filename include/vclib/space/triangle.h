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

#ifndef VCL_SPACE_TRIANGLE_H
#define VCL_SPACE_TRIANGLE_H

#include <vclib/concepts/space/triangle.h>
#include <vclib/space/point.h>

namespace vcl {

template<PointConcept PointT>
class Triangle
{
    std::array<PointT, 3> mPoints;

public:
    using ScalarType = PointT::ScalarType;

    using PointType = PointT;

    static const uint DIM = PointT::DIM;

    Triangle() = default;

    Triangle(const PointT& p0, const PointT& p1, const PointT& p2) :
            mPoints(p0, p1, p2)
    {
    }

    /**
     * @brief Returns the number of points of the triangle.
     * @return 3
     */
    constexpr uint size() const { return 3; }

    /**
     * @brief Returns the i-th point of the triangle.
     * @param[in] i: Index of the point to return.
     * @return The i-th point of the triangle.
     */
    const PointT& point(uint i) const { return mPoints.at(i); }

    PointT& point(uint i) { return mPoints.at(i); }

    PointT& point0() { return mPoints[0]; }

    const PointT& point0() const { return mPoints[0]; }

    PointT& point1() { return mPoints[1]; }

    const PointT& point1() const { return mPoints[1]; }

    PointT& point2() { return mPoints[2]; }

    const PointT& point2() const { return mPoints[2]; }

    /**
     * @brief Returns the length of the i-th side of the triangle.
     * @param[in] i: Index of the side.
     * @return The length of the i-th side of the triangle.
     */
    ScalarType sideLength(uint i) const
    {
        return sideLength(mPoints[0], mPoints[1], mPoints[2], i);
    }

    /**
     * @brief Returns the length of the first side of the triangle.
     * @return The length of the first side of the triangle.
     */
    ScalarType sideLength0() const { return sideLength(0); }

    /**
     * @brief Returns the length of the second side of the triangle.
     * @return The length of the second side of the triangle.
     */
    ScalarType sideLength1() const { return sideLength(1); }

    /**
     * @brief Returns the length of the third side of the triangle.
     * @return The length of the third side of the triangle.
     */
    ScalarType sideLength2() const { return sideLength(2); }

    /**
     * @brief Returns the normal of the triangle.
     * @note This function is available only if the dimension of the triangle
     * is 3.
     * @return The ormal of the triangle.
     */
    PointT normal() const requires (PointT::DIM == 3)
    {
        return normal(mPoints[0], mPoints[1], mPoints[2]);
    }

    /**
     * @brief Computes the barycenter of the triangle.
     *
     * @return The barycenter of the triangle.
     */
    PointT barycenter() const
    {
        return barycenter(mPoints[0], mPoints[1], mPoints[2]);
    }

    /**
     * @brief Computes the weighted barycenter of the triangle.
     *
     * Given three weights corresponding to the three triangle points, this
     * function computes the weighted barycenter of the triangle.
     *
     * @param[in] w0: The weight of the first point of the triangle.
     * @param[in] w1: The weight of the second point of the triangle.
     * @param[in] w2: The weight of the third point of the triangle.
     * @return The weighted barycenter of the triangle.
     */
    PointT weightedBarycenter(ScalarType w0, ScalarType w1, ScalarType w2) const
    {
        return weightedBarycenter(
            mPoints[0], mPoints[1], mPoints[2], w0, w1, w2);
    }

    PointT weightedBarycenter(const Point3<ScalarType>& w) const
    {
        return weightedBarycenter(w(0), w(1), w(2));
    }

    PointT barycentricCoordinatePoint(
        ScalarType b0,
        ScalarType b1,
        ScalarType b2) const
    {
        return barycentricCoordinatePoint(
            mPoints[0], mPoints[1], mPoints[2], b0, b1, b2);
    }

    /**
     * @brief Computes the point in the triangle with the given barycentric
     * coordinates.
     *
     * Given a set of barycentric coordinates \p b, this function computes
     * the point in the triangle corresponding to those barycentric coordinates.
     *
     * @param[in] b: The barycentric coordinates of the point in the triangle of
     * type Point3<ScalarType>.
     *
     * @return The point in the triangle corresponding to the given barycentric
     * coordinates.
     */
    PointT barycentricCoordinatePoint(const Point3<ScalarType>& b) const
    {
        return barycentricCoordinatePoint(
            mPoints[0], mPoints[1], mPoints[2], b(0), b(1), b(2));
    }

    /**
     * @brief Compute the circumcenter of the triangle.
     *
     * The function computes the circumcenter of the triangle, which is the
     * center of the circle that passes through the three vertices of the
     * triangle. The circumcenter is defined as the intersection of the
     * perpendicular bisectors of the three sides of the triangle.
     *
     * @return The circumcenter of the triangle.
     *
     * @note The function assumes that the three points are not collinear and
     * form a valid triangle.
     */
    PointT circumcenter() const
    {
        return circumcenter(mPoints[0], mPoints[1], mPoints[2]);
    }

    /**
     * @brief Computes the perimeter of the triangle.
     *
     * @return The perimeter of the triangle.
     */
    ScalarType perimeter() const
    {
        return perimeter(mPoints[0], mPoints[1], mPoints[2]);
    }

    /**
     * @brief Computes the area of the triangle.
     *
     * @return The area of the triangle.
     */
    ScalarType area() const { return area(mPoints[0], mPoints[1], mPoints[2]); }

    /**
     * @brief Calculates the quality measure of the triangle.
     *
     * The quality measure is computed as 2 times the triangle's area divided by
     * the square of the length of its longest edge. The resulting value is in
     * the range [0.0, 0.866], where 0.0 represents a degenerate triangle and
     * 0.866 represents an equilateral triangle.
     *
     * @return The quality measure of the triangle.
     *
     * @note If the area of the triangle is zero (i.e., the vertices are
     * collinear), the function returns 0.0.
     */
    ScalarType quality() const
    {
        return quality(mPoints[0], mPoints[1], mPoints[2]);
    }

    /**
     * @brief Compute a shape quality measure of the triangle.
     *
     * This function computes the quality measure of a triangle based on the
     * ratio between its inradius and circumradius.
     *
     * @return A value between 0 and 1 that represents the quality of the
     * triangle shape. A value of 1 means the triangle is perfectly equilateral,
     * while lower values indicate more distorted shapes (i.e., halfsquare:
     * 0.81). A value of 0 is returned when the triangle is degenerate (i.e., it
     * has zero area).
     */
    ScalarType qualityRadii() const
    {
        return qualityRadii(mPoints[0], mPoints[1], mPoints[2]);
    }

    /**
     * @brief Compute the mean ratio of the triangle shape quality measure.
     *
     * The function computes the mean ratio shape quality measure of the
     * triangle. The mean ratio is defined as 2 * sqrt(a * b) / (a + b), where a
     * and b are the eigenvalues of the M^tM transformation matrix into a
     * regular simplex. The mean ratio is a scalar value in the range [0, 1]
     * that describes the triangle quality, where 0 means the triangle is
     * degenerate, and 1 means it is perfectly equilateral.
     *
     * @return The mean ratio shape quality measure of the triangle.
     *
     * @note If the three points are collinear or form a degenerate triangle,
     * the function returns 0.
     */
    ScalarType qualityMeanRatio() const
    {
        return qualityMeanRatio(mPoints[0], mPoints[1], mPoints[2]);
    }

    /* Static member functions */

    /**
     * @brief Returns the length of the i-th side of the triangle composed by
     * the points \p p0, \p p1, and \p p2.
     *
     * @param[in] p0: first point of the triangle.
     * @param[in] p1: second point of the triangle.
     * @param[in] p2: third point of the triangle.
     * @param[in] i: Index of the side.
     * @return The length of the i-th side of the triangle.
     */
    ScalarType sideLength(
        const PointT& p0,
        const PointT& p1,
        const PointT& p2,
        uint          i) const
    {
        switch (i % 3) {
        case 0: return p0.dist(p1);
        case 1: return p1.dist(p2);
        case 2: return p2.dist(p0);
        default: throw std::out_of_range("Invalid side index");
        }
    }

    /**
     * @brief Computes the normal of the triangle composed by the 3D points \p
     * p0, \p p1, and \p p2, considering that these three points are ordered in
     * counter-clockwise order.
     *
     * @tparam PointType: A type that satisfies the PointConcept.
     *
     * @param[in] p0: first point of the triangle.
     * @param[in] p1: second point of the triangle.
     * @param[in] p2: third point of the triangle.
     * @return The normal of the triangle composed by \p p0, \p p1 and \p p2.
     */
    static PointT normal(const PointT& p0, const PointT& p1, const PointT& p2)
        requires (PointT::DIM == 3)
    {
        return (p1 - p0).cross(p2 - p0);
    }

    /**
     * @brief Computes the barycenter of the triangle composed by the points \p
     * p0, \p p1, and \p p2.
     *
     * @param[in] p0: first point of the triangle.
     * @param[in] p1: second point of the triangle.
     * @param[in] p2: third point of the triangle.
     * @return The barycenter of the triangle composed by \p p0, \p p1 and \p
     * p2.
     */
    static PointT barycenter(
        const PointT& p0,
        const PointT& p1,
        const PointT& p2)
    {
        return (p0 + p1 + p2) / 3;
    }

    /**
     * @brief Computes the weighted barycenter of a triangle composed of three
     * points.
     *
     * Given three points and their corresponding weights, this function
     * computes the weighted barycenter of the triangle they form.
     *
     * @param[in] p0: The first point of the triangle.
     * @param[in] w0: The weight of the first point of the triangle.
     * @param[in] p1: The second point of the triangle.
     * @param[in] w1: The weight of the second point of the triangle.
     * @param[in] p2: The third point of the triangle.
     * @param[in] w2: The weight of the third point of the triangle.
     * @return The weighted barycenter of the triangle formed by p0, p1 and p2.
     */
    static PointT weightedBarycenter(
        const PointT& p0,
        const PointT& p1,
        const PointT& p2,
        ScalarType    w0,
        ScalarType    w1,
        ScalarType    w2)
    {
        return (p0 * w0 + p1 * w1 + p2 * w2) / (w0 + w1 + w2);
    }

    /**
     * @brief Computes the point in a triangle with the given barycentric
     * coordinates.
     *
     * Given a triangle with vertices \p p0, \p p1, and \p p2, and a set of
     * barycentric coordinates \p b0, \p b1 and \p b2, this function computes
     * the point in the triangle corresponding to those barycentric coordinates.
     *
     * @param[in] p0: The first vertex of the triangle.
     * @param[in] p1: The second vertex of the triangle.
     * @param[in] p2: The third vertex of the triangle.
     * @param[in] b0: The first barycentric coordinate of the point in the
     * triangle.
     * @param[in] b1: The first barycentric coordinate of the point in the
     * triangle.
     * @param[in] b2: The first barycentric coordinate of the point in the
     * triangle.
     *
     * @return The point in the triangle corresponding to the given barycentric
     * coordinates.
     */
    static PointT barycentricCoordinatePoint(
        const PointT& p0,
        const PointT& p1,
        const PointT& p2,
        ScalarType    b0,
        ScalarType    b1,
        ScalarType    b2)
    {
        return p0 * b0 + p1 * b1 + p2 * b2;
    }

    /**
     * @brief Compute the circumcenter of a triangle.
     *
     * Given three points \p p0, \p p1, \p p2 representing a triangle, the
     * function computes the circumcenter of the triangle, which is the center
     * of the circle that passes through the three vertices of the triangle. The
     * circumcenter is defined as the intersection of the perpendicular
     * bisectors of the three sides of the triangle.
     *
     * @tparam PointType: A type that satisfies the PointConcept.
     *
     * @param[in] p0: The first point of the triangle.
     * @param[in] p1: The second point of the triangle.
     * @param[in] p2: The third point of the triangle.
     * @return The circumcenter of the triangle.
     *
     * @note The function assumes that the three points are not collinear and
     * form a valid triangle.
     */
    static PointT circumcenter(
        const PointT& p0,
        const PointT& p1,
        const PointT& p2)
    {
        ScalarType a2 = p1.squaredDist(p2);
        ScalarType b2 = p2.squaredDist(p0);
        ScalarType c2 = p0.squaredDist(p1);

        PointType c = p0 * a2 * (-a2 + b2 + c2) + p1 * b2 * (a2 - b2 + c2) +
                      p2 * c2 * (a2 + b2 - c2);

        c /= 2 * (a2 * b2 + a2 * c2 + b2 * c2) - a2 * a2 - b2 * b2 - c2 * c2;

        return c;
    }

    /**
     * @brief Computes the perimeter of the triangle composed by the points \p
     * p0, \p p1, and \p p2.
     *
     * @param[in] p0: first point of the triangle.
     * @param[in] p1: second point of the triangle.
     * @param[in] p2: third point of the triangle.
     * @return The perimeter of the triangle composed by \p p0, \p p1, and \p
     * p2.
     */
    static ScalarType perimeter(
        const PointT& p0,
        const PointT& p1,
        const PointT& p2)
    {
        return p0.dist(p1) + p1.dist(p2) + p2.dist(p0);
    }

    /**
     * @brief Computes the area of the triangle composed by the points \p p0, \p
     * p1, and \p p2, considering that these three points are ordered in
     * counterclockwise order.
     *
     * @param[in] p0: First point of the triangle.
     * @param[in] p1: Second point of the triangle.
     * @param[in] p2: Third point of the triangle.
     *
     * @return The area of the triangle composed by \p p0, \p p1, and \p p2.
     */
    static ScalarType area(const PointT& p0, const PointT& p1, const PointT& p2)
    {
        if constexpr (DIM == 2) {
            return ((p1[0] - p0[0]) * (p2[1] - p0[1]) -
                    (p2[0] - p0[0]) * (p1[1] - p0[1])) /
                   2;
        }
        if constexpr (DIM == 3) {
            return normal(p0, p1, p2).norm() / 2;
        }
        else {
            // heron's formula
            ScalarType s = perimeter() / 2;
            return std::sqrt(
                s * (s - p0.dist(p1)) * (s - p1.dist(p2)) * (s - p2.dist(p0)));
        }
    }

    /**
     * @brief Calculates the quality measure of a triangle, given its three
     * vertices.
     *
     * The quality measure is computed as 2 times the triangle's area divided by
     * the square of the length of its longest edge. The resulting value is in
     * the range [0.0, 0.866], where 0.0 represents a degenerate triangle and
     * 0.866 represents an equilateral triangle.
     *
     * @param[in] p0: The first vertex of the triangle.
     * @param[in] p1: The second vertex of the triangle.
     * @param[in] p2: The third vertex of the triangle.
     * @return The quality measure of the triangle.
     *
     * @note If the area of the triangle is zero (i.e., the vertices are
     * collinear), the function returns 0.0.
     */
    static ScalarType quality(
        const PointT& p0,
        const PointT& p1,
        const PointT& p2)
    {
        ScalarType a = area();
        if (a == 0)
            return 0; // Area zero triangles have surely quality==0;

        PointType d10 = p1 - p0;
        PointType d20 = p2 - p0;
        PointType d12 = p1 - p2;
        double    b =
            std::min({d10.squaredNorm(), d20.squaredNorm(), d12.squaredNorm()});
        if (b == 0)
            return 0; // Again: area zero triangles have surely quality==0;
        return (2 * a) / b;
    }

    /**
     * @brief Compute a shape quality measure of the triangle composed by points
     * \p p0, \p p1, \p p2.
     *
     * This function computes the quality measure of a triangle based on the
     * ratio between its inradius and circumradius. The function takes three
     * points (\p p0, \p p1, \p p2) representing the vertices of the triangle.
     *
     * @param[in] p0: The first vertex of the triangle.
     * @param[in] p1: The second vertex of the triangle.
     * @param[in] p2: The third vertex of the triangle.
     * @return A value between 0 and 1 that represents the quality of the
     * triangle shape. A value of 1 means the triangle is perfectly equilateral,
     * while lower values indicate more distorted shapes (i.e., halfsquare:
     * 0.81). A value of 0 is returned when the triangle is degenerate (i.e., it
     * has zero area).
     */
    static ScalarType qualityRadii(
        const PointT& p0,
        const PointT& p1,
        const PointT& p2)
    {
        double a = p0.dist(p1);
        double b = p2.dist(p0);
        double c = p1.dist(p2);

        double sum   = (a + b + c) * 0.5;
        double area2 = sum * (a + b - sum) * (a + c - sum) * (b + c - sum);

        if (area2 <= 0)
            return 0;

        return (8 * area2) / (a * b * c * sum);
    }

    /**
     * @brief Compute the mean ratio of a triangle shape quality measure.
     *
     * Given three points \p p0, \p p1, \p p2 representing a triangle, the
     * function computes the mean ratio shape quality measure of the triangle.
     * The mean ratio is defined as 2 * sqrt(a * b) / (a + b), where a and b are
     * the eigenvalues of the M^tM transformation matrix into a regular simplex.
     * The mean ratio is a scalar value in the range [0, 1] that describes the
     * triangle quality, where 0 means the triangle is degenerate, and 1 means
     * it is perfectly equilateral.
     *
     * @param[in] p0 The first point of the triangle.
     * @param[in] p1 The second point of the triangle.
     * @param[in] p2 The third point of the triangle.
     * @return The mean ratio shape quality measure of the triangle.
     *
     * @note The function assumes that the three points are not collinear and
     * form a valid triangle. If the three points are collinear or form a
     * degenerate triangle, the function returns 0.
     */
    static ScalarType qualityMeanRatio(
        const PointT& p0,
        const PointT& p1,
        const PointT& p2)
    {
        double a = p0.dist(p1);
        double b = p2.dist(p0);
        double c = p1.dist(p2);

        double sum   = (a + b + c) * 0.5; // semiperimeter
        double area2 = sum * (a + b - sum) * (a + c - sum) * (b + c - sum);
        if (area2 <= 0)
            return 0;
        return (4.0 * std::sqrt(3.0) * std::sqrt(area2)) /
               (a * a + b * b + c * c);
    }
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
