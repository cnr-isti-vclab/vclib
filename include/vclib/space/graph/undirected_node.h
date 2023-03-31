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

#ifndef VCL_SPACE_GRAPH_UNDIRECTED_NODE_H
#define VCL_SPACE_GRAPH_UNDIRECTED_NODE_H

#include <unordered_set>

namespace vcl {

template<class T>
class UndirectedNode
{
public:
	using InfoType = T;

	UndirectedNode();
	UndirectedNode(const T& info);

	const T& info() const;
	T&       info();

	void addAdjacent(unsigned int node);
	bool isAdjacent(unsigned int node) const;
	void deleteAdjacent(unsigned int node);
	void clearAdjacentNodes();

	std::unordered_set<unsigned int>::const_iterator begin() const;
	std::unordered_set<unsigned int>::const_iterator end() const;

	unsigned int sizeAdjacentNodes() const;

protected:
	T                                inf;
	std::unordered_set<unsigned int> adjacentNodes;
};

} // namespace vcl

#include "undirected_node.cpp"

#endif // VCL_SPACE_GRAPH_UNDIRECTED_NODE_H
