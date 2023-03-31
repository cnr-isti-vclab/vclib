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

#include "scalar.h"

#include "../polygon.h"
#include "../stat.h"

namespace vcl {

/**
 * @brief Sets a constant value to all the vertex scalars of the mesh.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Scalar
 *
 * @param[in,out] m: mesh on which set the vertex scalars
 * @param[in] s: scalar value to set
 */
template<MeshConcept MeshType>
void setPerVertexScalar(MeshType& m, typename MeshType::VertexType::ScalarType s)
{
	vcl::requirePerVertexScalar(m);

	using VertexType = typename MeshType::VertexType;

	for (VertexType& v : m.vertices()) {
		v.scalar() = s;
	}
}

/**
 * @brief Sets a constant value to all the face scalars of the mesh.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Scalar
 *
 * @param[in,out] m: mesh on which set the face scalars
 * @param[in] s: scalar value to set
 */
template <FaceMeshConcept MeshType>
void setPerFaceScalar(MeshType& m, typename MeshType::FaceType::ScalarType s)
{
	vcl::requirePerFaceScalar(m);

	using FaceType = typename MeshType::FaceType;

	for (FaceType& f : m.faces()) {
		f.scalar() = s;
	}
}

/**
 * @brief Clamps the vertex scalars of a mesh in a given interval.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Scalar
 *
 * @param[in,out] m: mesh on which clamp the vertex scalars
 * @param[in] minS: minimum value of the clamping interval
 * @param[in] maxS: maximum value of the clamping interval
 */
template<MeshConcept MeshType>
void clampPerVertexScalar(
	MeshType&                                 m,
	typename MeshType::VertexType::ScalarType minS,
	typename MeshType::VertexType::ScalarType maxS)
{
	vcl::requirePerVertexScalar(m);

	using VertexType = typename MeshType::VertexType;

	for (VertexType& v : m.vertices()) {
		v.scalar() = std::min(maxS, std::max(minS, v.scalar()));
	}
}

/**
 * @brief Clamps the face scalars of a mesh in a given interval.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Scalar
 *
 * @param[in,out] m: mesh on which clamp the face scalars
 * @param[in] minS: minimum value of the clamping interval
 * @param[in] maxS: maximum value of the clamping interval
 */
template<FaceMeshConcept MeshType>
void clampPerFaceScalar(
	MeshType&                                 m,
	typename MeshType::FaceType::ScalarType minS,
	typename MeshType::FaceType::ScalarType maxS)
{
	vcl::requirePerFaceScalar(m);

	using FaceType = typename MeshType::FaceType;

	for (FaceType& f : m.faces()) {
		f.scalar() = std::min(maxS, std::max(minS, f.scalar()));
	}
}

/**
 * @brief Normalizes the vertex scalars of a mesh in a given interval (default [0, 1]).
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Scalar
 *
 * @param[in,out] m: mesh on which normalize the vertex scalars
 * @param[in] minS: minimum value of the normalizing interval, default 0
 * @param[in] maxS: maximum value of the normalizing interval, default 1
 */
template<MeshConcept MeshType>
void normalizePerVertexScalar(
	MeshType&                                 m,
	typename MeshType::VertexType::ScalarType minS,
	typename MeshType::VertexType::ScalarType maxS)
{
	vcl::requirePerVertexScalar(m);

	using VertexType = typename MeshType::VertexType;
	using ScalarType = typename VertexType::ScalarType;

	ScalarType range = maxS - minS;
	std::pair<ScalarType, ScalarType> p = vertexScalarMinMax(m);

	for (VertexType& v : m.vertices()) {
		v.scalar() = minS + range * ((v.scalar() - p.first)/(p.second-p.first));
	}
}

/**
 * @brief Normalizes the face scalars of a mesh in a given interval (default [0, 1]).
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Scalar
 *
 * @param[in,out] m: mesh on which normalize the face scalars
 * @param[in] minS: minimum value of the normalizing interval, default 0
 * @param[in] maxS: maximum value of the normalizing interval, default 1
 */
template<FaceMeshConcept MeshType>
void normalizePerFaceScalar(
	MeshType&                                 m,
	typename MeshType::FaceType::ScalarType minS,
	typename MeshType::FaceType::ScalarType maxS)
{
	vcl::requirePerFaceScalar(m);

	using FaceType = typename MeshType::FaceType;
	using ScalarType = typename FaceType::ScalarType;

	ScalarType range = maxS - minS;
	std::pair<ScalarType, ScalarType> p = faceScalarMinMax(m);

	for (FaceType& f : m.faces()) {
		f.scalar() = minS + range * ((f.scalar() - p.first)/(p.second-p.first));
	}
}

/**
 * @brief Assign to the vertex scalars of the mesh the valence of each vertex, that is the number of
 * adjacent faces of the vertex.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Scalar
 *   - Faces
 *
 * @param[in,out] m: mesh on which set the vertex scalars to the vertex valence
 */
template<FaceMeshConcept MeshType>
void setPerVertexScalarFromVertexValence(MeshType& m)
{
	vcl::requirePerVertexScalar(m);

	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;

	setPerVertexScalar(m, 0);

	for (FaceType& f : m.faces()) {
		for (VertexType* v : f.vertices()) {
			v->scalar() += 1;
		}
	}
}

/**
 * @brief Assign to the face scalars of the mesh the area of each face.
 *
 * Requirements:
 * - Mesh:
 *   - Faces
 *
 * @param[in,out] m: mesh on which set the face scalars to the face area
 */
template<FaceMeshConcept MeshType>
void setPerFaceScalarFromFaceArea(MeshType& m)
{
	vcl::requirePerFaceScalar(m);

	using FaceType = typename MeshType::FaceType;

	for (FaceType& f : m.faces()) {
		f.scalar() = faceArea(f);
	}
}

template<MeshConcept MeshType>
void setPerVertexScalarFromPrincipalCurvatureGaussian(MeshType &m)
{
	vcl::requirePerVertexScalar(m);
	vcl::requirePerVertexPrincipalCurvature(m);

	using VertexType = typename MeshType::VertexType;

	for (VertexType& v : m.vertices()) {
		v.scalar() = v.principalCurvature().maxValue() * v.principalCurvature().minValue();
	}
}

template<MeshConcept MeshType>
void setPerVertexScalarFromPrincipalCurvatureMean(MeshType& m)
{
	vcl::requirePerVertexScalar(m);
	vcl::requirePerVertexPrincipalCurvature(m);

	using VertexType = typename MeshType::VertexType;

	for (VertexType& v : m.vertices()) {
		v.scalar() = (v.principalCurvature().maxValue() + v.principalCurvature().minValue()) / 2;
	}
}

template<MeshConcept MeshType>
void setPerVertexScalarFromPrincipalCurvatureMinValue(MeshType& m)
{
	vcl::requirePerVertexScalar(m);
	vcl::requirePerVertexPrincipalCurvature(m);

	using VertexType = typename MeshType::VertexType;

	for (VertexType& v : m.vertices()) {
		v.scalar() = v.principalCurvature().minValue();
	}
}

template<MeshConcept MeshType>
void setPerVertexScalarFromPrincipalCurvatureMaxValue(MeshType& m)
{
	vcl::requirePerVertexScalar(m);
	vcl::requirePerVertexPrincipalCurvature(m);

	using VertexType = typename MeshType::VertexType;

	for (VertexType& v : m.vertices()) {
		v.scalar() = v.principalCurvature().maxValue();
	}
}

/**
 * @brief Computes the Shape Index S from the Principal Curvature, as defined by [Koenderink 1992]
 * and stores it in the per-vertex scalar.
 *
 * S = 2/pi atan(k1+k2/k1-k2)
 *
 * J. Koenderink and A. van Doorn.
 * Surface shape and curvature scales.
 * Image and vision computing, 10(8):557–565, 1992.
 * @param m
 */
template<MeshConcept MeshType>
void setPerVertexScalarFromPrincipalCurvatureShapeIndex(MeshType& m)
{
	vcl::requirePerVertexScalar(m);
	vcl::requirePerVertexPrincipalCurvature(m);

	using VertexType = typename MeshType::VertexType;
	using ScalarType = typename VertexType::PrincipalCurvatureType::ScalarType;

	for (VertexType& v : m.vertices()) {
		ScalarType k1 = v.principalCurvature().maxValue();
		ScalarType k2 = v.principalCurvature().minValue();
		if (k1 < k2) std::swap(k1, k2);
		v.scalar() = (2.0 / M_PI) * std::atan2(k1 + k2, k1 - k2);
	}
}

/**
 * @brief Computes the Curvedness C from the Principal Curvature, as defined by [Koenderink 1992]
 * and stores it in the per-vertex scalar.
 *
 * C =  Sqrt((k1*k1+k2*k2)/2.0)
 *
 * J. Koenderink and A. van Doorn.
 * Surface shape and curvature scales.
 * Image and vision computing, 10(8):557–565, 1992.
 * @param m
 */
template<MeshConcept MeshType>
void setPerVertexScalarFromPrincipalCurvatureCurvedness(MeshType& m)
{
	vcl::requirePerVertexScalar(m);
	vcl::requirePerVertexPrincipalCurvature(m);

	using VertexType = typename MeshType::VertexType;
	using ScalarType = typename VertexType::PrincipalCurvatureType::ScalarType;

	for (VertexType& v : m.vertices()) {
		ScalarType k1 = v.principalCurvature().maxValue();
		ScalarType k2 = v.principalCurvature().minValue();

		v.scalar() = std::sqrt((k1 * k1 + k2 * k2) / 2.0);
	}
}

} // namespace vcl
