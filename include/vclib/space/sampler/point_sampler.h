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

#ifndef VCL_SPACE_SAMPLER_POINT_SAMPLER_H
#define VCL_SPACE_SAMPLER_POINT_SAMPLER_H

#include <vclib/concepts/space/sampler.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

template<PointConcept PointT = vcl::Point3d>
class PointSampler
{
public:
	using PointType = PointT;
	using ScalarType = typename PointT::ScalarType;
	using ConstIterator = typename std::vector<PointT>::const_iterator;

	PointSampler() = default;

	const std::vector<PointT>& samples() const;

	const PointT& sample(uint i) const;

	std::size_t size() const;

	void clear();
	void reserve(uint n);
	void resize(uint n);

	void add(const PointT& p);
	void set(uint i, const PointT& p);

	template<VertexConcept VertexType>
	void add(const VertexType& v);

	template<VertexConcept VertexType>
	void set(uint i, const VertexType& v);

	template<EdgeConcept EdgeType>
	void add(const EdgeType& e, double u = 0.5);

	template<EdgeConcept EdgeType>
	void set(uint i, const EdgeType& e, double u = 0.5);

	template<FaceConcept FaceType>
	void add(const FaceType& f);

	template<FaceConcept FaceType>
	void set(uint i, const FaceType& f);

	template<FaceConcept FaceType>
	void add(const FaceType& f, const std::vector<ScalarType>& barCoords);

	template<FaceConcept FaceType>
	void set(uint i, const FaceType& f, const std::vector<ScalarType>& barCoords);

	template<FaceConcept FaceType>
	void add(const FaceType& f, const PointT& barCoords);

	template<FaceConcept FaceType>
	void set(uint i, const FaceType& f, const PointT& barCoords);

	ConstIterator begin() const;
	ConstIterator end() const;

private:
	std::vector<PointT> samplesVec;
};

} // namespace vcl

#include "point_sampler.cpp"

#endif // VCL_SPACE_SAMPLER_POINT_SAMPLER_H
