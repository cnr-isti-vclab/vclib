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

#include "normal.h"

#include <vclib/mesh/requirements.h>

namespace vcl {

/**
 * @brief Computes the normal of the triangle composed by the points p0, p1, and p2, considering
 * that these three points are ordered in counterclockwise order.
 *
 * @param[in] p0: first point of the triangle.
 * @param[in] p1: second point of the triangle.
 * @param[in] p2: third point of the triangle.
 * @return The normal of the triangle composed by p0, p1 and p2.
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
 * @param[in] t: input triangle of type Face
 * @return The normal of t.
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
 * @param[in] p: input polygonal Face
 * @return The normal of p.
 */
template<typename Polygon, typename NormalType>
NormalType polygonNormal(const Polygon& p)
{
	// compute the sum of normals for each triplet of consecutive points
	NormalType sum;
	sum.setZero();
	for (uint i = 0; i < p.vertexNumber(); ++i) {
		sum += triangleNormal(
			p.vertexMod(i)->coord(), p.vertexMod(i + 1)->coord(), p.vertexMod(i + 2)->coord());
	}
	sum.normalize();
	return sum;
}

/**
 * @brief Normalizes the length of the face normals.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Normal
 *
 * @param[in/out] m: the mesh on which normalize the face normals.
 */
template<typename MeshType>
void normalizePerFaceNormals(MeshType& m)
{
	vcl::requireFaces<MeshType>();
	vcl::requirePerFaceNormal(m);

	using FaceType = typename MeshType::FaceType;

	for (FaceType& f : m.faces()) {
		f.normal().normalize();
	}
}

/**
 * @brief updatePerFaceNormals
 * @param m
 * @param[in] normalize: if true (default), normals are normalized after computation.
 */
template<typename MeshType>
void updatePerFaceNormals(MeshType& m, bool normalize)
{
	vcl::requireVertices<MeshType>();
	vcl::requireFaces<MeshType>();
	vcl::requirePerFaceNormal(m);

	using FaceType = typename MeshType::FaceType;
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

/**
 * @brief Sets to zero the normals of all the vertices of the mesh, including the unreferenced ones.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *
 * @param[in/out] m: The mesh on which clear the vertex normals.
 */
template<typename MeshType>
void clearPerVertexNormals(MeshType& m)
{
	vcl::requireVertices<MeshType>();
	vcl::requirePerVertexNormal(m);

	using VertexType = typename MeshType::VertexType;

	for (VertexType& v : m.vertices()) {
		v.normal().setZero();
	}
}

/**
 * @brief Sets to zero all the normals of vertices that are referenced by at least one face, leaving
 * unchanged all the normals of the unreferenced vertices that may be still useful.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *   - Faces
 *
 * @param[in/out] m: The mesh on which clear the referenced vertex normals.
 */
template<typename MeshType>
void clearPerReferencedVertexNormals(MeshType& m)
{
	vcl::requireVertices<MeshType>();
	vcl::requireFaces<MeshType>();
	vcl::requirePerVertexNormal(m);

	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;

	for (FaceType& f : m.faces()) {
		for (VertexType* v : f.vertices()) {
			v->normal().setZero();
		}
	}
}

/**
 * @brief Normalizes the length of the vertex normals.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *
 * @param[in/out] m: the mesh on which normalize the vertex normals.
 */
template<typename MeshType>
void normalizePerVertexNormals(MeshType& m)
{
	vcl::requireVertices<MeshType>();
	vcl::requirePerVertexNormal(m);

	using VertexType = typename MeshType::VertexType;

	for (VertexType& v : m.vertices()) {
		v.normal().normalize();
	}
}

/**
 * @brief Computes the vertex normal as the classic area weighted average.
 *
 * This function does not need or exploit current face normals. Unreferenced vertex normals are left
 * unchanged.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *   - Faces
 *
 * @param[in/out] m: the mesh on which compute the vertex normals.
 * @param[in] normalize: if true (default), normals are normalized after computation.
 */
template<typename MeshType>
void updatePerVertexNormals(MeshType& m, bool normalize)
{
	clearPerReferencedVertexNormals(m);

	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;
	using NormalType = typename VertexType::NormalType;

	for (FaceType& f : m.faces()) {
		NormalType n = polygonNormal(f);
		for (VertexType* v : f.vertices()) {
			v->normal() += n;
		}
	}
	if (normalize)
		normalizePerVertexNormals(m);
}

/**
 * @brief Computes the vertex normal as the sum of the adjacent faces normals.
 *
 * Unreferenced vertex normals are left unchanged.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *   - Faces
 *     - Normal
 *
 * @param[in/out] m: the mesh on which compute the vertex normals.
 * @param[in] normalize: if true (default), normals are normalized after computation.
 */
template<typename MeshType>
void updatePerVertexNormalsFromFaceNormals(MeshType& m, bool normalize)
{
	vcl::requirePerFaceNormal(m);

	clearPerReferencedVertexNormals(m);

	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;

	for (FaceType& f : m.faces()) {
		for (VertexType* v : f.vertices()) {
			v->normal() += f.normal();
		}
	}
	if (normalize)
		normalizePerVertexNormals(m);
}

/**
 * @brief Computes the vertex normal as an angle weighted average.
 *
 * The normal of a vertex `v` computed as a weighted sum the incident face normals.
 * The weight is simlply the angle of the involved wedge. Described in:
 *
 * ```
 * G. Thurmer, C. A. Wuthrich
 *   "Computing vertex normals from polygonal facets"
 *   Journal of Graphics Tools, 1998
 * ```
 *
 * This function does not need or exploit current face normals. Unreferenced vertex normals are left
 * unchanged.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *   - Faces
 *
 * @param[in/out] m: the mesh on which compute the angle weighted vertex normals.
 * @param[in] normalize: if true (default), normals are normalized after computation.
 */
template<typename MeshType>
void updatePerVertexNormalsAngleWeighted(MeshType& m, bool normalize)
{
	clearPerReferencedVertexNormals(m);

	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;
	using NormalType = typename VertexType::NormalType;

	for (FaceType& f : m.faces()) {
		NormalType n = polygonNormal(f);

		for (int i = 0; i < f.vertexNumber(); ++i) {
			NormalType vec1 = (f.vertexMod(i - 1)->coord() - f.vertexMod(i)->coord()).normalized();
			NormalType vec2 = (f.vertexMod(i + 1)->coord() - f.vertexMod(i)->coord()).normalized();

			f.vertex(i)->normal() += n * vec1.angle(vec2);
		}
	}
	if (normalize)
		normalizePerVertexNormals(m);
}

/**
 * @brief Computes the vertex normal using the Max et al. weighting scheme.
 *
 * The normal of a vertex v is computed according to the formula described in:
 *
 * ```
 * Max, N.,
 *   "Weights for Computing Vertex Normals from Facet Normals",
 *   Journal of Graphics Tools, 4(2) (1999)
 * ```
 *
 * The weight for each wedge is the cross product of the two edge over the product of the square of
 * the two edge lengths. According to the original paper it is perfect only for spherical surface,
 * but it should perform well...
 *
 * This function does not need or exploit current face normals. Unreferenced vertex normals are left
 * unchanged.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *   - Faces
 *
 * @param[in/out] m: the mesh on which compute the Max et al. weighted vertex normals.
 * @param[in] normalize: if true (default), normals are normalized after computation.
 */
template<typename MeshType>
void updatePerVertexNormalsNelsonMaxWeighted(MeshType& m, bool normalize)
{
	clearPerReferencedVertexNormals(m);

	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;
	using NormalType = typename VertexType::NormalType;
	using ScalarType = typename NormalType::ScalarType;

	for (FaceType& f : m.faces()) {
		NormalType n = polygonNormal(f);

		for (int i = 0; i < f.vertexNumber(); ++i) {
			ScalarType e1 = (f.vertexMod(i - 1)->coord() - f.vertexMod(i)->coord()).squaredNorm();
			ScalarType e2 = (f.vertexMod(i + 1)->coord() - f.vertexMod(i)->coord()).squaredNorm();

			f.vertex(i)->normal() += n / (e1*e2);
		}
	}
	if (normalize)
		normalizePerVertexNormals(m);
}

/**
 * @brief Multiplies the Face Normals by the given RTS 4x4 Matrix.
 * By default, the scale component is removed from the matrix.
 *
 * Requirements:
 * - Mesh:
 *   - Faces
 *     - Normal
 *
 * @param[in/out] mesh: the mesh on which multiply the face normals.
 * @param[in] mat: the 4x4 RTS matrix that is multiplied to the normals.
 * @param[in] removeScalingFromMatrix: if true (default), the scale component is removed from the
 * matrix.
 */
template<typename MeshType, typename MScalar>
void multiplyPerFaceNormalsByMatrix(
	MeshType&               mesh,
	vcl::Matrix44<MScalar>& mat,
	bool                    removeScalingFromMatrix)
{
	requirePerFaceNormal(mesh);

	using FaceType   = typename MeshType::FaceType;

	Matrix33<MScalar> m33 = mat.block(0,0,3,3);
	if (removeScalingFromMatrix) {
		MScalar scaleX = std::sqrt(m33(0,0)*m33(0,0) + m33(0,1)*m33(0,1) + m33(0,2)*m33(0,2));
		MScalar scaleY = std::sqrt(m33(1,0)*m33(1,0) + m33(1,1)*m33(1,1) + m33(1,2)*m33(1,2));
		MScalar scaleZ = std::sqrt(m33(2,0)*m33(2,0) + m33(2,1)*m33(2,1) + m33(2,2)*m33(2,2));
		for (int i = 0; i < 3; ++i) {
			m33(0,i) /= scaleX;
			m33(1,i) /= scaleY;
			m33(2,i) /= scaleZ;
		}
	}
	for (FaceType& f : mesh.faces()) {
		f.normal() *= m33;
	}
}

/**
 * @brief Multiplies the Vertex Normals by the given RTS 4x4 Matrix.
 * By default, the scale component is removed from the matrix.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *
 * @param[in/out] mesh: the mesh on which multiply the vertex normals.
 * @param[in] mat: the 4x4 RTS matrix that is multiplied to the normals.
 * @param[in] removeScalingFromMatrix: if true (default), the scale component is removed from the
 * matrix.
 */
template<typename MeshType, typename MScalar>
void multiplyPerVertexNormalsByMatrix(
	MeshType&               mesh,
	vcl::Matrix44<MScalar>& mat,
	bool                    removeScalingFromMatrix)
{
	requirePerVertexNormal(mesh);

	using VertexType   = typename MeshType::VertexType;

	Matrix33<MScalar> m33 = mat.block(0,0,3,3);
	if (removeScalingFromMatrix) {
		MScalar scaleX = std::sqrt(m33(0,0)*m33(0,0) + m33(0,1)*m33(0,1) + m33(0,2)*m33(0,2));
		MScalar scaleY = std::sqrt(m33(1,0)*m33(1,0) + m33(1,1)*m33(1,1) + m33(1,2)*m33(1,2));
		MScalar scaleZ = std::sqrt(m33(2,0)*m33(2,0) + m33(2,1)*m33(2,1) + m33(2,2)*m33(2,2));
		for (int i = 0; i < 3; ++i) {
			m33(0,i) /= scaleX;
			m33(1,i) /= scaleY;
			m33(2,i) /= scaleZ;
		}
	}
	for (VertexType& v : mesh.vertices()) {
		v.normal() *= m33;
	}
}

} // namespace vcl
