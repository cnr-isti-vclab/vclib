/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
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

#include "optional_adjacent_edges.h"

#include <algorithm>

namespace vcl::comp {

template<typename Edge, int N, typename T>
OptionalAdjacentEdges<Edge, N, T>::OptionalAdjacentEdges()
{
	if constexpr (N >= 0) {
		// I'll use the array, N is >= 0.
		// There will be a static number of references.
		if (B::contPtr)
			B::optCont().adjEdges(thisId()) = std::array<Edge*, N> {nullptr};
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of references.
		if (B::contPtr)
			B::optCont().adjEdges(thisId()) = std::vector<Edge*>();
	}
}

template<typename Edge, int N, typename T>
uint OptionalAdjacentEdges<Edge, N, T>::adjEdgesNumber() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return B::optCont().adjEdges(thisId()).size();
	}
}

template<typename Edge, int N, typename T>
Edge*& OptionalAdjacentEdges<Edge, N, T>::adjEdge(uint i)
{
	assert(i < adjEdgesNumber());
	return B::optCont().adjEdges(thisId())[i];
}

template<typename Edge, int N, typename T>
const Edge* OptionalAdjacentEdges<Edge, N, T>::adjEdge(uint i) const
{
	assert(i < adjEdgesNumber());
	return B::optCont().adjEdges(thisId())[i];
}

template<typename Edge, int N, typename T>
Edge*& OptionalAdjacentEdges<Edge, N, T>::adjEdgeMod(int i)
{
	int n = adjEdgesNumber();
	return B::optCont().adjEdges(thisId())[(i % n + n) % n];
}

template<typename Edge, int N, typename T>
const Edge* OptionalAdjacentEdges<Edge, N, T>::adjEdgeMod(int i) const
{
	int n = adjEdgesNumber();
	return B::optCont().adjEdges(thisId())[(i % n + n) % n];
}

template<typename Edge, int N, typename T>
void OptionalAdjacentEdges<Edge, N, T>::setAdjEdge(Edge* e, uint i)
{
	assert(i < adjEdgesNumber());
	B::optCont().adjEdges(thisId())[i] = e;
}

template<typename Edge, int N, typename T>
void OptionalAdjacentEdges<Edge, N, T>::setAdjEdges(const std::vector<Edge*>& list)
{
	if constexpr (N >= 0) {
		assert(list.size() == N);
		uint i = 0;
		for (const auto& e : list) {
			setEdge(e, i);
			++i;
		}
	}
	else {
		B::optCont().adjEdges(thisId()) = list;
	}
}

template<typename Edge, int N, typename T>
bool OptionalAdjacentEdges<Edge, N, T>::containsAdjEdge(const Edge* e) const
{
	return std::find(B::optCont().adjEdges.begin(), B::optCont().adjEdges.end(), e) !=
		   B::optCont().adjEdges.end();
}

template<typename Edge, int N, typename T>
typename OptionalAdjacentEdges<Edge, N, T>::AdjacentEdgeIterator
OptionalAdjacentEdges<Edge, N, T>::findAdjEdge(const Edge* e)
{
	return std::find(adjEdgeBegin(), adjEdgeEnd(), e);
}

template<typename Edge, int N, typename T>
typename OptionalAdjacentEdges<Edge, N, T>::ConstAdjacentEdgeIterator
OptionalAdjacentEdges<Edge, N, T>::findAdjEdge(const Edge* e) const
{
	return std::find(adjEdgeBegin(), adjEdgeEnd(), e);
}

template<typename Edge, int N, typename T>
int OptionalAdjacentEdges<Edge, N, T>::indexOfAdjEdge(const Edge* e) const
{
	auto it = findAdjEdge(e);
	if (it == adjEdgeEnd())
		return -1;
	else
		return it - adjEdgeBegin();
}

template<typename Edge, int N, typename T>
bool OptionalAdjacentEdges<Edge, N, T>::isAdjEdgesEnabled() const
{
	if (B::contPtr != nullptr)
		return B::optCont().isAdjacentEdgesEnabled();
	else
		return false;
}

template<typename Edge, int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void)
	OptionalAdjacentEdges<Edge, N, T>::resizeAdjEdges(uint n)
{
	B::optCont().adjEdges(thisId()).resize(n);
}

template<typename Edge, int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void) OptionalAdjacentEdges<Edge, N, T>::pushAdjEdge(Edge* e)
{
	B::optCont().adjEdges(thisId()).push_back(e);
}

template<typename Edge, int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void)
	OptionalAdjacentEdges<Edge, N, T>::insertAdjEdge(uint i, Edge* e)
{
	assert(i < adjEdgesNumber());
	B::optCont().adjEdges(thisId()).insert(B::optCont().adjEdges(thisId()).begin() + i, e);
}

template<typename Edge, int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void) OptionalAdjacentEdges<Edge, N, T>::eraseAdjEdge(uint i)
{
	assert(i < adjEdgesNumber());
	B::optCont().adjEdges(thisId()).erase(B::optCont().adjEdges(thisId()).begin() + i);
}

