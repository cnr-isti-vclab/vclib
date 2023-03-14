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

#include "point_sampler.h"

#include <vclib/algorithm/polygon.h>

namespace vcl {

template<PointConcept PointType>
const std::vector<PointType>& PointSampler<PointType>::samples() const
{
	return samplesVec;
}

template<PointConcept PointType>
void PointSampler<PointType>::clear()
{
	samplesVec.clear();
}

template<PointConcept PointType>
void PointSampler<PointType>::reserve(uint n)
{
	samplesVec.reserve(n);
}

template<PointConcept PointType>
void PointSampler<PointType>::resize(uint n)
{
	samplesVec.resize(n);
}

template<PointConcept PointType>
void PointSampler<PointType>::add(const PointType& p)
{
	samplesVec.push_back(p);
}

template<PointConcept PointType>
void PointSampler<PointType>::set(uint i, const PointType& p)
{
	samplesVec[i] = p;
}

template<PointConcept PointType>
template<VertexConcept VertexType>
void PointSampler<PointType>::add(
	const VertexType& v)
{
	samplesVec.push_back(v.coord());
}

template<PointConcept PointType>
template<VertexConcept VertexType>
void PointSampler<PointType>::set(
	uint i,
	const VertexType& v)
{
	samplesVec[i] = v.coord();
}

template<PointConcept PointType>
template<EdgeConcept EdgeType>
void PointSampler<PointType>::add(const EdgeType& e, double u)
{
	samplesVec.push_back((e.vertex(0).coord()*(1-u)) + (e.vertex(1).coord()*u));
}

template<PointConcept PointType>
template<EdgeConcept EdgeType>
void PointSampler<PointType>::set(uint i, const EdgeType& e, double u)
{
	samplesVec[i] = (e.vertex(0).coord()*(1-u)) + (e.vertex(1).coord()*u);
}

template<PointConcept PointType>
template<FaceConcept FaceType>
void PointSampler<PointType>::add(const FaceType& f)
{
	samplesVec.push_back(vcl::faceBarycenter(f));
}

template<PointConcept PointType>
template<FaceConcept FaceType>
void PointSampler<PointType>::set(uint i, const FaceType& f)
{
	samplesVec[i] = vcl::faceBarycenter(f);
}

template<PointConcept PointType>
template<FaceConcept FaceType>
void PointSampler<PointType>::add(const FaceType& f, const std::vector<ScalarType>& barCoords)
{
	assert(f.vertexNumber() <= barCoords.size());

	PointType p;
	for (uint i = 0; i < f.vertexNumber(); i++)
		p += f.vertex(i)->coord() * barCoords[i];

	samplesVec.push_back(p);
}

template<PointConcept PointType>
template<FaceConcept FaceType>
void PointSampler<PointType>::set(
	uint                           i,
	const FaceType&                f,
	const std::vector<ScalarType>& barCoords)
{
	assert(f.vertexNumber() <= barCoords.size());

	PointType p;
	for (uint i = 0; i < f.vertexNumber(); i++)
		p += f.vertex(i)->coord() * barCoords[i];

	samplesVec[i] = p;
}

template<PointConcept PointType>
template<FaceConcept FaceType>
void PointSampler<PointType>::add(const FaceType& f, const PointType& barCoords)
{
	static_assert(FaceType::NV == 3 || FaceType::NV == -1);
	if constexpr(FaceType::NV == -1) {
		assert(f.vertexNumber() == 3);
	}

	PointType p = triangleBarycentricCoordinatePoint(f, barCoords);

	samplesVec.push_back(p);
}

template<PointConcept PointType>
template<FaceConcept FaceType>
void PointSampler<PointType>::set(uint i, const FaceType& f, const PointType& barCoords)
{
	static_assert(FaceType::NV == 3 || FaceType::NV == -1);
	if constexpr(FaceType::NV == -1) {
		assert(f.vertexNumber() == 3);
	}

	PointType p = triangleBarycentricCoordinatePoint(f, barCoords);

	samplesVec[i] = p;
}

} // namespace vcl
