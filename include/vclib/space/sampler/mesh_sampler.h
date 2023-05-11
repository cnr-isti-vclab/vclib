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

#ifndef VCL_SPACE_SAMPLER_MESH_SAMPLER_H
#define VCL_SPACE_SAMPLER_MESH_SAMPLER_H

#include <vclib/concepts/mesh/elements/edge.h>
#include <vclib/concepts/space/sampler.h>
#include <vclib/mesh/requirements.h>
#include <vclib/views/mesh.h>

namespace vcl {

template<MeshConcept MeshType>
class MeshSampler
{
#ifdef VCLIB_USES_RANGES
	using CoordView = decltype(typename MeshType::VertexView() | views::coords);
#endif
public:
	using PointType  = typename MeshType::VertexType::CoordType;
	using ScalarType = typename PointType::ScalarType;
#ifdef VCLIB_USES_RANGES
	using ConstIterator = std::ranges::iterator_t<CoordView>;
#endif
	MeshSampler();

	const MeshType& samples() const;

	const PointType& sample(uint i) const;
	std::size_t size() const;

	void clear();
	void resize(uint n);
	void reserve(uint n);

	void add(const PointType& p);
	void set(uint i, const PointType& p);

	template<VertexConcept VertexType>
	void add(const VertexType& v);

	template<VertexConcept VertexType>
	void set(uint i, const VertexType& v);

	template<EdgeConcept EdgeType>
	void add(const EdgeType& e, double u, bool copyScalar = true);

	template<EdgeConcept EdgeType>
	void set(uint i, const EdgeType& e, double u, bool copyScalar = true);

	template<FaceConcept FaceType>
	void add(const FaceType& f, bool copyNormal = false, bool copyScalar = true);

	template<FaceConcept FaceType>
	void set(uint i, const FaceType& f, bool copyNormal = false, bool copyScalar = true);

	template<FaceConcept FaceType>
	void
	add(const FaceType&                f,
		const std::vector<ScalarType>& barCoords,
		bool                           copyNormal = false,
		bool                           copyScalar = true);

	template<FaceConcept FaceType>
	void
	set(uint                           i,
		const FaceType&                f,
		const std::vector<ScalarType>& barCoords,
		bool                           copyNormal = false,
		bool                           copyScalar = true);

	template<FaceConcept FaceType>
	void
	add(const FaceType&  f,
		const PointType& barCoords,
		bool             copyNormal = false,
		bool             copyScalar = true);

	template<FaceConcept FaceType>
	void
	set(uint             i,
		const FaceType&  f,
		const PointType& barCoords,
		bool             copyNormal = false,
		bool             copyScalar = true);

#ifdef VCLIB_USES_RANGES
	ConstIterator begin() const;
	ConstIterator end() const;
#else
	auto points() const;
#endif

private:

	template<FaceConcept FaceType>
	void copyComponents(uint vi, const FaceType&  f, bool copyNormal, bool copyScalar);

	void setBirthElement(uint vi, const std::string& key, uint value);

	MeshType m;
};

} // namespace vcl

#include "mesh_sampler.cpp"

#endif // VCL_SPACE_SAMPLER_MESH_SAMPLER_H
