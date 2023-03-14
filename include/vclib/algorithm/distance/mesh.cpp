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

#include "mesh.h"

#include <vclib/algorithm/point_sampling.h>
#include <vclib/iterator/pointer_iterator.h>
#include <vclib/space/spatial_data_structures.h>

namespace vcl {

namespace internal {

template<MeshConcept MeshType1, MeshConcept MeshType2, SamplerConcept SamplerType>
HausdorffDistResult vertUniformHausdorffDistance(
	const MeshType1& m1,
	const MeshType2& m2,
	uint nSamples,
	bool deterministic,
	SamplerType& sampler,
	std::vector<uint>& birthVertices)
{
	using VertexType1 = typename MeshType1::VertexType;
	using VPI = vcl::PointerIterator<typename MeshType1::VertexIterator>;

	HausdorffDistResult res;
	vcl::StaticGrid3<const VertexType1*> grid(VPI(m1.vertexBegin()), VPI(m1.vertexEnd()));

	res.histogram = Histogramd(0, m1.boundingBox().diagonal() / 100, 100);

	sampler = vcl::vertexUniformPointSampling<SamplerType>(
		m2, nSamples, birthVertices, false, deterministic);

	// loop on all the samples
	// for each sample
	//     compute closest between m1 vertices (stored in grid) and sample
	//     update results

	return res;
}

} // namespace vcl::internal

template<MeshConcept MeshType1, MeshConcept MeshType2>
HausdorffDistResult hausdorffDistance(
	const MeshType1& m1,
	const MeshType2& m2,
	uint nSamples,
	HausdorffSamplingMethod sampMethod,
	bool deterministic)
{
	if (nSamples == 0)
		nSamples = m2.vertexNumber();

	std::vector<uint> birth;

	switch (sampMethod) {
	case VERTEX_UNIFORM: {
		//ToDo VertexReferenceSampler
		VertexIndexSampler sampler;

		return internal::vertUniformHausdorffDistance(
			m1, m2, nSamples, deterministic, sampler, birth);
	}

	case EDGE_UNIFORM: {
		// todo
		return HausdorffDistResult();
	}
	case MONTECARLO: {
		// todo
		return HausdorffDistResult();
	default:
		assert(0);
		return HausdorffDistResult();
	}
	}
}

} // namespace vcl
