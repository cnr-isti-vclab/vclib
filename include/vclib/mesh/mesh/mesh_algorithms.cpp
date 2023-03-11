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

#include <vclib/space/polygon.h>

namespace vcl::mesh {

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
	return Polygon<CoordType>::earCut(pol.begin(), pol.end());
}

} // namespace vcl::mesh
