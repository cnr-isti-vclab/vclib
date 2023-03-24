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

#include "bipartite_graph_iterators.h"

namespace vcl {

// AdjacentLeftNodeIterator

template<class T1, class T2>
BipartiteGraph<T1, T2>::AdjacentLeftNodeIterator::AdjacentLeftNodeIterator() : g(nullptr)
{
}

template<class T1, class T2>
const T2& BipartiteGraph<T1, T2>::AdjacentLeftNodeIterator::operator*() const
{
	return g->nodesR[*pos].getInfo();
}

template<class T1, class T2>
bool BipartiteGraph<T1, T2>::AdjacentLeftNodeIterator::operator==(
	const AdjacentLeftNodeIterator& otherIterator) const
{
	return (g == otherIterator.g && pos == otherIterator.pos);
}

template<class T1, class T2>
bool BipartiteGraph<T1, T2>::AdjacentLeftNodeIterator::operator!=(
	const AdjacentLeftNodeIterator& otherIterator) const
{
	return (g != otherIterator.g || pos != otherIterator.pos);
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::AdjacentLeftNodeIterator
BipartiteGraph<T1, T2>::AdjacentLeftNodeIterator::operator++()
{
	++pos;
	return *this;
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::AdjacentLeftNodeIterator
BipartiteGraph<T1, T2>::AdjacentLeftNodeIterator::operator++(int)
{
	AdjacentLeftNodeIterator oldIt = *this;
	++pos;
	return oldIt;
}

template<class T1, class T2>
BipartiteGraph<T1, T2>::AdjacentLeftNodeIterator::AdjacentLeftNodeIterator(
	std::unordered_set<unsigned int>::const_iterator it,
	const BipartiteGraph&                            g) :
		g(&g), pos(it)
{
}

// AdjacentRightNodeIterator

template<class T1, class T2>
BipartiteGraph<T1, T2>::AdjacentRightNodeIterator::AdjacentRightNodeIterator() : g(nullptr)
{
}

template<class T1, class T2>
const T1& BipartiteGraph<T1, T2>::AdjacentRightNodeIterator::operator*() const
{
	return g->nodesL[*pos].getInfo();
}

template<class T1, class T2>
bool BipartiteGraph<T1, T2>::AdjacentRightNodeIterator::operator==(
	const AdjacentRightNodeIterator& otherIterator) const
{
	return (g == otherIterator.g && pos == otherIterator.pos);
}

template<class T1, class T2>
bool BipartiteGraph<T1, T2>::AdjacentRightNodeIterator::operator!=(
	const AdjacentRightNodeIterator& otherIterator) const
{
	return (g != otherIterator.g || pos != otherIterator.pos);
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::AdjacentRightNodeIterator
BipartiteGraph<T1, T2>::AdjacentRightNodeIterator::operator++()
{
	++pos;
	return *this;
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::AdjacentRightNodeIterator
BipartiteGraph<T1, T2>::AdjacentRightNodeIterator::operator++(int)
{
	AdjacentRightNodeIterator oldIt = *this;
	++pos;
	return oldIt;
}

template<class T1, class T2>
BipartiteGraph<T1, T2>::AdjacentRightNodeIterator::AdjacentRightNodeIterator(
	std::unordered_set<unsigned int>::const_iterator it,
	const BipartiteGraph&                            g) :
		g(&g), pos(it)
{
}

// ConstAdjacentLeftNodeRangeBasedIterator

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::AdjacentLeftNodeIterator
BipartiteGraph<T1, T2>::AdjacentLeftNodeRangeBasedIterator::begin() const
{
	return g->adjacentLeftNodeBegin(uNode);
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::AdjacentLeftNodeIterator
BipartiteGraph<T1, T2>::AdjacentLeftNodeRangeBasedIterator::end() const
{
	return g->adjacentLeftNodeEnd(uNode);
}

template<class T1, class T2>
BipartiteGraph<T1, T2>::AdjacentLeftNodeRangeBasedIterator::AdjacentLeftNodeRangeBasedIterator(
	const BipartiteGraph* g,
	const T1&             uNode) :
		g(g), uNode(uNode)
{
}

// ConstAdjacentRightNodeRangeBasedIterator

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::AdjacentRightNodeIterator
BipartiteGraph<T1, T2>::AdjacentRightNodeRangeBasedIterator::begin() const
{
	return g->adjacentRightNodeBegin(vNode);
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::AdjacentRightNodeIterator
BipartiteGraph<T1, T2>::AdjacentRightNodeRangeBasedIterator::end() const
{
	return g->adjacentRightNodeEnd(vNode);
}

template<class T1, class T2>
BipartiteGraph<T1, T2>::AdjacentRightNodeRangeBasedIterator::AdjacentRightNodeRangeBasedIterator(
	const BipartiteGraph* g,
	const T2&             vNode) :
		g(g), vNode(vNode)
{
}

} // namespace vcl
