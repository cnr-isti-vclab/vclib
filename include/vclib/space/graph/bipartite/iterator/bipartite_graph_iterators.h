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

#ifndef VCL_SPACE_GRAPH_BIPARTITE_ITERATOR_BIPARTITE_GRAPH_ITERATORS_H
#define VCL_SPACE_GRAPH_BIPARTITE_ITERATOR_BIPARTITE_GRAPH_ITERATORS_H

#include "../bipartite_graph.h"

namespace vcl {

template<class T1, class T2>
class BipartiteGraph<T1, T2>::AdjacentLeftNodeIterator
{
	friend class BipartiteGraph;

public:
	AdjacentLeftNodeIterator();

	const T2& operator*() const;
	bool      operator==(const AdjacentLeftNodeIterator& otherIterator) const;
	bool      operator!=(const AdjacentLeftNodeIterator& otherIterator) const;

	AdjacentLeftNodeIterator operator++();
	AdjacentLeftNodeIterator operator++(int);

protected:
	const BipartiteGraph*                            g;
	std::unordered_set<unsigned int>::const_iterator pos;
	AdjacentLeftNodeIterator(
		std::unordered_set<unsigned int>::const_iterator it,
		const BipartiteGraph&                            g);
};

template<class T1, class T2>
class BipartiteGraph<T1, T2>::AdjacentRightNodeIterator
{
	friend class BipartiteGraph;

public:
	AdjacentRightNodeIterator();

	const T1& operator*() const;
	bool      operator==(const AdjacentRightNodeIterator& otherIterator) const;
	bool      operator!=(const AdjacentRightNodeIterator& otherIterator) const;

	AdjacentRightNodeIterator operator++();
	AdjacentRightNodeIterator operator++(int);

protected:
	const BipartiteGraph*                            g;
	std::unordered_set<unsigned int>::const_iterator pos;
	AdjacentRightNodeIterator(
		std::unordered_set<unsigned int>::const_iterator it,
		const BipartiteGraph&                            g);
};

template<class T1, class T2>
class BipartiteGraph<T1, T2>::AdjacentLeftNodeRangeBasedIterator
{
	friend class BipartiteGraph;

public:
	AdjacentLeftNodeIterator begin() const;
	AdjacentLeftNodeIterator end() const;

protected:
	const BipartiteGraph* g;
	T1                    uNode;
	AdjacentLeftNodeRangeBasedIterator(const BipartiteGraph* g, const T1& uNode);
};

template<class T1, class T2>
class BipartiteGraph<T1, T2>::AdjacentRightNodeRangeBasedIterator
{
	friend class BipartiteGraph;

public:
	AdjacentRightNodeIterator begin() const;
	AdjacentRightNodeIterator end() const;

protected:
	const BipartiteGraph* g;
	T2                    vNode;
	AdjacentRightNodeRangeBasedIterator(const BipartiteGraph* g, const T2& vNode);
};

} // namespace vcl

#include "bipartite_graph_iterators.cpp"

#endif // VCL_SPACE_GRAPH_BIPARTITE_ITERATOR_BIPARTITE_GRAPH_ITERATORS_H
