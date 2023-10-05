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

#include <vclib/algorithms/polygon.h>
#include <vclib/concepts/space/sampler.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

template<PointConcept PointT = vcl::Point3d>
class PointSampler
{
	std::vector<PointT> samplesVec;

public:
	using PointType = PointT;
	using ScalarType = PointT::ScalarType;
	using ConstIterator = std::vector<PointT>::const_iterator;

	PointSampler() = default;

	const std::vector<PointT>& samples() const { return samplesVec; }

	const PointT& sample(uint i) const { return samplesVec[i]; }

	std::size_t size() const { return samplesVec.size(); }

	void clear() { samplesVec.clear(); }

	void reserve(uint n) { samplesVec.reserve(n); }

	void resize(uint n) { samplesVec.resize(n); }

	void add(const PointT& p) { samplesVec.push_back(p); }

	void set(uint i, const PointT& p) { samplesVec[i] = p; }

	template<VertexConcept VertexType>
	void add(const VertexType& v)
	{
		samplesVec.push_back(v.coord());
	}

	template<VertexConcept VertexType>
	void set(uint i, const VertexType& v)
	{
		samplesVec[i] = v.coord();
	}

	template<EdgeConcept EdgeType>
	void add(const EdgeType& e, double u = 0.5)
	{
		samplesVec.push_back(
			(e.vertex(0).coord() * (1 - u)) + (e.vertex(1).coord() * u));
	}

	template<EdgeConcept EdgeType>
	void set(uint i, const EdgeType& e, double u = 0.5)
	{
		samplesVec[i] =
			(e.vertex(0).coord() * (1 - u)) + (e.vertex(1).coord() * u);
	}

	template<FaceConcept FaceType>
	void add(const FaceType& f)
	{
		samplesVec.push_back(vcl::faceBarycenter(f));
	}

	template<FaceConcept FaceType>
	void set(uint i, const FaceType& f)
	{
		samplesVec[i] = vcl::faceBarycenter(f);
	}

	template<FaceConcept FaceType>
	void add(const FaceType& f, const std::vector<ScalarType>& barCoords)
	{
		assert(f.vertexNumber() <= barCoords.size());

		PointT p;
		for (uint i = 0; i < f.vertexNumber(); i++)
			p += f.vertex(i)->coord() * barCoords[i];

		samplesVec.push_back(p);
	}

	template<FaceConcept FaceType>
	void
	set(uint i, const FaceType& f, const std::vector<ScalarType>& barCoords)
	{
		assert(f.vertexNumber() <= barCoords.size());

		PointT p;
		for (uint i = 0; i < f.vertexNumber(); i++)
			p += f.vertex(i)->coord() * barCoords[i];

		samplesVec[i] = p;
	}

	template<FaceConcept FaceType>
	void add(const FaceType& f, const PointT& barCoords)
	{
		static_assert(FaceType::NV == 3 || FaceType::NV == -1);
		if constexpr(FaceType::NV == -1) {
			assert(f.vertexNumber() == 3);
		}

		PointT p = triangleBarycentricCoordinatePoint(f, barCoords);

		samplesVec.push_back(p);
	}

	template<FaceConcept FaceType>
	void set(uint i, const FaceType& f, const PointT& barCoords)
	{
		static_assert(FaceType::NV == 3 || FaceType::NV == -1);
		if constexpr(FaceType::NV == -1) {
			assert(f.vertexNumber() == 3);
		}

		PointT p = triangleBarycentricCoordinatePoint(f, barCoords);

		samplesVec[i] = p;
	}

	ConstIterator begin() const { return samplesVec.begin(); }

	ConstIterator end() const { return samplesVec.end(); }
};

} // namespace vcl

#endif // VCL_SPACE_SAMPLER_POINT_SAMPLER_H
