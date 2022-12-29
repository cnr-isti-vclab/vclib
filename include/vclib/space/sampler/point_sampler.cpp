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

#include "point_sampler.h"

#include <vclib/algorithms/polygon.h>

namespace vcl {

template<typename PointType>
const std::vector<PointType>& PointSampler<PointType>::samples() const
{
	return samplesVec;
}

template<typename PointType>
void PointSampler<PointType>::clear()
{
	samplesVec.clear();
}

template<typename PointType>
void PointSampler<PointType>::reserve(uint n)
{
	samplesVec.reserve(n);
}

template<typename PointType>
void PointSampler<PointType>::addPoint(const PointType& p)
{
	samplesVec.push_back(p);
}

template<typename PointType>
template<MeshConcept MeshType>
void PointSampler<PointType>::addVertex(
	const typename MeshType::VertexType& v,
	const MeshType&)
{
	samplesVec.push_back(v.coord());
}

template<typename PointType>
template<EdgeMeshConcept MeshType>
void PointSampler<PointType>::addEdge(
	const typename MeshType::EdgeType& e,
	const MeshType&,
	double u)
{
	samplesVec.push_back((e.vertex(0).coord()*(1-u)) + (e.vertex(1).coord()*u));
}

template<typename PointType>
template<FaceMeshConcept MeshType>
void PointSampler<PointType>::addFace(const typename MeshType::FaceType& f, const MeshType&)
{
	samplesVec.push_back(vcl::polygonBarycenter(f));
}

template<typename PointType>
template<FaceMeshConcept MeshType>
void PointSampler<PointType>::addFace(
	const typename MeshType::FaceType& f,
	const std::vector<ScalarType>&     weights,
	const MeshType&)
{
	assert(f.vertexNumber() <= weights.size());

	PointType p;
	for (uint i = 0; i < f.vertexNumber(); i++)
		p += f.vertex(i)->coord() * weights[i];

	samplesVec.push_back(p);
}

template<typename PointType>
template<FaceMeshConcept MeshType>
void PointSampler<PointType>::addFace(
	const typename MeshType::FaceType& f,
	const PointType&                   weights,
	const MeshType&)
{
	using FaceType = typename MeshType::FaceType;
	static_assert(FaceType::NV == 3 || FaceType::NV == -1);
	if constexpr(FaceType::NV == -1) {
		assert(f.vertexNumber() == 3);
	}

	PointType p;
	for (uint i = 0; i < 3; i++)
		p += f.vertex(i)->coord() * weights(i);

	samplesVec.push_back(p);
}

} // namespace vcl
