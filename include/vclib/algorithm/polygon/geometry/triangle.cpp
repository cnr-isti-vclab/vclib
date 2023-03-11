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
 * @brief Computes the normal of a triangle face.
 *
 * Given a triangle face, this function computes its normal vector without modifying the input.
 * If a polygonal face is provided, only the first three vertices of the face are taken into
 * account. In such cases, the resulting normal may be flipped with respect to the actual normal of
 * the polygon.
 *
 * @tparam Triangle: A type that satisfies the FaceConcept. The type should have a nested type
 *         called VertexType, which satisfies the VertexConcept. The VertexType should have a nested
 *         type called CoordType, which represents the type of the vertex coordinates.
 *
 * @param[in] t: The input triangle face of type Face.
 * @return The normal vector of the input triangle face.
 *
 * @note The input triangle is not modified by this function.
 */
template<FaceConcept Triangle>
typename Triangle::VertexType::CoordType triangleNormal(const Triangle& t)
{
	using CoordType = typename Triangle::VertexType::CoordType;
	return vcl::Triangle<CoordType>::normal(
		t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

/**
 * @brief Computes the barycenter of a triangle face.
 *
 * Given a triangle face, this function computes its barycenter without modifying the input.
 * If a polygonal face is provided, only the first three vertices of the face are taken into
 * account. In such cases, the resulting barycenter may not be accurate.
 *
 * @tparam Triangle A type that satisfies the FaceConcept. The type should have a nested type
 *         called VertexType, which satisfies the VertexConcept. The VertexType should have a nested
 *         type called CoordType, which represents the type of the vertex coordinates.
 *
 * @param[in] t: The input triangle face of type Face.
 * @return The barycenter of the input triangle face.
 *
 * @note The input triangle is not modified by this function.
 */
template<FaceConcept Triangle>
typename Triangle::VertexType::CoordType triangleBarycenter(const Triangle& t)
{
	using CoordType = typename Triangle::VertexType::CoordType;
	return vcl::Triangle<CoordType>::barycenter(
		t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

/**
 * @brief Computes the point in barycentric coordinates of the given triangle.
 *
 * @tparam Triangle: A type that satisfies the FaceConcept.
 * @tparam ScalarType: The type of the scalar value used to represent the coordinates.
 *
 * @param[in] t: the triangle to compute the barycentric coordinate point for.
 * @param[in] barCoords: the barycentric coordinates of the point.
 * @return The point in Cartesian coordinates.
 */
template <FaceConcept Triangle, typename ScalarType>
typename Triangle::CoordType triangleBarycentricCoordinatePoint(
	const Triangle& t,
	const Point3<ScalarType>& barCoords)
{
	using CoordType = typename Triangle::VertexType::CoordType;
	return vcl::Triangle<CoordType>::barycentricCoordinatePoint(
		t.vertex->coord(0), t.vertex->coord(1), t.vertex->coord(2),
		barCoords(0), barCoords(1), barCoords(2));
}

/**
 * @brief Compute the circumcenter of a Face that is a triangle. Does not modify the triangle.
 *
 * Given the triangle \p t, the function computes the circumcenter of the triangle, which is the
 * center of the circle that passes through the three vertices of the triangle. The circumcenter is
 * defined as the intersection of the perpendicular bisectors of the three sides of the triangle.
 *
 * @tparam Triangle: The type of the Face, that must satisfy the FaceConcept concept.
 *
 * @param[in] t: The input triangle face.
 * @return The circumcenter of the triangle.
 *
 * @note The function assumes that the three points are not collinear and form a valid triangle.
 */
template<FaceConcept Triangle>
typename Triangle::VertexType::CoordType triangleCircumcenter(const Triangle& t)
{
	using CoordType = typename Triangle::VertexType::CoordType;
	return vcl::Triangle<CoordType>::circumcenter(
		t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

/**
 * @brief Computes the perimeter of a triangle given its vertices as a Face of type Triangle.
 *
 * @tparam Triangle: A type that satisfies the FaceConcept.
 *
 * @param[in] t: The Triangle face whose perimeter is to be computed.
 * @return The perimeter of the triangle.
 */
template<FaceConcept Triangle>
auto trianglePerimeter(const Triangle& t)
{
	using CoordType = typename Triangle::VertexType::CoordType;
	return vcl::Triangle<CoordType>::perimeter(
		t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

/**
 * @brief Computes the area of a Face that is a triangle. This function could accept also
 * polygonal faces, but it will take into account only the first 3 vertices of the polygon. In this
 * case, the resulting area won't be accurate.
 *
 * @tparam Triangle: A type that satisfies the FaceConcept.
 *
 * @param[in] t: input triangle of type Face
 * @return The area of \p t.
 */
template<FaceConcept Triangle>
auto triangleArea(const Triangle& t)
{
	using CoordType = typename Triangle::VertexType::CoordType;
	return vcl::Triangle<CoordType>::area(
		t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

/**
 * @brief Calculates the quality measure of a a Face that is a triangle. Does not modify the
 * triangle.
 *
 * The quality measure is computed as 2 times the triangle's area divided by the square of the
 * length of its longest edge. The resulting value is in the range [0.0, 0.866], where 0.0
 * represents a degenerate triangle and 0.866 represents an equilateral triangle.
 *
 *
 * @tparam Triangle The type of the Face, that satisfies the FaceConcept.
 *
 * @param[in] t: The input triangle face.
 * @return The quality measure of the triangle.
 *
 * @note If the area of the triangle is zero (i.e., the vertices are collinear), the function
 * returns 0.0.
 */
template<FaceConcept Triangle>
double triangleQuality(const Triangle& t)
{
	using CoordType = typename Triangle::VertexType::CoordType;
	return vcl::Triangle<CoordType>::quality(
		t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

/**
 * @brief Compute a shape quality measure of a Face that is a triangle. Does not modify the
 * triangle.
 *
 * This function computes the quality measure of a triangle based on the ratio between its inradius
 * and circumradius. The function takes a FaceConcept Face.
 *
 * @tparam Triangle The type of the Face, that must satisfy the FaceConcept concept.
 *
 * @param[in] t: The input triangle face.
 * @return A value between 0 and 1 that represents the quality of the triangle shape. A value of 1
 * means the triangle is perfectly equilateral, while lower values indicate more distorted shapes
 * (i.e., halfsquare: 0.81). A value of 0 is returned when the triangle is degenerate (i.e., it has
 * zero area).
 */
template<FaceConcept Triangle>
double triangleQualityRadii(const Triangle& t)
{
	using CoordType = typename Triangle::VertexType::CoordType;
	return vcl::Triangle<CoordType>::qualityRadii(
		t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

/**
 * @brief Compute the mean ratio of a Face that is a triangle. Does not modify the triangle.
 *
 * Given the triangle \p t, the function computes the mean ratio shape quality measure of the
 * triangle. The mean ratio is defined as 2 * sqrt(a * b) / (a + b), where a and b are the
 * eigenvalues of the M^tM transformation matrix into a regular simplex. The mean ratio is a scalar
 * value in the range [0, 1] that describes the triangle quality, where 0 means the triangle is
 * degenerate, and 1 means it is perfectly equilateral.
 *
 * @tparam Triangle: The type of the Face, that must satisfy the FaceConcept concept.
 *
 * @param[in] t: The input triangle face.
 * @return The mean ratio shape quality measure of the triangle.
 *
 * @note The function assumes that the three points are not collinear and form a valid triangle.
 * If the three points are collinear or form a degenerate triangle, the function returns 0.
 */
template<FaceConcept Triangle>
double triangleQualityMeanRatio(const Triangle& t)
{
	using CoordType = typename Triangle::VertexType::CoordType;
	return vcl::Triangle<CoordType>::qualityMeanRatio(
		t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

} // namespace vcl
