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

#include "polygon.h"

namespace vcl {

/**
 * @brief Computes the normal of a std::vector of 3D points listed in counterclockwise order,
 * representing a polygon.
 *
 * @param[in] p: input container of 3D points representing a polygon.
 * @return The normal of p.
 */
template<PointConcept PointType>
PointType polygonNormal(const std::vector<PointType>& p)
{
	// compute the sum of normals for each triplet of consecutive points
	PointType sum;
	sum.setZero();
	for (uint i = 0; i < p.size(); ++i) {
		sum += triangleNormal(
			p[i], p[(i+1)%p.size()], p[(i+2)%p.size()]);
	}
	sum.normalize();
	return sum;
}

/**
 * @brief Computes the normal of a Face that is a generic polygon. Does not modify the
 * polygon. This function works also with simple triangles, but it is less efficient thant the
 * function "triangleNormal".
 *
 * @param[in] p: input polygonal Face
 * @return The normal of p.
 */
template<typename Polygon>
typename Polygon::VertexType::CoordType polygonNormal(const Polygon& p)
{
	using NormalType = typename Polygon::VertexType::CoordType;
	// compute the sum of normals for each triplet of consecutive points
	NormalType sum;
	sum.setZero();
	for (uint i = 0; i < p.vertexNumber(); ++i) {
		sum += triangleNormal(
			p.vertexMod(i)->coord(), p.vertexMod(i + 1)->coord(), p.vertexMod(i + 2)->coord());
	}
	sum.normalize();
	return sum;
}

/**
 * @brief Computes the barycenter of a std::vector of points representing a polygon.
 *
 * @param[in] p: input container of points representing a polygon.
 * @return The barycenter of p.
 */
template<PointConcept PointType>
PointType polygonBarycenter(const std::vector<PointType>& p)
{
	PointType bar;
	bar.setZero();

	for (const PointType& pp : p)
		bar += pp;
	return bar / p.size();
}

/**
 * @brief Computes the barycenter of a Face that is a generic polygon. This function works also with
 * simple triangles, but it is less efficient thant the function "triangleBarycenter".
 *
 * @param[in] p: input polygonal Face
 * @return The barycenter of p.
 */
template<typename Polygon>
typename Polygon::VertexType::CoordType polygonBarycenter(const Polygon& p)
{
	using PointType = typename Polygon::VertexType::CoordType;

	PointType bar;
	bar.setZero();

	for (uint i = 0; i < p.vertexNumber(); ++i)
		bar += p.vertex(i)->coord();
	return bar / p.vertexNumber();
}

/**
 * @brief Computes the weighted barycenter of a std::vector of points representing a polygon.
 *
 * @param[in] p: input container of points representing a polygon.
 * @param[in] w: the weights for each point of the polygon.
 * @return The weighted barycenter of p.
 */
template<PointConcept PointType>
PointType polygonWeighedBarycenter(
	const std::vector<PointType>&                      p,
	const std::vector<typename PointType::ScalarType>& w)
{
	PointType bar;
	bar.setZero();
	typename PointType::ScalarType wsum = 0;

	for (uint i = 0; i < p.size(); ++i){
		bar += p[i] * w[i];
		wsum += w[i];
	}

	return bar / wsum;
}

/**
 * @brief Computes the area of a std::vector of 3D points listed in counterclockwise order,
 * representing a polygon.
 *
 * @param[in] p: input container of 3D points representing a polygon.
 * @return The area of p.
 */
template<PointConcept PointType>
typename PointType::ScalarType polygonArea(const std::vector<PointType>& p)
{
	using Scalar = typename PointType::ScalarType;
	PointType bar = polygonBarycenter(p);
	Scalar area = 0;
	for (uint i = 0; i < p.size(); ++i){
		const PointType& p0 = p[i];
		const PointType& p1 = p[(i+1)%p.size()];
		area += triangleArea(p0, p1, bar);
	}
	return area;
}

/**
 * @brief Computes the area of a Face that is a generic polygon. This function works also with
 * simple triangles, but it is less efficient thant the function "triangleArea".
 *
 * @param[in] p: input polygonal Face
 * @return The area of p.
 */
template<typename Polygon, typename ScalarType>
ScalarType polygonArea(const Polygon& p)
{
	using PointType = typename Polygon::VertexType::CoordType;

	if (p.vertexNumber() == 3) {
		return triangleArea(p);
	}

	PointType bar = polygonBarycenter(p);
	ScalarType area = 0;
	for (uint i = 0; i < p.vertexNumber(); ++i){
		const PointType& p0 = p.vertex(i)->coord();
		const PointType& p1 = p.vertexMod(i+1)->coord();
		area += triangleArea(p0, p1, bar);
	}
	return area;
}

} // namespace vcl
