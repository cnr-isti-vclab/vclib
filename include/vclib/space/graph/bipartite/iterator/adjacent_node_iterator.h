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

#ifndef VCL_SPACE_GRAPH_BIPARTITE_ITERATOR_ADJACENT_NODE_ITERATOR_H
#define VCL_SPACE_GRAPH_BIPARTITE_ITERATOR_ADJACENT_NODE_ITERATOR_H

namespace vcl::internal {

template<typename Graph, typename Iterator>
class AdjacentNodeIterator
{
public:
	AdjacentNodeIterator();
	AdjacentNodeIterator(const Graph& g, Iterator it);

	bool operator==(const AdjacentNodeIterator& otherIterator) const;
	bool operator!=(const AdjacentNodeIterator& otherIterator) const;

	AdjacentNodeIterator operator++();
	AdjacentNodeIterator operator++(int);

protected:
	const Graph* g = nullptr;
	Iterator it;
};

} // namespace vcl::internal

#include "adjacent_node_iterator.cpp"

#endif // VCL_SPACE_GRAPH_BIPARTITE_ITERATOR_ADJACENT_NODE_ITERATOR_H
