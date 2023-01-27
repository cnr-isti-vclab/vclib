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

#include "optional_adjacent_vertices.h"

#include <algorithm>

namespace vcl::comp {

template<typename Vertex, typename T>
OptionalAdjacentVertices<Vertex, T>::OptionalAdjacentVertices()
{
	// I'll use the vector, because N is < 0.
	// There will be a dynamic number of references.
	if (B::contPtr)
		B::optCont().adjVerts(thisId()) = std::vector<Vertex*>();
}

template<typename Vertex, typename T>
uint OptionalAdjacentVertices<Vertex,T>::adjVerticesNumber() const
{
	return B::optCont().adjVerts(thisId()).size();
}

template<typename Vertex, typename T>
Vertex*& OptionalAdjacentVertices<Vertex, T>::adjVertex(uint i)
{
	assert(i < adjVerticesNumber());
	return B::optCont().adjVerts(thisId())[i];
}

template<typename Vertex, typename T>
const Vertex* OptionalAdjacentVertices<Vertex, T>::adjVertex(uint i) const
{
	assert(i < adjVerticesNumber());
	return B::optCont().adjVerts(thisId())[i];
}

template<typename Vertex, typename T>
Vertex*& OptionalAdjacentVertices<Vertex, T>::adjVertexMod(int i)
{
	int n = adjVerticesNumber();
	return B::optCont().adjVerts(thisId())[(i % n + n) % n];
}

template<typename Vertex, typename T>
const Vertex* OptionalAdjacentVertices<Vertex, T>::adjVertexMod(int i) const
{
	int n = adjVerticesNumber();
	return B::optCont().adjVerts(thisId())[(i % n + n) % n];
}

template<typename Vertex, typename T>
void OptionalAdjacentVertices<Vertex, T>::setAdjVertex(Vertex* f, uint i)
{
	assert(i < adjVerticesNumber());
	B::optCont().adjVerts(thisId())[i] = f;
}

template<typename Vertex, typename T>
void OptionalAdjacentVertices<Vertex, T>::setAdjVertices(const std::vector<Vertex*>& list)
{

	B::optCont().adjVerts(thisId()) = list;
}

template<typename Vertex, typename T>
bool OptionalAdjacentVertices<Vertex, T>::containsAdjVertex(const Vertex* v) const
{
	return std::find(B::optCont().adjVerts.begin(), B::optCont().adjVerts.end(), v) !=
		   B::optCont().adjVerts.end();
}

template<typename Vertex, typename T>
typename OptionalAdjacentVertices<Vertex, T>::AdjacentVertexIterator
OptionalAdjacentVertices<Vertex, T>::findAdjVertex(const Vertex* v)
{
	return std::find(adjVertexBegin(), adjVertexEnd(), v);
}

template<typename Vertex, typename T>
typename OptionalAdjacentVertices<Vertex, T>::ConstAdjacentVertexIterator
OptionalAdjacentVertices<Vertex, T>::findAdjVertex(const Vertex* v) const
{
	return std::find(adjVertexBegin(), adjVertexEnd(), v);
}

template<typename Vertex, typename T>
int OptionalAdjacentVertices<Vertex, T>::indexOfAdjVertex(const Vertex* v) const
{
	auto it = findAdjVertex(v);
	if (it == adjVertexEnd())
		return -1;
	else
		return it - adjVertexBegin();
}

template<typename Vertex, typename T>
bool OptionalAdjacentVertices<Vertex, T>::isAdjVerticesEnabled() const
{
	if (B::contPtr != nullptr)
		return B::optCont().isAdjacentVerticesEnabled();
	else
		return false;
}

template<typename Vertex, typename T>
void OptionalAdjacentVertices<Vertex, T>::resizeAdjVertices(uint n)
{
	B::optCont().adjVerts(thisId()).resize(n);
}

template<typename Vertex, typename T>
void OptionalAdjacentVertices<Vertex, T>::pushAdjVertex(Vertex* f)
{
	B::optCont().adjVerts(thisId()).push_back(f);
}

template<typename Vertex, typename T>
void OptionalAdjacentVertices<Vertex, T>::insertAdjVertex(uint i, Vertex* f)
{
	assert(i < adjVerticesNumber() + 1);
	B::optCont().adjVerts(thisId()).insert(B::optCont().adjVerts(thisId()).begin() + i, f);
}

template<typename Vertex, typename T>
void OptionalAdjacentVertices<Vertex, T>::eraseAdjVertex(uint i)
{
	assert(i < adjVerticesNumber());
	B::optCont().adjVerts(thisId()).erase(B::optCont().adjVerts(thisId()).begin() + i);
}

template<typename Vertex, typename T>
void OptionalAdjacentVertices<Vertex, T>::clearAdjVertices()
{
	B::optCont().adjVerts(thisId()).clear();
}

template<typename Vertex, typename T>
typename OptionalAdjacentVertices<Vertex, T>::AdjacentVertexIterator
OptionalAdjacentVertices<Vertex, T>::adjVertexBegin()
{
	return B::optCont().adjVerts(thisId()).begin();
}

template<typename Vertex, typename T>
typename OptionalAdjacentVertices<Vertex, T>::AdjacentVertexIterator
OptionalAdjacentVertices<Vertex, T>::adjVertexEnd()
{
	return B::optCont().adjVerts(thisId()).end();
}

template<typename Vertex, typename T>
typename OptionalAdjacentVertices<Vertex, T>::ConstAdjacentVertexIterator
OptionalAdjacentVertices<Vertex, T>::adjVertexBegin() const
{
	return B::optCont().adjVerts(thisId()).begin();
}

template<typename Vertex, typename T>
typename OptionalAdjacentVertices<Vertex, T>::ConstAdjacentVertexIterator
OptionalAdjacentVertices<Vertex, T>::adjVertexEnd() const
{
	return B::optCont().adjVerts(thisId()).end();
}

template<typename Vertex, typename T>
typename OptionalAdjacentVertices<Vertex, T>::AdjacentVertexRangeIterator
OptionalAdjacentVertices<Vertex, T>::adjVertices()
{
	return AdjacentVertexRangeIterator(
		*this,
		&OptionalAdjacentVertices::adjVertexBegin,
		&OptionalAdjacentVertices::adjVertexEnd);
}

template<typename Vertex, typename T>
typename OptionalAdjacentVertices<Vertex, T>::ConstAdjacentVertexRangeIterator
OptionalAdjacentVertices<Vertex, T>::adjVertices() const
{
	return ConstAdjacentVertexRangeIterator(
		*this,
		&OptionalAdjacentVertices::adjVertexBegin,
		&OptionalAdjacentVertices::adjVertexEnd);
}

template<typename Vertex, typename T>
void OptionalAdjacentVertices<Vertex, T>::updateVertexReferences(
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

template<typename Vertex, typename T>
void OptionalAdjacentVertices<Vertex, T>::updateVertexReferencesAfterCompact(
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

template<typename Vertex, typename T>
template<typename Element>
void OptionalAdjacentVertices<Vertex, T>::importFrom(const Element&)
{
}

template<typename Vertex, typename T>
template<typename Element, typename ElVType>
void OptionalAdjacentVertices<Vertex, T>::importVertexReferencesFrom(
	const Element& e,
	Vertex* base,
	const ElVType* ebase)
{
	if constexpr (HasAdjacentVertices<Element>) {
		if (isAdjVerticesEnabled() && isAdjacentVerticesEnabledOn(e)){
			// from static/dynamic to dynamic size: need to resize first, then import
			resizeAdjVertices(e.adjVerticesNumber());
			importReferencesFrom(e, base, ebase);
		}
	}
}

template<typename Vertex, typename T>
template<typename Element, typename ElVType>
void OptionalAdjacentVertices<Vertex, T>::importReferencesFrom(
	const Element& e,
	Vertex* base,
	const ElVType* ebase)
{
	if (ebase != nullptr && base != nullptr) {
		for (uint i = 0; i < e.adjVerticesNumber(); ++i){
			if (e.adjVertex(i) != nullptr){
				adjVertex(i) = base + (e.adjVertex(i) - ebase);
			}
		}
	}
}

} // namespace vcl::comp
