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

#ifndef VCL_SPACE_SAMPLER_POINT_SAMPLER_H
#define VCL_SPACE_SAMPLER_POINT_SAMPLER_H

#include <vclib/mesh/requirements.h>

#include "sampler_concept.h"

namespace vcl {

template<typename PointType = vcl::Point3d>
class PointSampler
{
public:
	using ScalarType = typename PointType::ScalarType;

	PointSampler() = default;

	const std::vector<PointType>& samples() const;

	void clear();
	void reserve(uint n);

	void addPoint(const PointType& p);

	template<MeshConcept MeshType>
	void addVertex(const typename MeshType::VertexType& v, const MeshType&);

	template<EdgeMeshConcept MeshType>
	void addEdge(const typename MeshType::EdgeType& e, const MeshType&, double u = 0.5);

	template<FaceMeshConcept MeshType>
	void addFace(const typename MeshType::FaceType& f, const MeshType&);

	template<FaceMeshConcept MeshType>
	void addFace(
		const typename MeshType::FaceType& f,
		const MeshType&,
		const std::vector<ScalarType>&     barCoords);

	template<FaceMeshConcept MeshType>
	void addFace(
		const typename MeshType::FaceType& f,
		const MeshType&,
		const PointType&                   barCoords);

private:
	std::vector<PointType> samplesVec;
};

// makes sure that the VertexIndexSampler satisfies SamplerConcept
static_assert(FaceSamplerConcept<PointSampler<>>, "PointSampler is not a valid FaceSampler");

} // namespace vcl

#include "point_sampler.cpp"

#endif // VCL_SPACE_SAMPLER_POINT_SAMPLER_H
