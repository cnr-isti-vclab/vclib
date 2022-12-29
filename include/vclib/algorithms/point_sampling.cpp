/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#include "point_sampling.h"

#include <vclib/algorithms/shuffle.h>

namespace vcl {

template<SamplerConcept SamplerType, MeshConcept MeshType>
SamplerType allVerticesSampling(const MeshType &m, bool onlySelected)
{
	using VertexType = typename MeshType::VertexType;

	SamplerType sampler;
	for (const VertexType& v : m.vertices()) {
		if (!onlySelected || v.isSelected())
			sampler.addVertex(v, m);
	}
	return sampler;
}

/**
 * @brief Samples the vertices in a weighted way, using the per vertex weights given as input.
 * Each vertex has a probability of being chosen that is proportional to its weight.
 *
 * @param m
 * @param weights: a vector of scalars having the i-th entry associated to the vertex having index i.
 *                 Note: weights.size() == m.vertexContainerSize().
 * @param nSamples
 * @return A Sampler, that is a collection of samples selected from the input mesh.
 */
template<SamplerConcept SamplerType, MeshConcept MeshType, typename ScalarType>
SamplerType vertexWeightedSampling(
	const MeshType& m,
	const std::vector<ScalarType>& weights,
	uint nSamples)
{
	if (nSamples >= m.vertexNumber()) {
		return allVerticesSampling<SamplerType>(m);
	}

	SamplerType ps;

	std::discrete_distribution<> dist(std::begin(weights), std::end(weights));

	static std::random_device rd;
	std::mt19937 gen(rd());

	std::vector<bool> visited(m.vertexContainerSize(), false);
	uint nVisited = 0;

	while (nVisited < nSamples) {
		uint vi = dist(gen);
		if (vi < m.vertexContainerSize() && !visited[vi]) {
			visited[vi] = true;
			nVisited++;
			ps.addVertex(m.vertex(vi), m);
		}
	}

	return ps;
}

/**
 * @brief Samples the vertices in a weighted way, using the per vertex Scalar component. Each vertex
 * has a probability of being chosen that is proportional to its scalar value.
 *
 * @param m
 * @param nSamples
 * @return
 */
template<SamplerConcept SamplerType, MeshConcept MeshType>
SamplerType vertexScalarWeightedSampling(const MeshType& m, uint nSamples)
{
	vcl::requirePerVertexScalar(m);

	using VertexType = typename MeshType::VertexType;
	using ScalarType = typename VertexType::ScalarType;

	std::vector<ScalarType> weights;
	weights.resize(m.vertexContainerSize(), 0);
	for (const VertexType& v : m.vertices()) {
		weights[m.index(v)] = v.scalar();
	}

	return vertexWeightedSampling<SamplerType>(m, weights, nSamples);
}

template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType vertexAreaWeightedSampling(const MeshType& m, uint nSamples)
{
	using VertexType = typename MeshType::VertexType;
	using ScalarType = typename VertexType::ScalarType;
	using FaceType = typename MeshType::FaceType;

	std::vector<ScalarType> weights(m.vertexContainerSize(), 0);
	std::vector<uint> cnt(m.vertexContainerSize(), 0);

	// for each vertex, store in weights the adjacent faces area and their number
	for (const FaceType& f : m.faces()) {
		ScalarType area = vcl::polygonArea(f);
		for (const VertexType* v : f.vertices()) {
			weights[m.index(v)] += area;
			cnt[m.index(v)]++;
		}
	}

	// divide each area sum by the number of adjacent faces
	for (uint i = 0; i < weights.size(); i++) {
		if (cnt[i] > 0)
			weights[i] /= cnt[i];
	}

	// use these weights to create a sapler
	return vertexWeightedSampling<SamplerType>(m, weights, nSamples);
}

} // namespace vcl

