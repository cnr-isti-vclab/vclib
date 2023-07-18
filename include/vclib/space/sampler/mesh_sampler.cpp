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

#include "mesh_sampler.h"

#include <vclib/algorithms/polygon.h>

namespace vcl {

template<MeshConcept MeshType>
MeshSampler<MeshType>::MeshSampler()
{
	vcl::enableIfPerVertexNormalOptional(m);
	vcl::enableIfPerVertexQualityOptional(m);
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
const typename MeshSampler<MeshType>::PointType& MeshSampler<MeshType>::sample(uint i) const
{
	return m.vertex(i).coord();
}

template<MeshConcept MeshType>
std::size_t MeshSampler<MeshType>::size() const
{
	return m.vertexNumber();
}

template<MeshConcept MeshType>
void MeshSampler<MeshType>::clear()
{
	m.clear();
}

template<MeshConcept MeshType>
void MeshSampler<MeshType>::resize(uint n)
{
	if (n > m.vertexNumber()) {
		uint k = n - m.vertexNumber();
		m.addVertices(k);
	}
}

template<MeshConcept MeshType>
void MeshSampler<MeshType>::reserve(uint n)
{
	m.reserveVertices(n);
}

template<MeshConcept MeshType>
void MeshSampler<MeshType>::add(const PointType& p)
{
	m.addVertex(p);
}

template<MeshConcept MeshType>
void MeshSampler<MeshType>::set(uint i, const PointType& p)
{
	m.vertex(i).coord() = p;
}

template<MeshConcept MeshType>
template<VertexConcept VertexType>
void MeshSampler<MeshType>::add(const VertexType& v)
{
	uint vi = m.addVertex(v.coord());
	m.vertex(vi).importFrom(v);

	setBirthElement(vi, "birthVertex", v.index());
}

template<MeshConcept MeshType>
template<VertexConcept VertexType>
void MeshSampler<MeshType>::set(uint i, const VertexType& v)
{
	m.vertex(i).coord() = v.coord();
	m.vertex(i).importFrom(v);

	setBirthElement(i, "birthVertex", v.index());
}

template<MeshConcept MeshType>
template<EdgeConcept EdgeType>
void MeshSampler<MeshType>::add(const EdgeType& e, double u, bool copyQuality)
{
	uint vi        = m.addVertex((e.vertex(0).coord() * (1 - u)) + (e.vertex(1).coord() * u));

	if constexpr (vcl::HasPerVertexQuality<MeshType> && vcl::edge::HasQuality<EdgeType>) {
		if (copyQuality) {
			if (vcl::isPerVertexQualityEnabled(m) && comp::isQualityEnabledOn(e)) {
				m.vertex(vi).quality() = e.quality();
			}
		}
	}

	setBirthElement(vi, "birthEdge", e.index());
}

template<MeshConcept MeshType>
template<EdgeConcept EdgeType>
void MeshSampler<MeshType>::set(uint i, const EdgeType& e, double u, bool copyQuality)
{
	m.vertex(i).coord() = (e.vertex(0).coord() * (1 - u)) + (e.vertex(1).coord() * u);

	if constexpr (vcl::HasPerVertexQuality<MeshType> && vcl::edge::HasQuality<EdgeType>) {
		if (copyQuality) {
			if (vcl::isPerVertexQualityEnabled(m) && comp::isQualityEnabledOn(e)) {
				m.vertex(i).quality() = e.quality();
			}
		}
	}

	setBirthElement(i, "birthEdge", e.index());
}

template<MeshConcept MeshType>
template<FaceConcept FaceType>
void MeshSampler<MeshType>::add(const FaceType& f, bool copyNormal, bool copyQuality)
{
	uint vi = m.addVertex(vcl::faceBarycenter(f));

	copyComponents(vi, f, copyNormal, copyQuality);
	setBirthElement(vi, "birthFace", f.index());
}

template<MeshConcept MeshType>
template<FaceConcept FaceType>
void MeshSampler<MeshType>::set(uint i, const FaceType& f, bool copyNormal, bool copyQuality)
{
	m.vertex(i).coord() = vcl::faceBarycenter(f);

	copyComponents(i, f, copyNormal, copyQuality);
	setBirthElement(i, "birthFace", f.index());
}

template<MeshConcept MeshType>
template<FaceConcept FaceType>
void MeshSampler<MeshType>::add(
	const FaceType& f,
	const std::vector<ScalarType>&      barCoords,
	bool                                copyNormal,
	bool                                copyQuality)
{
	assert(f.vertexNumber() <= barCoords.size());

	PointType p;
	for (uint i = 0; i < f.vertexNumber(); i++)
		p += f.vertex(i)->coord() * barCoords[i];

	uint vi = m.addVertex(p);

	copyComponents(vi, f, copyNormal, copyQuality);
	setBirthElement(vi, "birthFace", f.index());
}

template<MeshConcept MeshType>
template<FaceConcept FaceType>
void MeshSampler<MeshType>::set(
	uint                                i,
	const FaceType& f,
	const std::vector<ScalarType>&      barCoords,
	bool                                copyNormal,
	bool                                copyQuality)
{
	assert(f.vertexNumber() <= barCoords.size());

	PointType p;
	for (uint i = 0; i < f.vertexNumber(); i++)
		p += f.vertex(i)->coord() * barCoords[i];

	m.vertex(i).coord() = p;

	copyComponents(i, f, copyNormal, copyQuality);
	setBirthElement(i, "birthFace", f.index());
}

template<MeshConcept MeshType>
template<FaceConcept FaceType>
void MeshSampler<MeshType>::add(
	const FaceType& f,
	const PointType&                    barCoords,
	bool                                copyNormal,
	bool                                copyQuality)
{
	static_assert(FaceType::NV == 3 || FaceType::NV == -1);
	if constexpr(FaceType::NV == -1) {
		assert(f.vertexNumber() == 3);
	}

	PointType p = triangleBarycentricCoordinatePoint(f, barCoords);

	uint vi = m.addVertex(p);

	copyComponents(vi, f, copyNormal, copyQuality);
	setBirthElement(vi, "birthFace", f.index());
}

template<MeshConcept MeshType>
template<FaceConcept FaceType>
void MeshSampler<MeshType>::set(
	uint i,
	const FaceType& f,
	const PointType&                    barCoords,
	bool                                copyNormal,
	bool                                copyQuality)
{
	static_assert(FaceType::NV == 3 || FaceType::NV == -1);
	if constexpr(FaceType::NV == -1) {
		assert(f.vertexNumber() == 3);
	}

	PointType p = triangleBarycentricCoordinatePoint(f, barCoords);

	m.vertex(i).coord() = p;

	copyComponents(i, f, copyNormal, copyQuality);
	setBirthElement(i, "birthFace", f.index());
}

template<MeshConcept MeshType>
typename MeshSampler<MeshType>::ConstIterator MeshSampler<MeshType>::begin() const
{
	return std::ranges::begin(m.vertices() | views::coords);
}

template<MeshConcept MeshType>
typename MeshSampler<MeshType>::ConstIterator MeshSampler<MeshType>::end() const
{
	return std::ranges::end(m.vertices() | views::coords);
}

template<MeshConcept MeshType>
template<FaceConcept FaceType>
void MeshSampler<MeshType>::copyComponents(
	uint            vi,
	const FaceType& f,
	bool            copyNormal,
	bool            copyQuality)
{
	if constexpr (vcl::HasPerVertexNormal<MeshType> && vcl::face::HasNormal<FaceType>) {
		if (copyNormal) {
			if (vcl::isPerVertexNormalEnabled(m) && comp::isNormalEnabledOn(f)) {
				m.vertex(vi).normal() = f.normal();
			}
		}
	}

	if constexpr (vcl::HasPerVertexQuality<MeshType> && vcl::face::HasQuality<FaceType>) {
		if (copyQuality) {
			if (vcl::isPerVertexQualityEnabled(m) && comp::isQualityEnabledOn(f)) {
				m.vertex(vi).quality() = f.quality();
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
