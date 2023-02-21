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

#include "normal.h"

#include "../polygon.h"

namespace vcl {

/**
 * @brief Normalizes the length of the face normals.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Normal
 *
 * @param[in,out] m: the mesh on which normalize the face normals.
 */
template<FaceMeshConcept MeshType>
void normalizePerFaceNormals(MeshType& m)
{
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
template<FaceMeshConcept MeshType>
void updatePerFaceNormals(MeshType& m, bool normalize)
{
	vcl::requirePerFaceNormal(m);

	using FaceType = typename MeshType::FaceType;
	if constexpr (vcl::HasTriangles<MeshType>) {
		for (FaceType& f : m.faces()) {
			f.normal() = triangleNormal(f).
						 template cast<typename FaceType::NormalType::ScalarType>();
		}
		
	}
	else {
		for (FaceType& f : m.faces()) {
			f.normal() = polygonNormal(f).
						 template cast<typename FaceType::NormalType::ScalarType>();
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
 * @param[in,out] m: The mesh on which clear the vertex normals.
 */
template<MeshConcept MeshType>
void clearPerVertexNormals(MeshType& m)
{
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
 * @param[in,out] m: The mesh on which clear the referenced vertex normals.
 */
template<FaceMeshConcept MeshType>
void clearPerReferencedVertexNormals(MeshType& m)
{
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
 * @param[in,out] m: the mesh on which normalize the vertex normals.
 */
template<MeshConcept MeshType>
void normalizePerVertexNormals(MeshType& m)
{
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
 * @param[in,out] m: the mesh on which compute the vertex normals.
 * @param[in] normalize: if true (default), normals are normalized after computation.
 */
template<FaceMeshConcept MeshType>
void updatePerVertexNormals(MeshType& m, bool normalize)
{
	clearPerReferencedVertexNormals(m);

	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;
	using NormalType = typename VertexType::NormalType;

	for (FaceType& f : m.faces()) {
		NormalType n = polygonNormal(f).template cast<typename NormalType::ScalarType>();
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
 * @param[in,out] m: the mesh on which compute the vertex normals.
 * @param[in] normalize: if true (default), normals are normalized after computation.
 */
template<FaceMeshConcept MeshType>
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
 * @param[in,out] m: the mesh on which compute the angle weighted vertex normals.
 * @param[in] normalize: if true (default), normals are normalized after computation.
 */
template<FaceMeshConcept MeshType>
void updatePerVertexNormalsAngleWeighted(MeshType& m, bool normalize)
{
	clearPerReferencedVertexNormals(m);

	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;
	using NormalType = typename VertexType::NormalType;
	using NScalarType = typename NormalType::ScalarType;

	for (FaceType& f : m.faces()) {
		NormalType n = polygonNormal(f).template cast<NScalarType>();

		for (uint i = 0; i < f.vertexNumber(); ++i) {
			NormalType vec1 = (f.vertexMod(i - 1)->coord() - f.vertexMod(i)->coord())
								  .normalized()
								  .template cast<NScalarType>();
			NormalType vec2 = (f.vertexMod(i + 1)->coord() - f.vertexMod(i)->coord())
								  .normalized()
								  .template cast<NScalarType>();

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
 * @param[in,out] m: the mesh on which compute the Max et al. weighted vertex normals.
 * @param[in] normalize: if true (default), normals are normalized after computation.
 */
template<FaceMeshConcept MeshType>
void updatePerVertexNormalsNelsonMaxWeighted(MeshType& m, bool normalize)
{
	clearPerReferencedVertexNormals(m);

	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;
	using NormalType = typename VertexType::NormalType;
	using NScalarType = typename NormalType::ScalarType;

	for (FaceType& f : m.faces()) {
		NormalType n = polygonNormal(f).template cast<NScalarType>();

		for (uint i = 0; i < f.vertexNumber(); ++i) {
			NScalarType e1 = (f.vertexMod(i - 1)->coord() - f.vertexMod(i)->coord()).squaredNorm();
			NScalarType e2 = (f.vertexMod(i + 1)->coord() - f.vertexMod(i)->coord()).squaredNorm();

			f.vertex(i)->normal() += n / (e1 * e2);
		}
	}
	if (normalize)
		normalizePerVertexNormals(m);
}

/**
 * @brief Multiplies the Face Normals by the given TRS 4x4 Matrix.
 * By default, the scale component is removed from the matrix.
 *
 * Requirements:
 * - Mesh:
 *   - Faces
 *     - Normal
 *
 * @param[in,out] mesh: the mesh on which multiply the face normals.
 * @param[in] mat: the 4x4 TRS matrix that is multiplied to the normals.
 * @param[in] removeScalingFromMatrix: if true (default), the scale component is removed from the
 * matrix.
 */
template<FaceMeshConcept MeshType, typename MScalar>
void multiplyPerFaceNormalsByMatrix(
	MeshType&                     mesh,
	const vcl::Matrix44<MScalar>& mat,
	bool                          removeScalingFromMatrix)
{
	requirePerFaceNormal(mesh);

	using FaceType = typename MeshType::FaceType;

	Matrix33<MScalar> m33 = mat.block(0, 0, 3, 3);
	if (removeScalingFromMatrix) {
		MScalar scaleX =
			std::sqrt(m33(0, 0) * m33(0, 0) + m33(0, 1) * m33(0, 1) + m33(0, 2) * m33(0, 2));
		MScalar scaleY =
			std::sqrt(m33(1, 0) * m33(1, 0) + m33(1, 1) * m33(1, 1) + m33(1, 2) * m33(1, 2));
		MScalar scaleZ =
			std::sqrt(m33(2, 0) * m33(2, 0) + m33(2, 1) * m33(2, 1) + m33(2, 2) * m33(2, 2));
		for (int i = 0; i < 3; ++i) {
			m33(0, i) /= scaleX;
			m33(1, i) /= scaleY;
			m33(2, i) /= scaleZ;
		}
	}
	for (FaceType& f : mesh.faces()) {
		f.normal() *= m33;
	}
}

/**
 * @brief Multiplies the Vertex Normals by the given TRS 4x4 Matrix.
 * By default, the scale component is removed from the matrix.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *
 * @param[in,out] mesh: the mesh on which multiply the vertex normals.
 * @param[in] mat: the 4x4 TRS matrix that is multiplied to the normals.
 * @param[in] removeScalingFromMatrix: if true (default), the scale component is removed from the
 * matrix.
 */
template<MeshConcept MeshType, typename MScalar>
void multiplyPerVertexNormalsByMatrix(
	MeshType&                     mesh,
	const vcl::Matrix44<MScalar>& mat,
	bool                          removeScalingFromMatrix)
{
	requirePerVertexNormal(mesh);

	using VertexType = typename MeshType::VertexType;

	Matrix33<MScalar> m33 = mat.block(0, 0, 3, 3);
	if (removeScalingFromMatrix) {
		MScalar scaleX =
			std::sqrt(m33(0, 0) * m33(0, 0) + m33(0, 1) * m33(0, 1) + m33(0, 2) * m33(0, 2));
		MScalar scaleY =
			std::sqrt(m33(1, 0) * m33(1, 0) + m33(1, 1) * m33(1, 1) + m33(1, 2) * m33(1, 2));
		MScalar scaleZ =
			std::sqrt(m33(2, 0) * m33(2, 0) + m33(2, 1) * m33(2, 1) + m33(2, 2) * m33(2, 2));
		for (int i = 0; i < 3; ++i) {
			m33(0, i) /= scaleX;
			m33(1, i) /= scaleY;
			m33(2, i) /= scaleZ;
		}
	}
	for (VertexType& v : mesh.vertices()) {
		v.normal() *= m33;
	}
}

} // namespace vcl
