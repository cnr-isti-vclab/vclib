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
template<FaceConcept Triangle, typename ScalarType>
ScalarType triangleArea(const Triangle& t)
{
	return triangleArea(t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

} // namespace vcl
