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

#include "mesh_algorithms.h"

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

namespace vcl::mesh {

/**
 * @brief Computes the normal of a container of 3D points iterated between the iterators begin and
 * end, listed in counterclockwise order, representing a polygon.
 *
 * @tparam Iterator: an iterator which points to a type that satiesfies the PointConcept.
 * @param[in] begin: iterator pointing to the first point of the polygon.
 * @param[in] end: end iterator
 * @return The normal of the polygon.
 */
template<typename Iterator>
auto polygonNormal(Iterator begin, Iterator end)
	requires PointConcept<typename Iterator::value_type>
{
	using PointType = typename Iterator::value_type;

	// compute the sum of normals for each triplet of consecutive points
	PointType sum;
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
template<typename Iterator>
std::vector<uint> earCut(Iterator begin, Iterator end)
	requires Point2Concept<typename Iterator::value_type>
{
	using PointType = typename Iterator::value_type;
	using Scalar = typename PointType::ScalarType;

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
template<typename Iterator>
std::vector<uint> earCut(Iterator begin, Iterator end)
	requires Point3Concept<typename Iterator::value_type>
{
	using PointType = typename Iterator::value_type;
	using Scalar = typename PointType::ScalarType;

	// Calculate the normal vector of the polygon and an orthonormal basis for the plane containing
	// the polygon.
	PointType n = mesh::polygonNormal(begin, end);
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
	return mesh::earCut(poly2D.begin(), poly2D.end());
}
/**
 * @brief Triangulates a simple polygon with no holes by applying the ear-cutting algorithm.
 *
 * @tparam Face: The type of face used to represent the polygon. It must have a VertexType member
 * type that represents the type of vertex used to represent the corners of the polygon. The
 * VertexType must have a CoordType member type that represents the scalar type used to represent
 * the coordinates of the vertices.
 * @param[in] polygon: A reference to the face representing the polygon to be triangulated.
 * @return A vector containing the indices of the vertices that form triangles in the triangulated
 * polygon. Each group of three indices represents the vertices of a single triangle, and the
 * indices are ordered in a counter-clockwise direction.
 * @throws std::logic_error If the polygon is not simple or has holes.
 *
 * @remarks This function triangulates a simple polygon with no holes by applying the ear-cutting
 * algorithm. The polygon is represented as a face, which consists of a sequence of vertices
 * connected by edges. The function extracts the coordinates of the vertices from the face and
 * passes them to the ear-cutting algorithm. The function returns a vector containing the indices of
 * the vertices that form triangles in the triangulated polygon. The indices are ordered in a
 * counter-clockwise direction, and each group of three indices represents the vertices of a single
 * triangle. The function requires that the type of face representing the polygon has a VertexType
 * member representing the type of vertices used to represent the polygon, and each vertex type must
 * have a CoordType member representing the scalar type used to represent the coordinates of the
 * vertex. If the polygon is not simple or has holes, the function throws a std::logic_error.
 */
template<typename Face>
std::vector<uint> earCut(const Face& polygon)
{
	using VertexType = typename Face::VertexType;
	using CoordType = typename VertexType::CoordType;

	// Extract the coordinates of the vertices from the face and pass them to the ear-cut algorithm.
	std::vector<CoordType> pol; pol.reserve(polygon.vertexNumber());
	for (const VertexType* v : polygon.vertices()){
		pol.push_back(v->coord());
	}
	return mesh::earCut(pol.begin(), pol.end());
}

} // namespace vcl::mesh
