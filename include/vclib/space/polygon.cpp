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

#if __has_include(<mapbox/earcut.hpp>)
#include <mapbox/earcut.hpp>
#else
// inclusion for usage of vclib without CMake - not ideal but necessary for header only
#include "../../../../external/earcut.hpp-2.2.3/include/mapbox/earcut.hpp"
#endif

// structs to make working the mapbox earcut algorithm on vcl::Point2
namespace mapbox {
namespace util {

template<typename Scalar>
struct nth<0, vcl::Point2<Scalar>>
{
	inline static auto get(const vcl::Point2<Scalar>& t) { return t.x(); };
};
template<typename Scalar>
struct nth<1, vcl::Point2<Scalar>>
{
	inline static auto get(const vcl::Point2<Scalar>& t) { return t.y(); };
};

} // namespace util
} // namespace mapbox

namespace vcl {

template<PointConcept PointT>
Polygon<PointT>::Polygon()
{

}

template<PointConcept PointT>
template<typename Iterator>
Polygon<PointT>::Polygon(Iterator begin, Iterator end)
	requires (std::is_same_v<typename Iterator::value_type, PointT>) :
		p(begin, end)
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

template<PointConcept PointT>
PointT Polygon<PointT>::normal() const requires (PointT::DIM == 3)
{
	return normal(p.begin(), p.end());
}

template<PointConcept PointT>
PointT Polygon<PointT>::barycenter() const
{
	return barycenter(p.begin(), p.end());
}

template<PointConcept PointT>
template<typename WIterator>
PointT Polygon<PointT>::weightedBarycenter(WIterator wbegin) const
{
	return weightedBarycenter(p.begin(), p.end(), wbegin);
}

template<PointConcept PointT>
typename Polygon<PointT>::ScalarType Polygon<PointT>::perimenter() const
{
	return perimeter(p.begin(), p.end());
}

template<PointConcept PointT>
typename Polygon<PointT>::ScalarType Polygon<PointT>::area() const
{
	return area(p.begin(), p.end());
}

template<PointConcept PointT>
std::vector<uint> Polygon<PointT>::earCut() const
	requires (PointT::DIM == 2 || PointT::DIM == 3)
{
	return earCut(p.begin(), p.end());
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
typename PointT::ScalarType Polygon<PointT>::perimeter(Iterator begin, Iterator end)
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
typename PointT::ScalarType  Polygon<PointT>::area(Iterator begin, Iterator end)
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

/**
 * @brief Triangulates a simple polygon with no holes using the ear-cutting algorithm.
 *
 * @tparam Iterator: The type of iterator used to represent the vertices of the polygon. It must
 * satisfy the Point2Concept requirement.
 * @param[in] begin: An iterator pointing to the first vertex of the polygon.
 * @param[in] end: An iterator pointing to one past the last vertex of the polygon.
 * @return A vector containing the indices of the vertices that form triangles in the triangulated
 * polygon. Each group of three indices represents the vertices of a single triangle, and the
 * indices are ordered in a counter-clockwise direction.
 * @throws std::logic_error If the polygon is not simple or has holes.
 *
 * @remarks This function uses the ear-cutting algorithm to triangulate a simple polygon with no
 * holes. The polygon is represented as a sequence of vertices, where each vertex is a
 * two-dimensional point. The function returns a vector containing the indices of the vertices that
 * form triangles in the triangulated polygon. The indices are ordered in a counter-clockwise
 * direction, and each group of three indices represents the vertices of a single triangle. The
 * function requires that the type of iterator used to represent the vertices of the polygon
 * satisfies the Point2Concept requirement, which means that it must have a value_type that is a
 * Point2 object with a ScalarType member representing the scalar type used to represent the
 * coordinates of the point. If the polygon is not simple or has holes, the function throws a
 * std::logic_error.
 */
template<PointConcept PointT>
template<typename Iterator>
std::vector<uint> Polygon<PointT>::earCut(Iterator begin, Iterator end)
	requires (std::is_same_v<typename Iterator::value_type, PointT> && PointT::DIM == 2)
{
	using Scalar = typename PointT::ScalarType;

	// Convert the polygon represented as a sequence of vertices to a vector of vectors of points.
	// This is necessary because the earcut library expects the polygon to be represented as a
	// vector of contours, where each contour is a vector of points. In this case, there is only one
	// contour, which is represented as a vector of points.
	std::vector<std::vector<Point2<Scalar>>> poly;
	poly.push_back(std::vector<Point2<Scalar>>(begin, end));

	// Use the earcut library to triangulate the polygon and return the result.
	return mapbox::earcut<uint>(poly);
}

/**
 * @brief Triangulates a simple polygon with no holes in 3D space by projecting it onto a 2D plane
 * and applying the ear-cutting algorithm.
 *
 * @tparam Iterator: The type of iterator used to represent the vertices of the polygon. It must
 * satisfy the Point3Concept requirement.
 * @param[in] begin: An iterator pointing to the first vertex of the polygon.
 * @param[in] end: An iterator pointing to one past the last vertex of the polygon.
 * @return A vector containing the indices of the vertices that form triangles in the triangulated
 * polygon. Each group of three indices represents the vertices of a single triangle, and the
 * indices are ordered in a counter-clockwise direction.
 * @throws std::logic_error If the polygon is not simple or has holes.
 *
 * @remarks This function triangulates a simple polygon with no holes in 3D space by projecting it
 * onto a 2D plane and applying the ear-cutting algorithm. The polygon is represented as a sequence
 * of vertices, where each vertex is a three-dimensional point. The function first calculates the
 * normal vector of the polygon and an orthonormal basis for the plane containing the polygon. It
 * then projects each vertex onto the plane and triangulates the resulting 2D polygon using the
 * ear-cutting algorithm. The function returns a vector containing the indices of the vertices that
 * form triangles in the triangulated polygon. The indices are ordered in a counter-clockwise
 * direction, and each group of three indices represents the vertices of a single triangle. The
 * function requires that the type of iterator used to represent the vertices of the polygon
 * satisfies the Point3Concept requirement, which means that it must have a value_type that is a
 * Point3 object with a ScalarType member representing the scalar type used to represent the
 * coordinates of the point. If the polygon is not simple or has holes, the function throws a
 * std::logic_error.
 */
template<PointConcept PointT>
template<typename Iterator>
std::vector<uint> Polygon<PointT>::earCut(Iterator begin, Iterator end)
	requires (std::is_same_v<typename Iterator::value_type, PointT> && PointT::DIM == 3)
{
	using Scalar = typename PointType::ScalarType;

	// Calculate the normal vector of the polygon and an orthonormal basis for the plane containing
	// the polygon.
	PointType n = normal(begin, end);
	PointType u, v;
	orthoBase(n, u, v);

	// Project each vertex onto the plane defined by the orthonormal basis.
	std::vector<Point2<Scalar>> poly2D;
	poly2D.reserve(std::distance(begin, end));
	for (auto i = begin; i != end; ++i){
		// project i-th polygon in a 2D plane
		poly2D.push_back(Point2<Scalar>(*i*u, *i*v));
	}

	// Use the ear-cut algorithm to triangulate the polygon in the 2D plane and return the result.
	return Polygon<Point2<Scalar>>::earCut(poly2D.begin(), poly2D.end());
}

} // namespace vcl
