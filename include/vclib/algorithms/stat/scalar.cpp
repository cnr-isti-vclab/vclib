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

#include <vclib/math/base.h>

#include <vclib/mesh/iterators/components/scalar_iterator.h>

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

/**
 * @brief When performing an adptive pruning for each sample we expect a varying radius to be
 * removed.
 * The radius is a PerVertex attribute that we compute from the current per vertex scalar component.
 * The expected radius of the sample is computed so that it linearly maps the quality between
 * diskradius and diskradius*variance
 *
 * @param m
 * @param diskRadius
 * @param radiusVariance
 * @param invert
 * @return
 */
template<MeshConcept MeshType>
std::vector<typename MeshType::VertexType::ScalarType> vertexRadiusFromScalar(
	const MeshType& m,
	double diskRadius,
	double radiusVariance,
	bool invert)
{
	vcl::requirePerVertexScalar(m);

	using VertexType = typename MeshType::VertexType;
	using ScalarType = typename VertexType::ScalarType;

	std::vector<ScalarType> radius(m.vertexContainerSize());
	std::pair<ScalarType, ScalarType> minmax = vertexScalarMinMax(m);
	float minRad = diskRadius;
	float maxRad = diskRadius * radiusVariance;
	float deltaQ = minmax.second - minmax.first;
	float deltaRad = maxRad - minRad;
	for (const VertexType& v : m.vertices()) {
		radius[m.index(v)] =
			minRad +
			deltaRad * ((invert ? minmax.second - v.scalar() : v.scalar() - minmax.first) / deltaQ);
	}

	return radius;
}

template<MeshConcept MeshType, typename HScalar>
Histogram<HScalar> vertexScalarHistogram(const MeshType& m, bool selectionOnly, uint histSize)
{
	vcl::requirePerVertexScalar(m);

	using VertexType = typename MeshType::VertexType;
	using ScalarType = typename VertexType::ScalarType;

	auto minmax = vertexScalarMinMax(m);

	vcl::Histogram<HScalar> h(minmax.first, minmax.second, histSize);
	for (const VertexType& v : m.vertices()) {
		if (!selectionOnly || v.isSelected()) {
			assert(!isDegenerate(v.scalar()));
			h.addValue(v.scalar());
		}
	}

//	for (auto it = ConstVertexScalarIterator<MeshType>(m.vertexBegin());
//		 it != ConstVertexScalarIterator<MeshType>(m.vertexEnd());
//		++it) {
//		assert(!isDegenerate(*it));
//		h.addValue(*it);
//	}
	return h;
}

template<FaceMeshConcept MeshType, typename HScalar>
Histogram<HScalar> faceScalarHistogram(const MeshType& m, bool selectionOnly, uint histSize)
{
	vcl::requirePerFaceScalar(m);

	using FaceType = typename MeshType::FaceType;
	using ScalarType = typename FaceType::ScalarType;

	auto minmax = vertexScalarMinMax(m);

	vcl::Histogram<HScalar> h(minmax.first, minmax.second, histSize);
	for (const FaceType& f : m.faces()) {
		if (!selectionOnly || f.isSelected()) {
			assert(!isDegenerate(f.scalar()));
			h.addValue(f.scalar());
		}
	}
	return h;
}

} // namespace vcl
