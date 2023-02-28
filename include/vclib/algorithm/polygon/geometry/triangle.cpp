/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#include "triangle.h"

namespace vcl {

/**
 * @brief Computes the normal of the triangle composed by the points p0, p1, and p2, considering
 * that these three points are ordered in counterclockwise order.
 *
 * @tparam PointType A type that satisfies the PointConcept.
 * @param[in] p0: first point of the triangle.
 * @param[in] p1: second point of the triangle.
 * @param[in] p2: third point of the triangle.
 * @return The normal of the triangle composed by p0, p1 and p2.
 */
template<PointConcept PointType>
PointType triangleNormal(const PointType& p0, const PointType& p1, const PointType& p2)
{
	return (p1 - p0).cross(p2 - p0);
}

/**
 * @brief Computes the normal of a Face that is a triangle. Does not modify the triangle.
 * This function could accept also polygonal faces, but it will take into account only the first 3
 * vertices of the polygon. In this case, the resulting normal could be flipped w.r.yt. the actual
 * normal of the polygon.
 *
 * @param[in] t: input triangle of type Face
 * @return The normal of t.
 */
template<FaceConcept Triangle>
typename Triangle::VertexType::CoordType triangleNormal(const Triangle& t)
{
	return triangleNormal(t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

/**
 * @brief Computes the barycenter of the triangle composed by the points p0, p1, and p2.
 *
 * @tparam PointType A type that satisfies the PointConcept.
 * @param[in] p0: first point of the triangle.
 * @param[in] p1: second point of the triangle.
 * @param[in] p2: third point of the triangle.
 * @return The barycenter of the triangle composed by p0, p1 and p2.
 */
template <PointConcept PointType>
PointType triangleBarycenter(const PointType& p0, const PointType& p1, const PointType& p2)
{
	return (p0 + p1 + p2) / 3;
}

/**
 * @brief Computes the barycenter of a Face that is a triangle. This function could accept also
 * polygonal faces, but it will take into account only the first 3 vertices of the polygon. In this
 * case, the resulting barycenter won't be accurate.
 *
 * @param[in] t: input triangle of type Face
 * @return The barycenter of t.
 */
template<FaceConcept Triangle>
typename Triangle::VertexType::CoordType triangleBarycenter(const Triangle& t)
{
	return triangleBarycenter(t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

/**
 * @brief Computes the weighted barycenter of the triangle composed by the points p0, p1, and p2.
 *
 * @tparam PointType A type that satisfies the PointConcept.
 * @param[in] p0: first point of the triangle.
 * @param[in] w0: weight of the first point of the triangle.
 * @param[in] p1: second point of the triangle.
 * @param[in] w1: weight of the second point of the triangle.
 * @param[in] p2: third point of the triangle.
 * @param[in] w2: weight of the third point of the triangle.
 * @return The weighted barycenter of the triangle composed by p0, p1 and p2.
 */
template <PointConcept PointType>
PointType triangleWeightedBarycenter(
	const PointType& p0, typename PointType::ScalarType w0,
	const PointType& p1, typename PointType::ScalarType w1,
	const PointType& p2, typename PointType::ScalarType w2)
{
	return (p0 * w0 + p1 * w1 + p2 * w2) / (w0 + w1 + w2);
}

template<PointConcept PointType, typename ScalarType>
PointType triangleBarycentricCoordinatePoint(
	const PointType& p0,
	const PointType& p1,
	const PointType& p2,
	const Point3<ScalarType> &barCoords)
{
	return p0 * barCoords(0) + p1 * barCoords(1) + p2 * barCoords(2);
}

template <FaceConcept Triangle, typename ScalarType>
typename Triangle::CoordType triangleBarycentricCoordinatePoint(
	const Triangle& t,
	const Point3<ScalarType>& barCoords)
{
	return triangleBarycentricCoordinatePoint(
		t.vertex->coord(0), t.vertex->coord(1), t.vertex->coord(2), barCoords);
}

/**
 * @brief Computes the area of the triangle composed by the points p0, p1, and p2, considering
 * that these three points are ordered in counterclockwise order.
 *
 * @tparam PointType A type that satisfies the PointConcept.
 * @param[in] p0: first point of the triangle.
 * @param[in] p1: second point of the triangle.
 * @param[in] p2: third point of the triangle.
 * @return The area of the triangle composed by p0, p1 and p2.
 */
template<PointConcept PointType>
typename PointType::ScalarType
triangleArea(const PointType& p0, const PointType& p1, const PointType& p2)
{
	return triangleNormal(p0, p1, p2).norm() / 2;
}

/**
 * @brief Computes the area of a Face that is a triangle. This function could accept also
 * polygonal faces, but it will take into account only the first 3 vertices of the polygon. In this
 * case, the resulting area won't be accurate.
 *
 * @param[in] t: input triangle of type Face
 * @return The area of t.
 */
template<FaceConcept Triangle>
auto triangleArea(const Triangle& t)
{
	return triangleArea(t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

template<PointConcept PointType>
typename PointType::ScalarType
trianglePerimeter(const PointType& p0, const PointType& p1, const PointType& p2)
{
	return p0.dist(p1) + p1.dist(p2) + p2.dist(p0);
}

template<FaceConcept Triangle>
auto trianglePerimeter(const Triangle& t)
{
	return trianglePerimeter(t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

/**
 * Calculates the quality measure of a triangle, given its three vertices.
 *
 * The quality measure is computed as 2 times the triangle's area divided by the square of the
 * length of its longest edge. The resulting value is in the range [0.0, 0.866], where 0.0
 * represents a degenerate triangle and 0.866 represents an equilateral triangle.
 *
 * @tparam PointType A type that satisfies the PointConcept.
 * @param[in] p0 The first vertex of the triangle.
 * @param[in] p1 The second vertex of the triangle.
 * @param[in] p2 The third vertex of the triangle.
 * @return The quality measure of the triangle.
 * @note If the area of the triangle is zero (i.e., the vertices are collinear), the function
 * returns 0.0.
 */
template <PointConcept PointType>
double triangleQuality(const PointType& p0, const PointType& p1, const PointType& p2)
{
	PointType d10=p1-p0;
	PointType d20=p2-p0;
	PointType d12=p1-p2;
	PointType x = d10^d20;

	double a = x.norm();
	if (a == 0)
		return 0; // Area zero triangles have surely quality==0;
	double b = std::min({d10.squaredNorm(), d20.squaredNorm(), d12.squaredNorm()});
	if (b == 0)
		return 0; // Again: area zero triangles have surely quality==0;
	return a / b;
}

/**
 * Calculates the quality measure of a a Face that is a triangle. Does not modify the triangle.
 *
 * The quality measure is computed as 2 times the triangle's area divided by the square of the
 * length of its longest edge. The resulting value is in the range [0.0, 0.866], where 0.0
 * represents a degenerate triangle and 0.866 represents an equilateral triangle.
 *
 *
 * @tparam Triangle The type of the Face, that satisfies the FaceConcept.
 * @param[in] t The input triangle face.
 * @return The quality measure of the triangle.
 * @note If the area of the triangle is zero (i.e., the vertices are collinear), the function
 * returns 0.0.
 */
template<FaceConcept Triangle>
double triangleQuality(const Triangle& t)
{
	return triangleQuality(t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

/**
 * @brief Compute a shape quality measure of the triangle composed by points p0, p1, p2.
 *
 * This function computes the quality measure of a triangle based on the ratio between its inradius
 * and circumradius. The function takes three points (p0, p1, p2) representing the vertices of the
 * triangle.
 *
 * @tparam PointType A type that satisfies the PointConcept.
 * @param[in] p0 The first vertex of the triangle.
 * @param[in] p1 The second vertex of the triangle.
 * @param[in] p2 The third vertex of the triangle.
 * @return A value between 0 and 1 that represents the quality of the triangle shape. A value of 1
 * means the triangle is perfectly equilateral, while lower values indicate more distorted shapes
 * (i.e., halfsquare: 0.81). A value of 0 is returned when the triangle is degenerate (i.e., it has
 * zero area).
 */
template <PointConcept PointType>
double triangleQualityRadii(const PointType& p0, const PointType& p1, const PointType& p2)
{
	double a = (p1 - p0).norm();
	double b = (p2 - p0).norm();
	double c = (p1 - p2).norm();

	double sum = (a + b + c) * 0.5;
	double area2 = sum * (a + b - sum) * (a + c - sum) * (b + c - sum);

	if (area2 <= 0)
		return 0;

	return (8 * area2) / (a * b * c * sum);
}

/**
 * @brief Compute a shape quality measure of a Face that is a triangle. Does not modify the
 * triangle.
 *
 * This function computes the quality measure of a triangle based on the ratio between its inradius
 * and circumradius. The function takes a FaceConcept Face.
 *
 * @tparam Triangle The type of the Face, that must satisfy the FaceConcept concept.
 * @param[in] t The input triangle face.
 * @return A value between 0 and 1 that represents the quality of the triangle shape. A value of 1
 * means the triangle is perfectly equilateral, while lower values indicate more distorted shapes
 * (i.e., halfsquare: 0.81). A value of 0 is returned when the triangle is degenerate (i.e., it has
 * zero area).
 */
template<FaceConcept Triangle>
double triangleQualityRadii(const Triangle& t)
{
	return triangleQualityRadii(t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

/**
 * @brief Compute the mean ratio of a triangle shape quality measure.
 *
 * Given three points p0, p1, p2 representing a triangle, the function computes the mean ratio
 * shape quality measure of the triangle. The mean ratio is defined as 2 * sqrt(a * b) / (a + b),
 * where a and b are the eigenvalues of the M^tM transformation matrix into a regular simplex.
 * The mean ratio is a scalar value in the range [0, 1] that describes the triangle quality,
 * where 0 means the triangle is degenerate, and 1 means it is perfectly equilateral.
 *
 * @tparam PointType A type that satisfies the PointConcept requirements.
 * @param[in] p0 The first point of the triangle.
 * @param[in] p1 The second point of the triangle.
 * @param[in] p2 The third point of the triangle.
 * @return The mean ratio shape quality measure of the triangle.
 * @note The function assumes that the three points are not collinear and form a valid triangle.
 * If the three points are collinear or form a degenerate triangle, the function returns 0.
 */
template <PointConcept PointType>
double triangleQualityMeanRatio(const PointType& p0, const PointType& p1, const PointType& p2)
{
	double a = (p1 - p0).norm();
	double b = (p2 - p0).norm();
	double c = (p1 - p2).norm();

	double sum   = (a + b + c) * 0.5; // semiperimeter
	double area2 = sum * (a + b - sum) * (a + c - sum) * (b + c - sum);
	if (area2 <= 0)
		return 0;
	return (4.0 * std::sqrt(3.0) * std::sqrt(area2)) / (a * a + b * b + c * c);
}

/**
 * @brief Compute the mean ratio of a Face that is a triangle. Does not modify the triangle.
 *
 * Given the triangle t, the function computes the mean ratio shape quality measure of the triangle.
 * The mean ratio is defined as 2 * sqrt(a * b) / (a + b), where a and b are the eigenvalues of the
 * M^tM transformation matrix into a regular simplex. The mean ratio is a scalar value in the range
 * [0, 1] that describes the triangle quality, where 0 means the triangle is degenerate, and 1 means
 * it is perfectly equilateral.
 *
 * @tparam Triangle The type of the Face, that must satisfy the FaceConcept concept.
 * @param[in] t The input triangle face.
 * @return The mean ratio shape quality measure of the triangle.
 * @note The function assumes that the three points are not collinear and form a valid triangle.
 * If the three points are collinear or form a degenerate triangle, the function returns 0.
 */
template<FaceConcept Triangle>
double triangleQualityMeanRatio(const Triangle& t)
{
	return triangleQualityMeanRatio(
		t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

/**
 * @brief Compute the circumcenter of a triangle.
 *
 * Given three points p0, p1, p2 representing a triangle, the function computes the circumcenter
 * of the triangle, which is the center of the circle that passes through the three vertices of
 * the triangle. The circumcenter is defined as the intersection of the perpendicular bisectors
 * of the three sides of the triangle.
 *
 * @tparam PointType A type that satisfies the PointConcept.
 * @param[in] p0 The first point of the triangle.
 * @param[in] p1 The second point of the triangle.
 * @param[in] p2 The third point of the triangle.
 * @return The circumcenter of the triangle.
 * @note The function assumes that the three points are not collinear and form a valid triangle.
 */
template <PointConcept PointType>
PointType triangleCircumcenter(const PointType& p0, const PointType& p1, const PointType& p2)
{
	using ScalarType = typename PointType::ScalarType;

	ScalarType a2 = (p1 - p2).squaredNorm();
	ScalarType b2 = (p2 - p0).squaredNorm();
	ScalarType c2 = (p0 - p2).squaredNorm();

	PointType  c  = p0 * a2 * (-a2 + b2 + c2) + p1 * b2 * (a2 - b2 + c2) + p2 * c2 * (a2 + b2 - c2);
	c /= 2 * (a2 * b2 + a2 * c2 + b2 * c2) - a2 * a2 - b2 * b2 - c2 * c2;

	return c;
}

/**
 * @brief Compute the circumcenter of a Face that is a triangle. Does not modify the triangle.
 *
 * Given the triangle t, the function computes the circumcenter of the triangle, which is the center
 * of the circle that passes through the three vertices of the triangle. The circumcenter is defined
 * as the intersection of the perpendicular bisectors of the three sides of the triangle.
 *
 * @tparam Triangle The type of the Face, that must satisfy the FaceConcept concept.
 * @param[in] t The input triangle face.
 * @return The circumcenter of the triangle.
 * @note The function assumes that the three points are not collinear and form a valid triangle.
 */
template<FaceConcept Triangle>
typename Triangle::VertexType::CoordType triangleCircumcenter(const Triangle& t)
{
	return triangleCircumcenter(t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

} // namespace vcl
