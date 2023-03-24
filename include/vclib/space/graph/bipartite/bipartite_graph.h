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

#ifndef VCL_SPACE_GRAPH_BIPARTITE_BIPARTITE_GRAPH_H
#define VCL_SPACE_GRAPH_BIPARTITE_BIPARTITE_GRAPH_H

#include <assert.h>
#include <map>
#include <set>
#include <vector>

#include <vclib/iterator/range.h>
#include <vclib/space/graph/undirected_node.h>

#include "iterator/node_iterator.h"

namespace vcl {

template<class T1, class T2>
class BipartiteGraph
{
public:
	using LeftNodeIterator = NodeIterator<typename std::vector<UndirectedNode<T1>>::const_iterator>;
	using RightNodeIterator =
		NodeIterator<typename std::vector<UndirectedNode<T2>>::const_iterator>;

	using LeftNodeRange  = vcl::Range<LeftNodeIterator>;
	using RightNodeRange = vcl::Range<RightNodeIterator>;

	class AdjacentLeftNodeIterator;
	class AdjacentRightNodeIterator;

	class AdjacentLeftNodeRangeBasedIterator;
	class AdjacentRightNodeRangeBasedIterator;

	BipartiteGraph();
	bool         addLeftNode(const T1& info);
	bool         addRightNode(const T2& info);
	bool         existsLeftNode(const T1& lNode) const;
	bool         existsRightNode(const T2& rNode) const;
	unsigned int sizeLeftNodes() const;
	unsigned int sizeRightNodes() const;
	unsigned int sizeAdjacencesLeftNode(const T1& lNode) const;
	unsigned int sizeAdjacencesRightNode(const T2& rNode) const;
	bool         deleteLeftNode(const T1& lNode);
	bool         deleteRightNode(const T2& rNode);
	bool         addArc(const T1& lNode, const T2& rNode);
	bool         deleteArc(const T1& lNode, const T2& rNode);
	bool         clearAdjacencesLeftNode(const T1& lNode);
	bool         clearAdjacencesRightNode(const T2& rNode);
	bool         modifyLeftNode(const T1& old, const T1& newInfo);
	bool         modifyRightNode(const T2& old, const T2& newInfo);

	AdjacentLeftNodeIterator adjacentLeftNodeBegin(const T1& lNode) const;
	AdjacentLeftNodeIterator adjacentLeftNodeEnd(const T1& lNode) const;

	AdjacentRightNodeIterator adjacentRightNodeBegin(const T2& rNode) const;
	AdjacentRightNodeIterator adjacentRightNodeEnd(const T2& rNode) const;

	LeftNodeIterator leftNodeBegin() const;
	LeftNodeIterator leftNodeEnd() const;

	RightNodeIterator rightNodeBegin() const;
	RightNodeIterator rightNodeEnd() const;

	LeftNodeRange leftNodes() const;
	RightNodeRange rightNodes() const;

	AdjacentLeftNodeRangeBasedIterator  adjacentLeftNodeIterator(const T1& lNode) const;
	AdjacentRightNodeRangeBasedIterator adjacentRightNodeIterator(const T2& rNode) const;

protected:
	int getIdLeftNode(const T1& uNode) const;
	int getIdRightNode(const T2& vNode) const;

	std::map<T1, unsigned int> mapL;
	std::map<T2, unsigned int> mapR;

	std::vector<UndirectedNode<T1>> nodesL;
	std::vector<UndirectedNode<T2>> nodesR;

	std::set<unsigned int> unusedLNodes;
	std::set<unsigned int> unusedRNodes;
};

} // namespace vcl

#include "iterator/bipartite_graph_iterators.h"

#include "bipartite_graph.cpp"

#endif // VCL_SPACE_GRAPH_BIPARTITE_BIPARTITE_GRAPH_H
