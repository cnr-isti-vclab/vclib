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

#ifndef VCL_SPACE_SAMPLER_MESH_SAMPLER_H
#define VCL_SPACE_SAMPLER_MESH_SAMPLER_H

#include <vclib/mesh/requirements.h>

#include "sampler_concept.h"

namespace vcl {

template<MeshConcept MeshType>
class MeshSampler
{
public:
	using PointType  = typename MeshType::VertexType::CoordType;
	using ScalarType = typename PointType::ScalarType;

	MeshSampler();

	const MeshType& samples() const;

	void clear();
	void reserve(uint n);

	void addPoint(const PointType& p);

	template<MeshConcept OMeshType>
	void addVertex(const typename OMeshType::VertexType& v, const OMeshType& m);

	template<EdgeMeshConcept OMeshType>
	void addEdge(
		const typename OMeshType::EdgeType& e,
		const OMeshType&,
		double u,
		bool   copyScalar = true);

	template<FaceMeshConcept OMeshType>
	void addFace(
		const typename OMeshType::FaceType& f,
		const OMeshType&,
		bool copyNormal = false,
		bool copyScalar = true);

	template<FaceMeshConcept OMeshType>
	void addFace(
		const typename OMeshType::FaceType& f,
		const OMeshType&,
		const std::vector<ScalarType>&      weights,
		bool                                copyNormal = false,
		bool                                copyScalar = true);

	template<FaceMeshConcept OMeshType>
	void addFace(
		const typename OMeshType::FaceType&  f,
		const OMeshType&,
		const PointType&                     weights,
		bool                                 copyNormal = false,
		bool                                 copyScalar = true);

private:

	template<FaceConcept FaceType>
	void copyComponents(uint vi, const FaceType&  f, bool copyNormal, bool copyScalar);

	void setBirthElement(uint vi, const std::string& key, uint value);

	MeshType m;
};

// makes sure that the MeshSampler satisfies SamplerConcept
static_assert(
	SamplerConcept<MeshSampler<internal::TMPSimplePolyMesh>>,
	"MeshSampler is not a valid Sampler");

} // namespace vcl

#include "mesh_sampler.cpp"

#endif // VCL_SPACE_SAMPLER_MESH_SAMPLER_H
