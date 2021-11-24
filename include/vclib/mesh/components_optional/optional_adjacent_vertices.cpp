/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "optional_adjacent_vertices.h"

#include <algorithm>

namespace vcl::comp {

template<typename Vertex, int N, typename T>
OptionalAdjacentVertices<Vertex, N, T>::OptionalAdjacentVertices()
{
	if constexpr (N >= 0) {
		// I'll use the array, N is >= 0.
		// There will be a static number of references.
		if (B::contPtr)
			B::optCont().adjVerts(thisId()) = std::array<Vertex*, N> {nullptr};
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of references.
		if (B::contPtr)
			B::optCont().adjVerts(thisId()) = std::vector<Vertex*>();
	}
}

template<typename Vertex, int N, typename T>
uint OptionalAdjacentVertices<Vertex, N, T>::adjVerticesNumber() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return B::optCont().adjVerts(thisId()).size();
	}
}

template<typename Vertex, int N, typename T>
Vertex*& OptionalAdjacentVertices<Vertex, N, T>::adjVertex(uint i)
{
	assert(i < adjVerticesNumber());
	return B::optCont().adjVerts(thisId())[i];
}

template<typename Vertex, int N, typename T>
const Vertex* OptionalAdjacentVertices<Vertex, N, T>::adjVertex(uint i) const
{
	assert(i < adjVerticesNumber());
	return B::optCont().adjVerts(thisId())[i];
}

template<typename Vertex, int N, typename T>
Vertex*& OptionalAdjacentVertices<Vertex, N, T>::adjVertexMod(int i)
{
	uint n = adjVerticesNumber();
	return B::optCont().adjVerts(thisId())[(i % n + n) % n];
}

template<typename Vertex, int N, typename T>
const Vertex* OptionalAdjacentVertices<Vertex, N, T>::adjVertexMod(int i) const
{
	uint n = adjVerticesNumber();
	return B::optCont().adjVerts(thisId())[(i % n + n) % n];
}

template<typename Vertex, int N, typename T>
void OptionalAdjacentVertices<Vertex, N, T>::setAdjVertex(Vertex* f, uint i)
{
	assert(i < adjVerticesNumber());
	B::optCont().adjVerts(thisId())[i] = f;
}

template<typename Vertex, int N, typename T>
void OptionalAdjacentVertices<Vertex, N, T>::setAdjVertices(const std::vector<Vertex*>& list)
{
	if constexpr (N >= 0) {
		assert(list.size() == N);
		uint i = 0;
		for (const auto& f : list) {
			setVertex(f, i);
			++i;
		}
	}
	else {
		B::optCont().adjVerts(thisId()) = list;
	}
}

template<typename Vertex, int N, typename T>
bool OptionalAdjacentVertices<Vertex, N, T>::containsAdjVertex(const Vertex* v) const
{
	return std::find(B::optCont().adjVerts.begin(), B::optCont().adjVerts.end(), v) !=
		   B::optCont().adjVerts.end();
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::AdjacentVertexIterator
OptionalAdjacentVertices<Vertex, N, T>::findAdjVertex(const Vertex* v)
{
	return std::find(adjVertexBegin(), adjVertexEnd(), v);
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::ConstAdjacentVertexIterator
OptionalAdjacentVertices<Vertex, N, T>::findAdjVertex(const Vertex* v) const
{
	return std::find(adjVertexBegin(), adjVertexEnd(), v);
}

template<typename Vertex, int N, typename T>
int OptionalAdjacentVertices<Vertex, N, T>::indexOfAdjVertex(const Vertex* v) const
{
	auto it = findAdjVertex(v);
	if (v == adjVertexEnd())
		return -1;
	else
		return it - adjVertexBegin();
}

template<typename Vertex, int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void)
OptionalAdjacentVertices<Vertex, N, T>::resizeAdjVertices(uint n)
{
	B::optCont().adjVerts(thisId()).resize(n);
}

template<typename Vertex, int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void) OptionalAdjacentVertices<Vertex, N, T>::pushAdjVertex(Vertex* f)
{
	B::optCont().adjVerts(thisId()).push_back(f);
}

template<typename Vertex, int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void)
OptionalAdjacentVertices<Vertex, N, T>::insertAdjVertex(uint i, Vertex* f)
{
	assert(i < adjVerticesNumber());
	B::optCont().adjVerts(thisId()).insert(B::optCont().adjVerts(thisId()).begin() + i, f);
}

template<typename Vertex, int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void)
OptionalAdjacentVertices<Vertex, N, T>::eraseAdjVertex(uint i)
{
	assert(i < adjVerticesNumber());
	B::optCont().adjVerts(thisId()).erase(B::optCont().adjVerts(thisId()).begin() + i);
}

template<typename Vertex, int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void) OptionalAdjacentVertices<Vertex, N, T>::clearAdjVertices()
{
	B::optCont().adjVerts(thisId()).clear();
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::AdjacentVertexIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertexBegin()
{
	return B::optCont().adjVerts(thisId()).begin();
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::AdjacentVertexIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertexEnd()
{
	return B::optCont().adjVerts(thisId()).end();
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::ConstAdjacentVertexIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertexBegin() const
{
	return B::optCont().adjVerts(thisId()).begin();
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::ConstAdjacentVertexIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertexEnd() const
{
	return B::optCont().adjVerts(thisId()).end();
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::AdjacentVertexRangeIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertices()
{
	return AdjacentVertexRangeIterator(
		*this,
		&OptionalAdjacentVertices::adjVertexBegin,
		&OptionalAdjacentVertices::adjVertexEnd);
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::ConstAdjacentVertexRangeIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertices() const
{
	return ConstAdjacentVertexRangeIterator(
		*this,
		&OptionalAdjacentVertices::adjVertexBegin,
		&OptionalAdjacentVertices::adjVertexEnd);
}

template<typename Vertex, int N, typename T>
void OptionalAdjacentVertices<Vertex, N, T>::updateVertexReferences(
	const Vertex* oldBase,
	const Vertex* newBase)
{
	for (uint j = 0; j < adjVerticesNumber(); ++j) {
		if (adjVertex(j) != nullptr) {
			size_t diff = adjVertex(j) - oldBase;
			adjVertex(j)  = (Vertex*) newBase + diff;
		}
	}
}

template<typename Vertex, int N, typename T>
void OptionalAdjacentVertices<Vertex, N, T>::updateVertexReferencesAfterCompact(
	const Vertex*           base,
	const std::vector<int>& newIndices)
{
	for (uint j = 0; j < adjVerticesNumber(); ++j) {
		if (adjVertex(j) != nullptr) {
			size_t diff = adjVertex(j) - base;
			if (newIndices[diff] < 0) { // element has been removed
				adjVertex(j) = nullptr;
			}
			else { // the new pointer will be base + newIndices[diff]
				adjVertex(j) = (Vertex*) base + newIndices[diff];
			}
		}
	}
}

} // namespace vcl::comp
