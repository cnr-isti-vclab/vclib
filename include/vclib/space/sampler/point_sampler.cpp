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

#include <vclib/algorithms/polygon.h>

namespace vcl {

template<PointConcept PointT>
const std::vector<PointT>& PointSampler<PointT>::samples() const
{
	return samplesVec;
}

template<PointConcept PointT>
const PointT& PointSampler<PointT>::sample(uint i) const
{
	return samplesVec[i];
}

template<PointConcept PointT>
std::size_t PointSampler<PointT>::size() const
{
	return samplesVec.size();
}

template<PointConcept PointT>
void PointSampler<PointT>::clear()
{
	samplesVec.clear();
}

template<PointConcept PointT>
void PointSampler<PointT>::reserve(uint n)
{
	samplesVec.reserve(n);
}

template<PointConcept PointT>
void PointSampler<PointT>::resize(uint n)
{
	samplesVec.resize(n);
}

template<PointConcept PointT>
void PointSampler<PointT>::add(const PointT& p)
{
	samplesVec.push_back(p);
}

template<PointConcept PointT>
void PointSampler<PointT>::set(uint i, const PointT& p)
{
	samplesVec[i] = p;
}

template<PointConcept PointT>
typename PointSampler<PointT>::ConstIterator PointSampler<PointT>::begin() const
{
	return samplesVec.begin();
}

template<PointConcept PointT>
typename PointSampler<PointT>::ConstIterator PointSampler<PointT>::end() const
{
	return samplesVec.end();
}

#ifndef VCLIB_USES_RANGES
template<PointConcept PointT>
const std::vector<PointT>& PointSampler<PointT>::points() const
{
	return samplesVec;
}
#endif

template<PointConcept PointT>
template<VertexConcept VertexType>
void PointSampler<PointT>::add(
	const VertexType& v)
{
	samplesVec.push_back(v.coord());
}

template<PointConcept PointT>
template<VertexConcept VertexType>
void PointSampler<PointT>::set(
	uint i,
	const VertexType& v)
{
	samplesVec[i] = v.coord();
}

template<PointConcept PointT>
template<EdgeConcept EdgeType>
void PointSampler<PointT>::add(const EdgeType& e, double u)
{
	samplesVec.push_back((e.vertex(0).coord()*(1-u)) + (e.vertex(1).coord()*u));
}

template<PointConcept PointT>
template<EdgeConcept EdgeType>
void PointSampler<PointT>::set(uint i, const EdgeType& e, double u)
{
	samplesVec[i] = (e.vertex(0).coord()*(1-u)) + (e.vertex(1).coord()*u);
}

template<PointConcept PointT>
template<FaceConcept FaceType>
void PointSampler<PointT>::add(const FaceType& f)
{
	samplesVec.push_back(vcl::faceBarycenter(f));
}

template<PointConcept PointT>
template<FaceConcept FaceType>
void PointSampler<PointT>::set(uint i, const FaceType& f)
{
	samplesVec[i] = vcl::faceBarycenter(f);
}

template<PointConcept PointT>
template<FaceConcept FaceType>
void PointSampler<PointT>::add(const FaceType& f, const std::vector<ScalarType>& barCoords)
{
	assert(f.vertexNumber() <= barCoords.size());

	PointT p;
	for (uint i = 0; i < f.vertexNumber(); i++)
		p += f.vertex(i)->coord() * barCoords[i];

	samplesVec.push_back(p);
}

template<PointConcept PointT>
template<FaceConcept FaceType>
void PointSampler<PointT>::set(
	uint                           i,
	const FaceType&                f,
	const std::vector<ScalarType>& barCoords)
{
	assert(f.vertexNumber() <= barCoords.size());

	PointT p;
	for (uint i = 0; i < f.vertexNumber(); i++)
		p += f.vertex(i)->coord() * barCoords[i];

	samplesVec[i] = p;
}

template<PointConcept PointT>
template<FaceConcept FaceType>
void PointSampler<PointT>::add(const FaceType& f, const PointT& barCoords)
{
	static_assert(FaceType::NV == 3 || FaceType::NV == -1);
	if constexpr(FaceType::NV == -1) {
		assert(f.vertexNumber() == 3);
	}

	PointT p = triangleBarycentricCoordinatePoint(f, barCoords);

	samplesVec.push_back(p);
}

template<PointConcept PointT>
template<FaceConcept FaceType>
void PointSampler<PointT>::set(uint i, const FaceType& f, const PointT& barCoords)
{
	static_assert(FaceType::NV == 3 || FaceType::NV == -1);
	if constexpr(FaceType::NV == -1) {
		assert(f.vertexNumber() == 3);
	}

	PointT p = triangleBarycentricCoordinatePoint(f, barCoords);

	samplesVec[i] = p;
}

} // namespace vcl
