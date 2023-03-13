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

#include "triangle.h"

#include <vclib/space/polygon.h>

namespace vcl {

/**
 * @brief Computes the normal of a Face that is a generic polygon. Does not modify the
 * polygon. This function works also with simple triangles, but it is less efficient thant the
 * function "triangleNormal".
 *
 * @tparam Polygon: The type of the polygon, that must satisfy the FaceConcept concept.
 *
 * @param[in] p: input polygonal Face
 * @return The normal of \p p.
 */
template<FaceConcept Polygon>
typename Polygon::VertexType::CoordType polygonNormal(const Polygon& p)
{
	using NormalType = typename Polygon::VertexType::CoordType;
	// compute the sum of normals for each triplet of consecutive points
	NormalType sum;
	sum.setZero();
	for (uint i = 0; i < p.vertexNumber(); ++i) {
		sum += Triangle<NormalType>::normal(
			p.vertex(i)->coord(), p.vertexMod(i + 1)->coord(), p.vertexMod(i + 2)->coord());
	}
	sum.normalize();
	return sum;
}

/**
 * @brief Computes the barycenter of a Face that is a generic polygon. This function works also with
 * simple triangles, but it is less efficient thant the function "triangleBarycenter".
 *
 * @tparam Polygon: The type of the polygon, that must satisfy the FaceConcept concept.
 *
 * @param[in] p: input polygonal Face
 * @return The barycenter of \p p.
 */
template<FaceConcept Polygon>
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
 * @brief Computes the area of a Face that is a generic polygon. This function works also with
 * simple triangles, but it is less efficient thant the function "triangleArea".
 *
 * @tparam Polygon: The type of the polygon, that must satisfy the FaceConcept concept.
 *
 * @param[in] p: input polygonal Face
 * @return The area of p.
 *
 * @note The polygon must be simple, i.e., it cannot intersect itself.
 */
template<FaceConcept Polygon>
auto polygonArea(const Polygon& p)
{
	using PointType = typename Polygon::VertexType::CoordType;
	using Scalar = typename PointType::ScalarType;

	if (p.vertexNumber() == 3) {
		return triangleArea(p);
	}

	PointType bar = polygonBarycenter(p);
	Scalar area = 0;
	for (uint i = 0; i < p.vertexNumber(); ++i){
		const PointType& p0 = p.vertex(i)->coord();
		const PointType& p1 = p.vertexMod(i+1)->coord();
		area += Triangle<PointType>::area(p0, p1, bar);
	}
	return area;
}

/**
 * @brief Calculates the perimeter of a polygon.
 *
 * The polygonPerimeter function calculates the perimeter of a polygon.
 * The function takes a polygon as input, which must satisfy the FaceConcept. The FaceConcept is a
 * concept that requires the polygon to have a VertexType and a vertexNumber() function that returns
 * the number of vertices in the polygon.
 *
 * @tparam Polygon: The type of the polygon, that must satisfy the FaceConcept concept.
 *
 * @param[in] p: The polygon for which to calculate the perimeter.
 * @returns The perimeter of the polygon.
 */

template<FaceConcept Polygon>
auto polygonPerimeter(const Polygon& p)
{
	using PointType = typename Polygon::VertexType::CoordType;
	using Scalar = typename PointType::ScalarType;

	Scalar per = 0;
	for (uint i = 0; i < p.vertexNumber(); ++i){
		const PointType& p0 = p.vertex(i)->coord();
		const PointType& p1 = p.vertexMod(i+1)->coord();
		per += p0.dist(p1);
	}
	return per;
}

} // namespace vcl
