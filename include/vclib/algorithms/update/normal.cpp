/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#include "normal.h"

#include <vclib/mesh/requirements.h>

namespace vcl {

/**
 * @brief Computes the normal of the triangle composed by the points p0, p1, and p2, considering
 * that these three points are ordered in counterclockwise order.
 *
 * @param p0: first point of the triangle.
 * @param p1: second point of the triangle.
 * @param p2: third point of the triangle.
 * @return the normal of the triangle composed by p0, p1 and p2.
 */
template<typename PointType>
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
 * @param t: input triangle of type Face
 * @return the normal of t
 */
template<typename Triangle, typename NormalType>
NormalType triangleNormal(const Triangle& t)
{
	return triangleNormal(t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

/**
 * @brief Computes the normal of a Face that is a generic polygon. Does not modify the
 * polygon. This function works also with simple triangles, but it is less efficient thant the
 * function "triangleNormal".
 *
 * @param p: input polygonal Face
 * @return the normal of p
 */
template<typename Polygon, typename NormalType>
NormalType polygonNormal(const Polygon& p)
{
	// compute the sum of normals for each triplet of consecutive points
	NormalType sum; sum.setZero();
	for (unsigned int i = 0; i < p.vertexNumber(); ++i){
		sum += triangleNormal(
			p.vertexMod(i)->coord(), p.vertexMod(i+1)->coord(), p.vertexMod(i+2)->coord());
	}
	sum.normalize();
	return sum;
}

template<typename MeshType>
void normalizePerFaceNormals(MeshType& m)
{
	vcl::requireFaces<MeshType>();
	vcl::requirePerFaceNormal(m);

	using FaceType = typename MeshType::Face;

	for (FaceType& f : m.faces()) {
		f.normal().normalize();
	}
}

template<typename MeshType>
void updatePerFaceNormals(MeshType& m, bool normalize)
{
	vcl::requireVertices<MeshType>();
	vcl::requireFaces<MeshType>();
	vcl::requirePerFaceNormal(m);

	using FaceType = typename MeshType::Face;
	if constexpr (vcl::hasTriangles<MeshType>()) {
		for (FaceType& f : m.faces()) {
			f.normal() = triangleNormal(f);
		}
	}
	else {
		for (FaceType& f : m.faces()) {
			f.normal() = polygonNormal(f);
		}
	}
	if (normalize)
		normalizePerFaceNormals(m);
}

} // namespace vcl
