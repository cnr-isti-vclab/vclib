/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

/**
 * This is a refactoring of VCGLib: https://github.com/cnr-isti-vclab/vcglib/
 * Copyright(C): Visual Computing Lab
 * ISTI - Italian National Research Council
 */

#include "normal.h"

#include <mgp/mesh/requirements.h>

namespace mgp {

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
 * @brief Computes the normal of an mgp::Face that is a triangle. Does not modify the triangle.
 * This function could accept also polygonal faces, but it will take into account only the first 3
 * vertices of the polygon. In this case, the resulting normal could be flipped w.r.yt. the actual
 * normal of the polygon.
 *
 * @param t: input triangle of type mgp::Face
 * @return the normal of t
 */
template<typename Triangle, typename NormalType>
NormalType triangleNormal(const Triangle& t)
{
	return triangleNormal(t.v(0)->coordinate(), t.v(1)->coordinate(), t.v(2)->coordinate());
}

/**
 * @brief Computes the normal of an mgp::Face that is a generic polygon. Does not modify the
 * polygon. This function works also with simple triangles, but it is less efficient thant the
 * function "triangleNormal".
 *
 * @param p: input polygonal mgp::Face
 * @return the normal of p
 */
template<typename Polygon, typename NormalType>
NormalType polygonNormal(const Polygon& p)
{
	// compute the sum of normals for each triplet of consecutive points
	NormalType sum; sum.setZero();
	for (unsigned int i = 0; i < p.vertexNumber(); ++i){
		sum += triangleNormal(
			p.vMod(i)->coordinate(), p.vMod(i+1)->coordinate(), p.vMod(i+2)->coordinate());
	}
	sum.normalize();
	return sum;
}

template<typename MeshType>
void normalizePerFaceNormals(MeshType& m)
{
	mgp::requireFaces<MeshType>();
	mgp::requirePerFaceNormal(m);

	using FaceType = typename MeshType::Face;

	for (FaceType& f : m.faceIterator()) {
		f.normal().normalize();
	}
}

template<typename MeshType>
void updatePerFaceNormals(MeshType& m, bool normalize)
{
	mgp::requireVertices<MeshType>();
	mgp::requireFaces<MeshType>();
	mgp::requirePerFaceNormal(m);

	using FaceType = typename MeshType::Face;
	if constexpr (mgp::hasTriangles<MeshType>()) {
		for (FaceType& f : m.faceIterator()) {
			f.normal() = triangleNormal(f);
		}
	}
	else {
		for (FaceType& f : m.faceIterator()) {
			f.normal() = polygonNormal(f);
		}
	}
	if (normalize)
		normalizePerFaceNormals(m);
}

} // namespace mgp
