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

#include "stat.h"

#include <vclib/mesh/requirements.h>

namespace vcl {

/**
 * @brief Returns a pair containing the min and the maximum vertex scalars.
 * @param[in] m: the input Mesh on which compute the minimum and the maximum scalars.
 * @return A `std::pair` having as first element the minimum, and as second element the maximum
 * scalar.
 */
template<typename MeshType>
std::pair<typename MeshType::VertexType::ScalarType, typename MeshType::VertexType::ScalarType>
perVertexScalarMinMax(const MeshType& m)
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
}

/**
 * @brief Returns a pair containing the min and the maximum face scalars.
 * @param[in] m: the input Mesh on which compute the minimum and the maximum scalars.
 * @return A `std::pair` having as first element the minimum, and as second element the maximum
 * scalar.
 */
template<typename MeshType>
std::pair<typename MeshType::FaceType::ScalarType, typename MeshType::FaceType::ScalarType>
perFaceScalarMinMax(const MeshType& m)
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
}

/**
 * @brief Returns a scalar that is the average of the vertex scalars.
 * @param[in] m: the input Mesh on which compute the average of the scalars.
 * @return The average of the vertex scalars of the given mesh.
 */
template<typename MeshType>
typename MeshType::VertexType::ScalarType perVertexScalarAverage(const MeshType& m)
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
 * @param[in] m: the input Mesh on which compute the average of the scalars.
 * @return The average of the face scalars of the given mesh.
 */
template<typename MeshType>
typename MeshType::FaceType::ScalarType perFaceScalarAverage(const MeshType& m)
{
	vcl::requirePerFaceScalar(m);

	using FaceType   = typename MeshType::FaceType;
	using ScalarType = typename FaceType::ScalarType;

	ScalarType avg = 0;

	for (const FaceType& f : m.faces())
		avg += f.scalar();

	return avg / m.faceNumber();
}

template <typename MeshType>
typename MeshType::VertexType::CoordType barycenter(const MeshType& m)
{
	vcl::requireVertices<MeshType>();

	using VertexType = typename MeshType::VertexType;
	using CoordType = typename VertexType::CoordType;

	CoordType bar;

	for (const VertexType& v : m.vertices()) {
		bar += v.coord();
	}

	return bar / m.vertexNumber();
}

template <typename MeshType>
typename MeshType::VertexType::CoordType scalarWeightedBarycenter(const MeshType& m)
{
	vcl::requirePerVertexScalar(m);

	using VertexType = typename MeshType::VertexType;
	using CoordType = typename VertexType::CoordType;
	using ScalarType = typename VertexType::ScalarType;

	CoordType bar;
	ScalarType weightedSum = 0;

	for (const VertexType& v : m.vertices()) {
		bar += v.coord() * v.scalar();
		weightedSum += v.scalar();
	}

	return bar / weightedSum;
}

} // namespace vcl
