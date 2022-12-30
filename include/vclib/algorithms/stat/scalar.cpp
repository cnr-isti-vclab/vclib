/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

namespace vcl {

/**
 * @brief Returns a pair containing the min and the maximum vertex scalars.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Scalar
 *
 * @param[in] m: the input Mesh on which compute the minimum and the maximum scalars.
 * @return A `std::pair` having as first element the minimum, and as second element the maximum
 * scalar.
 */
template<MeshConcept MeshType>
std::pair<typename MeshType::VertexType::ScalarType, typename MeshType::VertexType::ScalarType>
vertexScalarMinMax(const MeshType& m)
{
	vcl::requirePerVertexScalar(m);

	using VertexType = typename MeshType::VertexType;
	using ScalarType = typename VertexType::ScalarType;

	std::pair<ScalarType, ScalarType> p = std::make_pair(
		std::numeric_limits<ScalarType>::max(), std::numeric_limits<ScalarType>::lowest());

	for (const VertexType& v : m.vertices()) {
		if (v.scalar() < p.first)
			p.first = v.scalar();
		if (v.scalar() > p.second)
			p.second = v.scalar();
	}

	return p;
}

/**
 * @brief Returns a pair containing the min and the maximum face scalars.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Scalar
 *
 * @param[in] m: the input Mesh on which compute the minimum and the maximum scalars.
 * @return A `std::pair` having as first element the minimum, and as second element the maximum
 * scalar.
 */
template<FaceMeshConcept MeshType>
std::pair<typename MeshType::FaceType::ScalarType, typename MeshType::FaceType::ScalarType>
faceScalarMinMax(const MeshType& m)
{
	vcl::requirePerFaceScalar(m);

	using FaceType   = typename MeshType::FaceType;
	using ScalarType = typename FaceType::ScalarType;

	std::pair<ScalarType, ScalarType> p = std::make_pair(
		std::numeric_limits<ScalarType>::max(), std::numeric_limits<ScalarType>::lowest());

	for (const FaceType& f : m.faces()) {
		if (f.scalar() < p.first)
			p.first = f.scalar();
		if (f.scalar() > p.second)
			p.second = f.scalar();
	}

	return p;
}

/**
 * @brief Returns a scalar that is the average of the vertex scalars.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Scalar
 *
 * @param[in] m: the input Mesh on which compute the average of the scalars.
 * @return The average of the vertex scalars of the given mesh.
 */
template<MeshConcept MeshType>
typename MeshType::VertexType::ScalarType vertexScalarAverage(const MeshType& m)
{
	vcl::requirePerVertexScalar(m);

	using VertexType = typename MeshType::VertexType;
	using ScalarType = typename VertexType::ScalarType;

	ScalarType avg = 0;

	for (const VertexType& v : m.vertices())
		avg += v.scalar();

	return avg / m.vertexNumber();
}

/**
 * @brief Returns a scalar that is the average of the face scalars.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Scalar
 *
 * @param[in] m: the input Mesh on which compute the average of the scalars.
 * @return The average of the face scalars of the given mesh.
 */
template<FaceMeshConcept MeshType>
typename MeshType::FaceType::ScalarType faceScalarAverage(const MeshType& m)
{
	vcl::requirePerFaceScalar(m);

	using FaceType   = typename MeshType::FaceType;
	using ScalarType = typename FaceType::ScalarType;

	ScalarType avg = 0;

	for (const FaceType& f : m.faces())
		avg += f.scalar();

	return avg / m.faceNumber();
}

} // namespace vcl
