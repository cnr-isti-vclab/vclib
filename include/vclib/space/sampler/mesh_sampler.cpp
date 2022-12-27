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

#include "mesh_sampler.h"

#include <vclib/algorithms/polygon.h>

namespace vcl {

template<MeshConcept MeshType>
MeshSampler<MeshType>::MeshSampler()
{
	vcl::enableIfPerVertexNormalOptional(m);
	vcl::enableIfPerVertexScalarOptional(m);
}

template<MeshConcept MeshType>
const MeshType &MeshSampler<MeshType>::samples() const
{
	return m;
}

template<MeshConcept MeshType>
void MeshSampler<MeshType>::clear()
{
	m.clear();
}

template<MeshConcept MeshType>
void MeshSampler<MeshType>::reserve(uint n)
{
	m.reserveVertices(n);
}

template<MeshConcept MeshType>
void MeshSampler<MeshType>::addPoint(const PointType& p)
{
	m.addVertex(p);
}

template<MeshConcept MeshType>
template<VertexConcept VertexType>
void MeshSampler<MeshType>::addVertex(const VertexType& v)
{
	uint vi = m.addVertex(v.coord());
	m.vertex(vi).importFrom(v);
}

template<MeshConcept MeshType>
template<EdgeConcept EdgeType>
void MeshSampler<MeshType>::addEdge(const EdgeType& e, double u, bool copyScalar)
{
	uint vi = m.addVertex((e.vertex(0).coord()*(1-u)) + (e.vertex(1).coord()*u));

	if constexpr (vcl::HasPerVertexScalar<MeshType> && vcl::edge::HasScalar<EdgeType>) {
		if (copyScalar) {
			if (vcl::isPerVertexScalarEnabled(m) && e.isScalarEnabled()) {
				m.vertex(vi).scalar() = e.scalar();
			}
		}
	}
}

template<MeshConcept MeshType>
template<FaceConcept FaceType>
void MeshSampler<MeshType>::addFace(const FaceType& f, bool copyNormal, bool copyScalar)
{
	uint vi = m.addVertex(vcl::polygonBarycenter(f));

	copyComponents(vi, f, copyNormal, copyScalar);
}

template<MeshConcept MeshType>
template<FaceConcept FaceType>
void MeshSampler<MeshType>::addFace(
	const FaceType&                f,
	const std::vector<ScalarType>& weights,
	bool                           copyNormal,
	bool                           copyScalar)
{
	assert(f.vertexNumber <= weights.size());

	PointType p;
	for (uint i = 0; i < f.vertexNumber(); i++)
		p += f.vertex(i)->coord() * weights[i];

	uint vi = m.addVertex(p);

	copyComponents(vi, f, copyNormal, copyScalar);
}

template<MeshConcept MeshType>
template<FaceConcept FaceType>
void MeshSampler<MeshType>::addFace(
	const FaceType&  f,
	const PointType& weights,
	bool             copyNormal,
	bool             copyScalar)
{
	static_assert(FaceType::NV == 3);

	PointType p;
	for (uint i = 0; i < 3; i++)
		p += f.vertex(i)->coord() * weights(i);

	uint vi = m.addVertex(p);

	copyComponents(vi, f, copyNormal, copyScalar);
}

template<MeshConcept MeshType>
template<FaceConcept FaceType>
void MeshSampler<MeshType>::copyComponents(
	uint            vi,
	const FaceType& f,
	bool            copyNormal,
	bool            copyScalar)
{
	if constexpr (vcl::HasPerVertexNormal<MeshType> && vcl::face::HasNormal<FaceType>) {
		if (copyNormal) {
			if (vcl::isPerVertexNormalEnabled(m) && f.isNormalEnabled()) {
				m.vertex(vi).normal() = f.normal();
			}
		}
	}

	if constexpr (vcl::HasPerVertexScalar<MeshType> && vcl::face::HasScalar<FaceType>) {
		if (copyScalar) {
			if (vcl::isPerVertexScalarEnabled(m) && f.isScalarEnabled()) {
				m.vertex(vi).scalar() = f.scalar();
			}
		}
	}
}

} // namespace vcl
