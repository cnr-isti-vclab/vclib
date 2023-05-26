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

#include "quality.h"

#include <vclib/math/base.h>
#include <vclib/views/mesh.h>

namespace vcl {

/**
 * @brief Returns a pair containing the min and the maximum vertex quality.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Quality
 *
 * @param[in] m: the input Mesh on which compute the minimum and the maximum quality.
 * @return A `std::pair` having as first element the minimum, and as second element the maximum
 * quality.
 */
template<MeshConcept MeshType>
std::pair<typename MeshType::VertexType::QualityType, typename MeshType::VertexType::QualityType>
vertexQualityMinMax(const MeshType& m)
{
	vcl::requirePerVertexQuality(m);

	using VertexType = typename MeshType::VertexType;
	using QualityType = typename VertexType::QualityType;

#ifdef VCLIB_USES_RANGES
	auto [min, max] = std::ranges::minmax(m.vertices() | views::quality);
#else
	QualityType min = std::numeric_limits<QualityType>::max(), max = std::numeric_limits<QualityType>::lowest();
	
	for (const VertexType& v : m.vertices()) {
		if (v.quality() < min)
			min = v.quality();
		if (v.quality() > max)
			max = v.quality();
	}
#endif

	return std::make_pair(min, max);;
}

/**
 * @brief Returns a pair containing the min and the maximum face quality.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Quality
 *
 * @param[in] m: the input Mesh on which compute the minimum and the maximum quality.
 * @return A `std::pair` having as first element the minimum, and as second element the maximum
 * quality.
 */
template<FaceMeshConcept MeshType>
std::pair<typename MeshType::FaceType::QualityType, typename MeshType::FaceType::QualityType>
faceQualityMinMax(const MeshType& m)
{
	vcl::requirePerFaceQuality(m);

	using FaceType   = typename MeshType::FaceType;
	using QualityType = typename FaceType::QualityType;
	
#ifdef VCLIB_USES_RANGES
	auto [min, max] = std::ranges::minmax(m.faces() | views::quality);
#else
	QualityType min = std::numeric_limits<QualityType>::max(), max = std::numeric_limits<QualityType>::lowest();
	
	for (const FaceType& f : m.faces()) {
		if (f.quality() < min)
			min = f.quality();
		if (f.quality() > max)
			max = f.quality();
	}
#endif

	return std::make_pair(min, max);
}

/**
 * @brief Returns a scalar that is the average of the vertex quality.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Quality
 *
 * @param[in] m: the input Mesh on which compute the average of the quality.
 * @return The average of the vertex quality of the given mesh.
 */
template<MeshConcept MeshType>
typename MeshType::VertexType::QualityType vertexQualityAverage(const MeshType& m)
{
	vcl::requirePerVertexQuality(m);

	using VertexType = typename MeshType::VertexType;
	using QualityType = typename VertexType::QualityType;

	QualityType avg = 0;

	for (const VertexType& v : m.vertices())
		avg += v.quality();

	return avg / m.vertexNumber();
}

/**
 * @brief Returns a scalar that is the average of the face quality.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Quality
 *
 * @param[in] m: the input Mesh on which compute the average of the quality.
 * @return The average of the face quality of the given mesh.
 */
template<FaceMeshConcept MeshType>
typename MeshType::FaceType::QualityType faceQualityAverage(const MeshType& m)
{
	vcl::requirePerFaceQuality(m);

	using FaceType   = typename MeshType::FaceType;
	using QualityType = typename FaceType::QualityType;

	QualityType avg = 0;

	for (const FaceType& f : m.faces())
		avg += f.quality();

	return avg / m.faceNumber();
}

/**
 * @brief When performing an adptive pruning for each sample we expect a varying radius to be
 * removed.
 * The radius is a PerVertex attribute that we compute from the current per vertex quality component.
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
std::vector<typename MeshType::VertexType::QualityType> vertexRadiusFromQuality(
	const MeshType& m,
	double diskRadius,
	double radiusVariance,
	bool invert)
{
	vcl::requirePerVertexQuality(m);

	using VertexType = typename MeshType::VertexType;
	using QualityType = typename VertexType::QualityType;

	std::vector<QualityType> radius(m.vertexContainerSize());
	std::pair<QualityType, QualityType> minmax = vertexQualityMinMax(m);
	float minRad = diskRadius;
	float maxRad = diskRadius * radiusVariance;
	float deltaQ = minmax.second - minmax.first;
	float deltaRad = maxRad - minRad;
	for (const VertexType& v : m.vertices()) {
		radius[m.index(v)] =
			minRad +
			deltaRad * ((invert ? minmax.second - v.quality() : v.quality() - minmax.first) / deltaQ);
	}

	return radius;
}

template<MeshConcept MeshType, typename HScalar>
Histogram<HScalar> vertexQualityHistogram(const MeshType& m, bool selectionOnly, uint histSize)
{
	vcl::requirePerVertexQuality(m);

	using VertexType = typename MeshType::VertexType;

	auto minmax = vertexQualityMinMax(m);

	vcl::Histogram<HScalar> h(minmax.first, minmax.second, histSize);
	for (const VertexType& v : m.vertices()) {
		if (!selectionOnly || v.selected()) {
			assert(!isDegenerate(v.quality()));
			h.addValue(v.quality());
		}
	}
	return h;
}

template<FaceMeshConcept MeshType, typename HScalar>
Histogram<HScalar> faceQualityHistogram(const MeshType& m, bool selectionOnly, uint histSize)
{
	vcl::requirePerFaceQuality(m);

	using FaceType = typename MeshType::FaceType;

	auto minmax = vertexQualityMinMax(m);

	vcl::Histogram<HScalar> h(minmax.first, minmax.second, histSize);
	for (const FaceType& f : m.faces()) {
		if (!selectionOnly || f.selected()) {
			assert(!isDegenerate(f.quality()));
			h.addValue(f.quality());
		}
	}
	return h;
}

} // namespace vcl