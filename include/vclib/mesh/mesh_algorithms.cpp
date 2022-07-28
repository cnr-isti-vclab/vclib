/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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
#include "../../../external/earcut.hpp-2.2.3/include/mapbox/earcut.hpp"
#endif

namespace vcl::mesh {

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
	Point3<Scalar> n = polygonNormal(polygon);
	Point3<Scalar> u, v;
	getOrthoBase(n, u, v);

	std::vector<Point2<Scalar>> poly2D(polygon.size());
	for (uint i = 0; i < polygon.size(); ++i){
		// project i-th polygon in a 2D plane
		poly2D[i] = Point2<Scalar>(polygon[i]*u, polygon[i]*v);
	}
	return mesh::earCut(poly2D);
}

template<typename Polygon>
std::vector<uint> earCut(const Polygon& polygon)
{
	using VertexType = typename Polygon::VertexType;
	using CoordType = typename VertexType::CoordType;

	std::vector<CoordType> pol; pol.reserve(polygon.vertexNumber());
	for (const VertexType* v : polygon.vertices()){
		pol.push_back(v->coord());
	}
	return mesh::earCut(pol);
}

} // namespace vcl::mesh
