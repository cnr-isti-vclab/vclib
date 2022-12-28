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

#include "shuffle.h"

#include <random>

#include <vclib/misc/timer.h>

namespace vcl {

namespace internal {

/*
 * Generic implementation of fillAndShuffleVertexPointerVector, to avoid code duplication.
 *
 * Templates M and V can be both const and non-const MeshType and VertexType
 */
template<typename M, typename V>
std::vector<V*> genericFASVPV(M m, std::random_device& rd)
{
	std::vector<V*> vec;
	vec.reserve(m.vertexNumber());

	for (V& v : m.vertices()) {
		vec.push_back(&v);
	}

	std::mt19937 g(rd());
	std::shuffle(vec.begin(),vec.end(), g);
	return vec;
}

/*
 * Generic implementation of fillAndShuffleFacePointerVector, to avoid code duplication.
 *
 * Templates M and F can be both const and non-const MeshType and FaceType
 */
template<typename M, typename F>
std::vector<F*> genericFASFPV(M m, std::random_device& rd)
{
	std::vector<F*> vec;
	vec.reserve(m.faceNumber());

	for (F& f : m.faces()) {
		vec.push_back(&f);
	}

	std::mt19937 g(rd());
	std::shuffle(vec.begin(),vec.end(), g);
	return vec;
}

} // namespace internal

template<MeshConcept MeshType>
std::vector<typename MeshType::VertexType*>fillAndShuffleVertexPointerVector(
	MeshType& m,
	std::random_device& rd)
{
	using VertexType = typename MeshType::VertexType;

	return internal::genericFASVPV<MeshType&, VertexType>(m, rd);
}

template<MeshConcept MeshType>
std::vector<typename MeshType::VertexType*> fillAndShuffleVertexPointerVector(MeshType& m)
{
	static std::random_device rd;
	return fillAndShuffleVertexPointerVector(m, rd);
}

template<MeshConcept MeshType>
std::vector<const typename MeshType::VertexType*> fillAndShuffleVertexPointerVector(
	const MeshType& m,
	std::random_device& rd)
{
	using VertexType = typename MeshType::VertexType;
	return internal::genericFASVPV<const MeshType&, const VertexType>(m, rd);
}

template<MeshConcept MeshType>
std::vector<const typename MeshType::VertexType*> fillAndShuffleVertexPointerVector(
	const MeshType& m)
{
	static std::random_device rd;
	return fillAndShuffleVertexPointerVector(m, rd);
}

template<MeshConcept MeshType>
std::vector<uint>
fillAndShuffleVertexIndexVector(const MeshType& m, std::random_device& rd)
{
	using VertexType = typename MeshType::VertexType;

	std::vector<uint> vec;
	vec.reserve(m.vertexNumber());

	for (const VertexType& v : m.vertices()) {
		vec.push_back(m.index(v));
	}

	std::mt19937 g(rd());
	std::shuffle(vec.begin(),vec.end(), g);
	return vec;
}

template<MeshConcept MeshType>
std::vector<uint>
fillAndShuffleVertexIndexVector(const MeshType& m)
{
	static std::random_device rd;
	return fillAndShuffleVertexIndexVector(m, rd);
}

template<FaceMeshConcept MeshType>
std::vector<typename MeshType::FaceType*>
fillAndShuffleFacePointerVector(MeshType& m, std::random_device& rd)
{
	using FaceType = typename MeshType::FaceType;

	return internal::genericFASFPV<MeshType&, FaceType>(m, rd);
}

template<FaceMeshConcept MeshType>
std::vector<typename MeshType::FaceType*>
fillAndShuffleFacePointerVector(MeshType& m)
{
	static std::random_device rd;
	return fillAndShuffleFacePointerVector(m, rd);
}

template<FaceMeshConcept MeshType>
std::vector<const typename MeshType::FaceType*>
fillAndShuffleFacePointerVector(const MeshType& m, std::random_device& rd)
{
	using FaceType = typename MeshType::FaceType;

	return internal::genericFASFPV<const MeshType&, const FaceType>(m, rd);
}

template<FaceMeshConcept MeshType>
std::vector<const typename MeshType::FaceType*>
fillAndShuffleFacePointerVector(const MeshType& m)
{
	static std::random_device rd;
	return fillAndShuffleFacePointerVector(m, rd);
}

template<FaceMeshConcept MeshType>
std::vector<uint>
fillAndShuffleFaceIndexVector(const MeshType& m, std::random_device& rd)
{
	using FaceType = typename MeshType::FaceType;

	std::vector<uint> vec;
	vec.reserve(m.faceNumber());

	for (const FaceType& f : m.faces()) {
		vec.push_back(m.index(f));
	}

	std::mt19937 g(rd());
	std::shuffle(vec.begin(),vec.end(), g);
	return vec;
}

template<FaceMeshConcept MeshType>
std::vector<uint>
fillAndShuffleFaceIndexVector(const MeshType& m)
{
	static std::random_device rd;
	return fillAndShuffleFaceIndexVector(m, rd);
}

} // namespace vcl
