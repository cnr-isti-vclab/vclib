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

#include "mesh_sampler.h"

#include <vclib/algorithms/polygon.h>

namespace vcl {

template<MeshConcept MeshType>
MeshSampler<MeshType>::MeshSampler()
{
	vcl::enableIfPerVertexNormalOptional(m);
	vcl::enableIfPerVertexScalarOptional(m);
	if constexpr (vcl::HasName<MeshType>) {
		m.name() = "Sampling";
	}
}

template<MeshConcept MeshType>
const MeshType& MeshSampler<MeshType>::samples() const
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
template<MeshConcept OMeshType>
void MeshSampler<MeshType>::addVertex(const typename OMeshType::VertexType& v, const OMeshType& mm)
{
	uint vi = m.addVertex(v.coord());
	m.vertex(vi).importFrom(v);

	setBirthElement(vi, "birthVertex", mm.index(v));
}

template<MeshConcept MeshType>
template<EdgeMeshConcept OMeshType>
void MeshSampler<MeshType>::addEdge(
	const typename OMeshType::EdgeType& e,
	const OMeshType&                    mm,
	double                              u,
	bool                                copyScalar)
{
	using EdgeType = typename OMeshType::EdgeType;
	uint vi        = m.addVertex((e.vertex(0).coord() * (1 - u)) + (e.vertex(1).coord() * u));

	if constexpr (vcl::HasPerVertexScalar<MeshType> && vcl::edge::HasScalar<EdgeType>) {
		if (copyScalar) {
			if (vcl::isPerVertexScalarEnabled(m) && e.isScalarEnabled()) {
				m.vertex(vi).scalar() = e.scalar();
			}
		}
	}

	setBirthElement(vi, "birthEdge", mm.index(e));
}

template<MeshConcept MeshType>
template<FaceMeshConcept OMeshType>
void MeshSampler<MeshType>::addFace(
	const typename OMeshType::FaceType& f,
	const OMeshType&                    mm,
	bool                                copyNormal,
	bool                                copyScalar)
{
	uint vi = m.addVertex(vcl::polygonBarycenter(f));

	copyComponents(vi, f, copyNormal, copyScalar);
	setBirthElement(vi, "birthFace", mm.index(f));
}

template<MeshConcept MeshType>
template<FaceMeshConcept OMeshType>
void MeshSampler<MeshType>::addFace(
	const typename OMeshType::FaceType& f,
	const OMeshType&                    mm,
	const std::vector<ScalarType>&      barCoords,
	bool                                copyNormal,
	bool                                copyScalar)
{
	assert(f.vertexNumber() <= weights.size());

	PointType p;
	for (uint i = 0; i < f.vertexNumber(); i++)
		p += f.vertex(i)->coord() * barCoords[i];

	uint vi = m.addVertex(p);

	copyComponents(vi, f, copyNormal, copyScalar);
	setBirthElement(vi, "birthFace", mm.index(f));
}

template<MeshConcept MeshType>
template<FaceMeshConcept OMeshType>
void MeshSampler<MeshType>::addFace(
	const typename OMeshType::FaceType& f,
	const OMeshType&                    mm,
	const PointType&                    barCoords,
	bool                                copyNormal,
	bool                                copyScalar)
{
	using FaceType = typename OMeshType::FaceType;
	static_assert(FaceType::NV == 3 || FaceType::NV == -1);
	if constexpr(FaceType::NV == -1) {
		assert(f.vertexNumber() == 3);
	}

	PointType p = triangleBarycentricCoordinatePoint(f, barCoords);

	uint vi = m.addVertex(p);

	copyComponents(vi, f, copyNormal, copyScalar);
	setBirthElement(vi, "birthFace", mm.index(f));
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

template<MeshConcept MeshType>
void MeshSampler<MeshType>::setBirthElement(uint vi, const std::string& key, uint value)
{
	if constexpr(vcl::HasPerVertexCustomComponents<MeshType>) {
		if (!m.hasPerVertexCustomComponent(key)) {
			m.template addPerVertexCustomComponent<uint>(key);
		}
		m.vertex(vi).template customComponent<uint>(key) = value;
	}
}

} // namespace vcl
