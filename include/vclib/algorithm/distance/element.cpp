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

#include "element.h"

#include "misc.h"

#include <vclib/algorithm/stat/bounding_box.h>
#include <vclib/algorithm/polygon/geometry.h>
#include <vclib/math/min_max.h>

namespace vcl {

/**
 * @brief Calculate the distance between a 3D point and a 3D triangle face.
 *
 * @tparam PointType: The type of point. Must satisfy the Point3Concept.
 * @tparam FaceType: The type of face. Must satisfy the FaceConcept.
 *
 * @param[in] p: The point to calculate the distance from.
 * @param[in] f: The face to calculate the distance to.
 * @param[out] closest: The closest point on the triangle to the given point.
 * @param[in] signedDist: Whether to calculate the signed distance. Default is false.
 * @return The distance between the point and the face.
 */
template<Point3Concept PointType, FaceConcept FaceType, typename ScalarType>
auto pointFaceDistance(
	const PointType& p,
	const FaceType&  f,
	ScalarType       maxDist,
	PointType&       closest,
	bool             signedDist)
{
	using FPointType = typename FaceType::VertexType::CoordType;

	ScalarType dist;

	// Extract the coordinates of the vertices of the face.
	const FPointType& fp0 = f.vertex(0)->coord();
	const FPointType& fp1 = f.vertex(1)->coord();
	const FPointType& fp2 = f.vertex(2)->coord();

	// If the face is degenerate (i.e. its normal vector has zero length), consider it as a segment.
	if (f.normal().norm() == 0) {
		// Calculate the bounding box of the face.
		Box3<ScalarType> box = vcl::boundingBox(f);
		Segment3<ScalarType> s(box.min(), box.max());
		// If the diagonal of the bounding box is greater than zero, calculate the distance between
		// the query point and the segment.
		if (box.diagonal() > 0) {
			dist = vcl::pointSegmentDistance(p, s, closest);
		}
		else {
			// If the diagonal of the bounding box is zero, set the closest point to be the minimum
			// corner of the bounding box and calculate the distance between the query point and
			// this point.
			closest = box.min();
			dist = p.dist(closest);
		}
	}
	else {
		// Calculate the distance between the query point and the plane of the triangle.
		Plane<ScalarType> fPlane(fp0, f.normal());
		dist = pointPlaneDistance(p, fPlane, true);

		if (dist >= maxDist)
			return dist;

		// Project the query point onto the triangle plane to obtain the closest point on the
		// triangle.
		closest = p - fPlane.direction() * dist;

		// Calculate the three edges of the triangle.
		Point3<ScalarType> fEdge[3];
		fEdge[0] = fp1 - fp0;
		fEdge[1] = fp2 - fp1;
		fEdge[2] = fp0 - fp2;

		// Determine the best axis to use for projection by finding the axis with the largest
		// component of the normal vector.
		int bestAxis;
		if (std::abs(f.normal()[0]) > std::abs(f.normal()[1])) {
			if (std::abs(f.normal()[0]) > std::abs(f.normal()[2]))
				bestAxis = 0;
			else
				bestAxis = 2;
		}
		else {
			if (std::abs(f.normal()[1]) > std::abs(f.normal()[2]))
				bestAxis = 1; /* 1 > 0 ? 2 */
			else
				bestAxis = 2; /* 2 > 1 ? 2 */
		}

		// Scale the edges by the inverse of the projection direction on the best axis.
		ScalarType scaleFactor = 1 / fPlane.direction()[bestAxis];
		fEdge[0] *= scaleFactor;
		fEdge[1] *= scaleFactor;
		fEdge[2] *= scaleFactor;

		// Compute barycentric coordinates of closest point
		ScalarType b0, b1, b2;
		int ba = (bestAxis + 2) % 3;
		int bb = (bestAxis + 1) % 3;

		Segment3<ScalarType> s0(fp1, fp2), s1(fp2, fp0), s2(fp0, fp1);
		b0 = fEdge[1][bb] * (closest[ba] - fp1[ba]) - fEdge[1][ba] * (closest[bb] - fp1[bb]);
		if (b0 <= 0) {
			return pointSegmentDistance(p, s0, closest);
		}
		b1 = fEdge[2][bb] * (closest[ba] - fp2[ba]) - fEdge[2][ba] * (closest[bb] - fp2[bb]);
		if (b1 <= 0) {
			return pointSegmentDistance(p, s1, closest);
		}
		b2 = fEdge[0][bb] * (closest[ba] - fp0[ba]) - fEdge[0][ba] * (closest[bb] - fp0[bb]);
		if (b2 <= 0) {
			return pointSegmentDistance(p, s2, closest);
		}

		const ScalarType EPS = ScalarType(0.000001);
		ScalarType b = vcl::min(b0, b1, b2);
		if (b < EPS * faceArea(f)) {
			ScalarType bt;
			if (b == b0)
				bt = pointSegmentDistance(p, s0, closest);
			else if (b == b1)
				bt = pointSegmentDistance(p, s1, closest);
			else {
				assert(b == b2);
				bt = pointSegmentDistance(p, s2, closest);
			}
			dist = bt;
		}

		if (!signedDist)
			dist = std::abs(dist);
	}

	return dist;
}

template<Point3Concept PointType, FaceConcept FaceType, typename ScalarType>
auto pointFaceDistance(const PointType& p, const FaceType& f, ScalarType maxDist, bool signedDist)
{
	PointType closest;
	return pointFaceDistance(p, f, maxDist, closest, signedDist);
}

template<Point3Concept PointType, FaceConcept FaceType>
auto pointFaceDistance(const PointType& p, const FaceType& f, PointType& closest, bool signedDist)
{
	using ScalarType = typename PointType::ScalarType;

	ScalarType maxDist = std::numeric_limits<ScalarType>::max();
	return pointFaceDistance(p, f, maxDist, closest, signedDist);
}

/**
 * @brief Calculate the distance between a 3D point and a 3D triangle face.
 *
 * @tparam PointType: The type of point. Must satisfy the Point3Concept.
 * @tparam FaceType: The type of face. Must satisfy the FaceConcept.
 *
 * @param[in] p: The point to calculate the distance from.
 * @param[in] f: The face to calculate the distance to.
 * @param[in] signedDist: Whether to calculate the signed distance. Default is false.
 * @return The distance between the point and the face.
 */
template<Point3Concept PointType, FaceConcept FaceType>
auto pointFaceDistance(const PointType& p, const FaceType& f, bool signedDist)
{
	using ScalarType = typename PointType::ScalarType;

	PointType closest;

	ScalarType maxDist = std::numeric_limits<ScalarType>::max();
	return pointFaceDistance(p, f, maxDist, closest, signedDist);
}

} // namespace vcl
