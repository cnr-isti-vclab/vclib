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

template<MeshConcept MeshType, SamplerConcept SamplerType, typename GridType>
HausdorffDistResult hausdorffDist(const MeshType& m, const SamplerType& s, const GridType& g)
{
	HausdorffDistResult res;
	res.histogram = Histogramd(0, m.boundingBox().diagonal() / 100, 100);

	// todo

	return res;
}

template<MeshConcept MeshType, SamplerConcept SamplerType>
HausdorffDistResult samplerMeshHausdorff(const MeshType& m, const SamplerType& s)
	requires(!HasFaces<MeshType>)
{
	using VertexType = typename MeshType::VertexType;
	using VPI = vcl::PointerIterator<typename MeshType::VertexIterator>;

	vcl::StaticGrid3<const VertexType*> grid(VPI(m.vertexBegin()), VPI(m.vertexEnd()));

	return hausdorffDist(m, s, grid);
}

template<FaceMeshConcept MeshType, SamplerConcept SamplerType>
HausdorffDistResult samplerMeshHausdorff(const MeshType& m, const SamplerType& s)
{
	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;
	using VPI = vcl::PointerIterator<typename MeshType::ConstVertexIterator>;
	using FPI = vcl::PointerIterator<typename MeshType::ConstFaceIterator>;

	vcl::StaticGrid3<const VertexType*> vgrid;
	vcl::StaticGrid3<const FaceType*> fgrid;

	if (m.faceNumber() == 0) {
		vcl::StaticGrid3<const VertexType*> grid(VPI(m.vertexBegin()), VPI(m.vertexEnd()));
		return hausdorffDist(m, s, grid);
	}
	else {
		vcl::StaticGrid3<const FaceType*> grid(FPI(m.faceBegin()), FPI(m.faceEnd()));
		return hausdorffDist(m, s, grid);
	}
}

template<MeshConcept MeshType1, MeshConcept MeshType2, SamplerConcept SamplerType>
HausdorffDistResult vertUniformHausdorffDistance(
	const MeshType1& m1,
	const MeshType2& m2,
	uint nSamples,
	bool deterministic,
	SamplerType& sampler,
	std::vector<uint>& birthVertices)
{
	sampler = vcl::vertexUniformPointSampling<SamplerType>(
		m2, nSamples, birthVertices, false, deterministic);

	return samplerMeshHausdorff(m1, sampler);
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
