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

#include <vclib/mesh/mesh/mesh_algorithms.h>

namespace vcl {

/**
 * @brief Computes the normal of a container of 3D points iterated between the iterators begin and
 * end, listed in counterclockwise order, representing a polygon.
 *
 * @tparam Iterator: an iterator which points to a type that satiesfies the PointConcept.
 *
 * @param[in] begin: iterator pointing to the first point of the polygon.
 * @param[in] end: end iterator
 * @return The normal of the polygon.
 */
template<typename Iterator>
auto polygonNormal(Iterator begin, Iterator end)
	requires PointConcept<typename Iterator::value_type>
{
	return mesh::polygonNormal(begin, end);
}

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
		sum += triangleNormal(
			p.vertex(i)->coord(), p.vertexMod(i + 1)->coord(), p.vertexMod(i + 2)->coord());
	}
	sum.normalize();
	return sum;
}

/**
 * @brief Computes the barycenter of a container of points iterated between the iterators begin
 * and end, listed in counterclockwise order, representing a polygon.
 *
 * @tparam Iterator: an iterator which points to a type that satiesfies the PointConcept.
 *
 * @param[in] begin: iterator pointing to the first point of the polygon.
 * @param[in] end: end iterator
 * @return The barycenter of the polygon.
 */
template<typename Iterator>
auto polygonBarycenter(Iterator begin, Iterator end)
	requires PointConcept<typename Iterator::value_type>
{
	using PointType = typename Iterator::value_type;

	PointType bar;
	bar.setZero();

	uint cnt = 0;
	for (; begin != end; ++begin) {
		bar += *begin;
		++cnt;
	}

	assert(cnt);

	return bar / cnt;
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
 * @brief Computes the weighted barycenter of a container of 3D points iterated between the
 * iterators begin and end, listed in counterclockwise order, representing a polygon. Weights are
 * iterated by another iterator wbegin, which iterates to a container of the same size of the
 * polygon container.
 *
 * @tparam Iterator: an iterator which points to a type that satiesfies the PointConcept.
 * @tparam WIterator: an iterator which points to a scalar type.
 *
 * @param[in] begin: iterator pointing to the first point of the polygon.
 * @param[in] end: end iterator of the polygon container.
 * @param[in] wbegin: iterator pointing to the first weight associated to the fist polygon point.
 * @return The weighted barycenter of the polygon.
 */
template<typename Iterator, typename WIterator>
auto polygonWeighedBarycenter(Iterator begin, Iterator end, WIterator wbegin)
	requires PointConcept<typename Iterator::value_type>
{
	using PointType = typename Iterator::value_type;
	using ScalarType = typename WIterator::value_type;

	PointType bar;
	bar.setZero();
	ScalarType wsum = 0;

	for (; begin != end; ++begin, ++wbegin){
		bar += *begin * *wbegin;
		wsum += *wbegin;
	}

	return bar / wsum;
}

/**
 * @brief Calculates the area of a polygon. This function works also with simple triangles, but it
 * is less efficient thant the function "triangleArea".
 *
 * @tparam Iterator: an iterator which points to a type that satiesfies the PointConcept.
 *
 * @param[in] begin: an iterator pointing to the first point in the polygon
 * @param[in] end: an iterator pointing past the last point in the polygon
 * @return the area of the polygon
 *
 * @note The points in the polygon must have a scalar type, and must satisfy the PointConcept
 * concept.
 * @note The polygon must be simple, i.e., it cannot intersect itself.
 */
template<typename Iterator>
auto polygonArea(Iterator begin, Iterator end) requires PointConcept<typename Iterator::value_type>
{
	using PointType = typename Iterator::value_type;
	using Scalar = typename PointType::ScalarType;

	PointType bar = polygonBarycenter(begin, end);
	Scalar area = 0;
	for (auto i = begin; i != end; ++i){
		const PointType& p0 = *i;
		auto i1 = i; ++i;
		if (i1 == end) i1 = begin;
		const PointType& p1 = *i1;

		area += triangleArea(p0, p1, bar);
	}
	return area;
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
		area += triangleArea(p0, p1, bar);
	}
	return area;
}

/**
 * @brief Calculates the perimeter of a polygon defined by a range of points.
 *
 * The polygonPerimeter function calculates the perimeter of a polygon defined by a range of points.
 * The function takes two iterators as input, representing the beginning and end of the range of
 * points. The points in the range must satisfy the PointConcept, which is a concept that requires
 * the point type to have a ScalarType and a dist() function that calculates the distance between
 * two points.
 *
 * @tparam Iterator: an iterator which points to a type that satiesfies the PointConcept.
 *
 * @param[in] begin: An iterator pointing to the first point in the range.
 * @param[in] end: An iterator pointing to one past the last point in the range.
 * @returns The perimeter of the polygon defined by the range of points.
 */
template<typename Iterator>
auto polygonPerimeter(Iterator begin, Iterator end)
	requires PointConcept<typename Iterator::value_type>
{
	using PointType = typename Iterator::value_type;
	using Scalar = typename PointType::ScalarType;

	Scalar per = 0;
	for (auto i = begin; i != end; ++i){
		const PointType& p0 = *i;
		auto i1 = i; ++i;
		if (i1 == end) i1 = begin;
		const PointType& p1 = *i1;
		per += p0.dist(p1);
	}
	return per;
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
