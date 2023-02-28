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

#include "geometry.h"

namespace vcl {

/**
 * @brief Computes the normal of a face, whithout modifying the face. Works both for triangle and
 * polygonal faces, and it is optimized in case of triangle faces.
 *
 * @param f
 * @return
 */
template<FaceConcept FaceType>
typename FaceType::VertexType::CoordType faceNormal(const FaceType& f)
{
	if constexpr (TriangleFaceConcept<FaceType>) {
		return triangleNormal(f);
	}
	else {
		return polygonNormal(f);
	}
}

/**
 * @brief Computes the barycenter of a face. Works both for triangle and polygonal faces, and it is
 * optimized in case of triangle faces.
 *
 * @param f
 * @return
 */
template<FaceConcept FaceType>
typename FaceType::VertexType::CoordType faceBarycenter(const FaceType& f)
{
	if constexpr (TriangleFaceConcept<FaceType>) {
		return triangleBarycenter(f);
	}
	else {
		return polygonBarycenter(f);
	}
}

/**
 * @brief Computes the area of a face. Works both for triangle and polygonal faces, and it is
 * optimized in case of triangle faces.
 *
 * @param f
 * @return
 */
template<FaceConcept FaceType>
auto faceArea(const FaceType& f)
{
	if constexpr (TriangleFaceConcept<FaceType>) {
		return triangleArea<FaceType>(f);
	}
	else {
		return polygonArea<FaceType>(f);
	}
}

/**
 * @brief Computes the perimeter of a face. Works both for triangle and polygonal faces, and it is
 * optimized in case of triangle faces.
 *
 * @param f
 * @return
 */
template<FaceConcept FaceType>
auto facePerimeter(const FaceType& f)
{
	if constexpr (TriangleFaceConcept<FaceType>) {
		return trianglePerimeter<FaceType>(f);
	}
	else {
		return polygonPerimeter<FaceType>(f);
	}
}

/**
 * @brief Returns the internal angle (in radians) of the vi-th vertex of the face.
 *
 * @param[in] f: the face on which calculate the angle
 * @param[in] vi: the index of the vertex in the face on which calculate the angle
 * @return the angle in radians at the vi-th vertex.
 */
template<FaceConcept FaceType>
auto faceAngleOnVertexRad(const FaceType& f, uint vi)
{
	const auto& p0 = f.vertex(vi)->coord();
	const auto& p1 = f.vertexMod((int)vi+1)->coord();
	const auto& p2 = f.vertexMod((int)vi-1)->coord();
	return (p2 - p0).angle(p1 - p0);
}

/**
 * @brief Compute the signed dihedral angle between the normals of the given face and its adjacent
 * face on the edge e.
 *
 * The angle between the normal is signed according to the concavity/convexity of the
 * dihedral angle: negative if the edge shared between the two faces is concave, positive otherwise.
 * The surface it is assumend to be oriented.
 * It simply use the projection of  the opposite vertex onto the plane of the other one.
 * It does not assume anything on face normals.
 *
 *     v0 ___________ vf1
 *       |\          |
 *       | e\     f1 |
 *       |    \e1    |
 *       |f     \    |
 *       |        \  |
 *       |__________\|
 *    vf0             v1
 */
template<FaceConcept FaceType>
auto faceDihedralAngleOnEdge(const FaceType& f, uint e) requires comp::HasAdjacentFaces<FaceType>
{
	if (! comp::isAdjacentFacesEnabledOn(f)) {
		throw vcl::MissingComponentException("Face has no Adjacent Faces component.");
	}

	assert(f.adjFace(e) != nullptr);
	const FaceType& f1 = *(f.adjFace(e));

	int e1 = f1.indexOfAdjFace(&f);
	assert(e1 != -1);

	const auto& vf0 = *(f.vertexMod((int) e - 1));
	const auto& vf1 = *(f1.vertexMod(e1 - 1));

	auto n0 = faceNormal(f);
	auto n1 = faceNormal(f1);

	auto off0 = n0 * vf0.coord();
	auto off1 = n1 * vf1.coord();

	auto dist01 = off0 - n0 * vf1.coord();
	auto dist10 = off1 - n1 * vf0.coord();

	auto sign = std::abs(dist01) > std::abs(dist10) ? dist01 : dist10;

	auto angleRad = n0.angle(n1);
	return sign > 0 ? angleRad : -angleRad;
}

} // namespace vcl