template<typename Edge, int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void) OptionalAdjacentEdges<Edge, N, T>::clearAdjEdges()
{
	B::optCont().adjEdges(thisId()).clear();
}

template<typename Edge, int N, typename T>
typename OptionalAdjacentEdges<Edge, N, T>::AdjacentEdgeIterator
OptionalAdjacentEdges<Edge, N, T>::adjEdgeBegin()
{
	return B::optCont().adjEdges(thisId()).begin();
}

template<typename Edge, int N, typename T>
typename OptionalAdjacentEdges<Edge, N, T>::AdjacentEdgeIterator
OptionalAdjacentEdges<Edge, N, T>::adjEdgeEnd()
{
	return B::optCont().adjEdges(thisId()).end();
}

template<typename Edge, int N, typename T>
typename OptionalAdjacentEdges<Edge, N, T>::ConstAdjacentEdgeIterator
OptionalAdjacentEdges<Edge, N, T>::adjEdgeBegin() const
{
	return B::optCont().adjEdges(thisId()).begin();
}

template<typename Edge, int N, typename T>
typename OptionalAdjacentEdges<Edge, N, T>::ConstAdjacentEdgeIterator
OptionalAdjacentEdges<Edge, N, T>::adjEdgeEnd() const
{
	return B::optCont().adjEdges(thisId()).end();
}

template<typename Edge, int N, typename T>
typename OptionalAdjacentEdges<Edge, N, T>::AdjacentEdgeRangeIterator
OptionalAdjacentEdges<Edge, N, T>::adjEdges()
{
	return AdjacentEdgeRangeIterator(
		*this,
		&OptionalAdjacentEdges::adjEdgeBegin,
		&OptionalAdjacentEdges::adjEdgeEnd);
}

template<typename Edge, int N, typename T>
typename OptionalAdjacentEdges<Edge, N, T>::ConstAdjacentEdgeRangeIterator
OptionalAdjacentEdges<Edge, N, T>::adjEdges() const
{
	return ConstAdjacentEdgeRangeIterator(
		*this,
		&OptionalAdjacentEdges::adjEdgeBegin,
		&OptionalAdjacentEdges::adjEdgeEnd);
}

template<typename Edge, int N, typename T>
void OptionalAdjacentEdges<Edge, N, T>::updateEdgeReferences(
	const Edge* oldBase,
	const Edge* newBase)
{
	for (uint j = 0; j < adjEdgesNumber(); ++j) {
		if (adjEdge(j) != nullptr) {
			size_t diff = adjEdge(j) - oldBase;
			adjEdge(j)  = (Edge*) newBase + diff;
		}
	}
}

template<typename Edge, int N, typename T>
void OptionalAdjacentEdges<Edge, N, T>::updateEdgeReferencesAfterCompact(
	const Edge*             base,
	const std::vector<int>& newIndices)
{
	for (uint j = 0; j < adjEdgesNumber(); ++j) {
		if (adjEdge(j) != nullptr) {
			size_t diff = adjEdge(j) - base;
			if (newIndices[diff] < 0) { // element has been removed
				adjEdge(j) = nullptr;
			}
			else { // the new pointer will be base + newIndices[diff]
				adjEdge(j) = (Edge*) base + newIndices[diff];
			}
		}
	}
}

template<typename Edge, int N, typename T>
template<typename Element>
void OptionalAdjacentEdges<Edge, N, T>::importFrom(const Element&)
{
}

template<typename Edge, int N, typename T>
template<typename Element, typename ElEType>
void OptionalAdjacentEdges<Edge, N, T>::importEdgeReferencesFrom(
	const Element& e,
	Edge* base,
	const ElEType* ebase)
{
	if constexpr (HasAdjacentEdges<Element>) {
		if (isAdjEdgesEnabled() && isAdjacentEdgesEnabledOn(e)){
			if constexpr(N > 0) {
				// same static size
				if constexpr (N == Element::ADJ_EDGE_NUMBER) {
					importReferencesFrom(e, base, ebase);
				}
				// from dynamic to static, but dynamic size == static size
				else if constexpr (Element::ADJ_EDGE_NUMBER < 0){
					if (e.adjEdgesNumber() == N){
						importReferencesFrom(e, base, ebase);
					}
				}
				else {
					// do not import in this case: cannot import from dynamic size != static size
				}
			}
			else {
				// from static/dynamic to dynamic size: need to resize first, then import
				resizeAdjEdges(e.adjEdgesNumber());
				importReferencesFrom(e, base, ebase);
			}
		}
	}
}

template<typename Edge, int N, typename T>
template<typename Element, typename ElEType>
void OptionalAdjacentEdges<Edge, N, T>::importReferencesFrom(
	const Element& e,
	Edge* base,
	const ElEType* ebase)
{
	if (ebase != nullptr && base != nullptr) {
		for (uint i = 0; i < e.adjEdgesNumber(); ++i){
			if (e.adjEdge(i) != nullptr){
				adjEdge(i) = base + (e.adjEdge(i) - ebase);
			}
		}
	}
}

} // namespace vcl::comp

