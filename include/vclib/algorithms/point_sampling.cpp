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
			sampler.addVertex(v);
	}
	return sampler;
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

	if (nSamples > m.vertexNumber()) {
		return allVerticesSampling<SamplerType>(m);
	}

	SamplerType ps;

	// todo
	// create distribution of vertices using their scalar
	// and use the distribution for a random generator
	// and generate then nSamples vertices to add into ps

	return ps;
}

} // namespace vcl

