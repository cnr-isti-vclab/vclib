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

#include <vclib/concept/space/sampler.h>
#include <vclib/mesh/iterator.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

template<MeshConcept MeshType>
class MeshSampler
{
public:
	using PointType  = typename MeshType::VertexType::CoordType;
	using ScalarType = typename PointType::ScalarType;
	using ConstIterator = CoordIterator<typename MeshType::ConstVertexIterator>;

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

	ConstIterator begin() const;
	ConstIterator end() const;

private:

	template<FaceConcept FaceType>
	void copyComponents(uint vi, const FaceType&  f, bool copyNormal, bool copyScalar);

	void setBirthElement(uint vi, const std::string& key, uint value);

	MeshType m;
};

} // namespace vcl

#include "mesh_sampler.cpp"

#endif // VCL_SPACE_SAMPLER_MESH_SAMPLER_H
