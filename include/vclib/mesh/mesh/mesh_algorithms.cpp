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

template<typename Scalar>
std::vector<uint> earCut(const std::vector<Point2<Scalar>>& polygon)
{
	std::vector<std::vector<Point2<Scalar>>> poly;
	poly.push_back(polygon);

	return mapbox::earcut<uint>(poly);
}

template<typename Scalar>
std::vector<uint> earCut(const std::vector<Point3<Scalar>>& polygon)
{
	Point3<Scalar> n = mesh::polygonNormal(polygon.begin(), polygon.end());
	Point3<Scalar> u, v;
	orthoBase(n, u, v);

	std::vector<Point2<Scalar>> poly2D(polygon.size());
	for (uint i = 0; i < polygon.size(); ++i){
		// project i-th polygon in a 2D plane
		poly2D[i] = Point2<Scalar>(polygon[i]*u, polygon[i]*v);
	}
	return mesh::earCut(poly2D);
}

template<typename Face>
std::vector<uint> earCut(const Face& polygon)
{
	using VertexType = typename Face::VertexType;
	using CoordType = typename VertexType::CoordType;

	std::vector<CoordType> pol; pol.reserve(polygon.vertexNumber());
	for (const VertexType* v : polygon.vertices()){
		pol.push_back(v->coord());
	}
	return mesh::earCut(pol);
}

} // namespace vcl::mesh
