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

namespace vcl {

template<PointConcept PointT>
Polygon<PointT>::Polygon()
{

}

template<PointConcept PointT>
PointT& Polygon<PointT>::point(uint i)
{
    return p[i];
}

template<PointConcept PointT>
const PointT& Polygon<PointT>::point(uint i) const
{
	return p[i];
}

template<PointConcept PointT>
typename Polygon<PointT>::ScalarType Polygon<PointT>::sideLength(uint i) const
{
	return p[i].dist(p[(i+1)%3]);
}

/**
 * @brief Computes the normal of a container of 3D points iterated between the iterators begin and
 * end, listed in counterclockwise order, representing a polygon.
 *
 * @tparam Iterator: an iterator which points to a type that satiesfies the PointConcept.
 * @param[in] begin: iterator pointing to the first point of the polygon.
 * @param[in] end: end iterator
 * @return The normal of the polygon.
 */
template<PointConcept PointT>
template<typename Iterator>
PointT Polygon<PointT>::normal(Iterator begin, Iterator end)
	requires (std::is_same_v<typename Iterator::value_type, PointT> && PointT::DIM == 3)
{
	// compute the sum of normals for each triplet of consecutive points
	PointT sum;
	sum.setZero();
	for (auto i = begin; i != end; ++i) {
		auto i1 = i; ++i1;
		if (i1 == end) i1 = begin;

		auto i2 = i1; ++i2;
		if (i2 == end) i2 = begin;

		sum += (*i1 - *i).cross(*i2 - *i);
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
template<PointConcept PointT>
template<typename Iterator>
PointT Polygon<PointT>::barycenter(Iterator begin, Iterator end)
	requires (std::is_same_v<typename Iterator::value_type, PointT>)
{
	PointT bar;
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
template<PointConcept PointT>
template<typename Iterator, typename WIterator>
PointT Polygon<PointT>::weightedBarycenter(Iterator begin, Iterator end, WIterator wbegin)
	requires (std::is_same_v<typename Iterator::value_type, PointT>)
{
	using ScalarType = typename WIterator::value_type;

	PointT bar;
	bar.setZero();
	ScalarType wsum = 0;

	for (; begin != end; ++begin, ++wbegin){
		bar += *begin * *wbegin;
		wsum += *wbegin;
	}

	return bar / wsum;
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
template<PointConcept PointT>
template<typename Iterator>
PointT Polygon<PointT>::perimeter(Iterator begin, Iterator end)
	requires (std::is_same_v<typename Iterator::value_type, PointT>)
{
	using Scalar = typename PointType::ScalarType;

	Scalar per = 0;
	for (auto i = begin; i != end; ++i){
		const PointT& p0 = *i;
		auto i1 = i; ++i;
		if (i1 == end) i1 = begin;
		const PointT& p1 = *i1;
		per += p0.dist(p1);
	}
	return per;
}

/**
 * @brief Calculates the area of a polygon. This function works also with simple triangles, but it
 * is less efficient thant the function "Triangle<PointT>::area()".
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
template<PointConcept PointT>
template<typename Iterator>
PointT Polygon<PointT>::area(Iterator begin, Iterator end)
	requires (std::is_same_v<typename Iterator::value_type, PointT>)
{
	using Scalar = typename PointType::ScalarType;

	PointType bar = barycenter(begin, end);
	Scalar area = 0;
	for (auto i = begin; i != end; ++i){
		const PointT& p0 = *i;
		auto i1 = i; ++i;
		if (i1 == end) i1 = begin;
		const PointT& p1 = *i1;

		area += Triangle<PointT>::area(p0, p1, bar);
	}
	return area;
}

} // namespace vcl
