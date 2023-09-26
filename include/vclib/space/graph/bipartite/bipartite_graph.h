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

#include <vclib/views/view.h>
#include <vclib/types.h>
#include <vclib/space/graph/undirected_node.h>

#include "iterator/node_iterator.h"
#include "iterator/adjacent_left_node_iterator.h"
#include "iterator/adjacent_right_node_iterator.h"

namespace vcl {

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

template<class T1, class T2>
class BipartiteGraph
{
public:
	using LeftType  = T1;
	using RightType = T2;
	using LeftNodeType  = UndirectedNode<T1>;
	using RightNodeType = UndirectedNode<T2>;

	using LeftNodeIterator = NodeIterator<typename std::vector<UndirectedNode<T1>>::const_iterator>;
	using RightNodeIterator =
		NodeIterator<typename std::vector<UndirectedNode<T2>>::const_iterator>;
	
	using LeftNodeView  = vcl::View<LeftNodeIterator>;
	using RightNodeView = vcl::View<RightNodeIterator>;

	using AdjacentLeftNodeIterator = internal::AdjacentLeftNodeIterator<
		BipartiteGraph<T1, T2>,
		std::unordered_set<unsigned int>::const_iterator>;
	using AdjacentRightNodeIterator = internal::AdjacentRightNodeIterator<
		BipartiteGraph<T1, T2>,
		std::unordered_set<unsigned int>::const_iterator>;
	
	using AdjacentLeftNodeView  = vcl::View<AdjacentLeftNodeIterator>;
	using AdjacentRightNodeView = vcl::View<AdjacentRightNodeIterator>;

	BipartiteGraph();

	bool leftNodeExists(const T1& lNode) const;
	bool rightNodeExists(const T2& rNode) const;

	uint leftNodesNumber() const;
	uint rightNodesNumber() const;
	uint adjacentLeftNodeNumber(const T1& lNode) const;
	uint adjacentRightNodeNumber(const T2& rNode) const;

	bool addLeftNode(const T1& info);
	bool addRightNode(const T2& info);
	bool deleteLeftNode(const T1& lNode);
	bool deleteRightNode(const T2& rNode);
	bool addArc(const T1& lNode, const T2& rNode);
	bool deleteArc(const T1& lNode, const T2& rNode);
	bool clearAdjacencesLeftNode(const T1& lNode);
	bool clearAdjacencesRightNode(const T2& rNode);
	bool setLeftNode(const T1& old, const T1& newInfo);
	bool setRightNode(const T2& old, const T2& newInfo);

	AdjacentLeftNodeIterator adjacentLeftNodeBegin(const T1& lNode) const;
	AdjacentLeftNodeIterator adjacentLeftNodeEnd(const T1& lNode) const;

	AdjacentRightNodeIterator adjacentRightNodeBegin(const T2& rNode) const;
	AdjacentRightNodeIterator adjacentRightNodeEnd(const T2& rNode) const;

	LeftNodeIterator leftNodeBegin() const;
	LeftNodeIterator leftNodeEnd() const;

	RightNodeIterator rightNodeBegin() const;
	RightNodeIterator rightNodeEnd() const;

	LeftNodeView leftNodes() const;
	RightNodeView rightNodes() const;

	AdjacentLeftNodeView  adjacentLeftNodes(const T1& lNode) const;
	AdjacentRightNodeView adjacentRightNodes(const T2& rNode) const;

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

/******************************************************************************
 *                                Definitions                                 *
 ******************************************************************************/

/**
 * @brief Default constructor. It creates an empty Bipartite Graph.
 */
template<class T1, class T2>
BipartiteGraph<T1, T2>::BipartiteGraph()
{
}

/**
 * @brief Checks if a node exists on the left side of the graph
 * @param lNode
 * @return true if the node exists
 */
template<class T1, class T2>
bool BipartiteGraph<T1, T2>::leftNodeExists(const T1& lNode) const
{
	return mapL.find(lNode) != mapL.end();
}

/**
 * @brief Checks if a node exists on the right side of the graph
 * @param rNode
 * @return true if the node exists
 */
template<class T1, class T2>
bool BipartiteGraph<T1, T2>::rightNodeExists(const T2& rNode) const
{
	return mapR.find(rNode) != mapR.end();
}

/**
 * @brief Returns the number of left nodes of the graph.
 * @return the number of nodes on the left side of the graph
 */
template<class T1, class T2>
unsigned int BipartiteGraph<T1, T2>::leftNodesNumber() const
{
	return (unsigned int) (nodesL.size() - unusedLNodes.size());
}

/**
 * @brief Returns the number of right nodes of the graph.
 * @return the number of nodes on the right side of the graph
 */
template<class T1, class T2>
unsigned int BipartiteGraph<T1, T2>::rightNodesNumber() const
{
	return (unsigned int) (nodesR.size() - unusedRNodes.size());
}

/**
 * @brief Returns the number of adjacent nodes to `lNode`.
 * @param lNode
 * @return the number of adjacent nodes to lNode
 */
template<class T1, class T2>
unsigned int BipartiteGraph<T1, T2>::adjacentLeftNodeNumber(const T1& lNode) const
{
	int uid = getIdLeftNode(lNode);
	return nodesL[uid].sizeAdjacentNodes();
}

/**
 * @brief Returns the number of adjacent nodes to `rNode`.
 * @param rNode
 * @return the number of adjacent nodes to rNode
 */
template<class T1, class T2>
unsigned int BipartiteGraph<T1, T2>::adjacentRightNodeNumber(const T2& rNode) const
{
	int vid = getIdRightNode(rNode);
	return nodesR[vid].sizeAdjacentNodes();
}

/**
 * @brief Removes lNode and all its arcs from the graph
 * @param lNode
 * @return true if the node is successfully deleted
 */
template<class T1, class T2>
bool BipartiteGraph<T1, T2>::deleteLeftNode(const T1& lNode)
{
	if (clearAdjacencesLeftNode(lNode)) {
		unusedLNodes.insert(mapL[lNode]);
		mapL.erase(lNode);
		return true;
	}
	else
		return false;
}

/**
 * @brief Removes rNode and all its arcs from the graph
 * @param rNode
 * @return true if the node is successfully deleted
 */
template<class T1, class T2>
bool BipartiteGraph<T1, T2>::deleteRightNode(const T2& rNode)
{
	if (clearAdjacencesRightNode(rNode)) {
		unusedRNodes.insert(mapR[rNode]);
		mapR.erase(rNode);
		return true;
	}
	else
		return false;
}

/**
 * @brief Creates an arc between lNode and rNode
 * @param lNode
 * @param rNode
 * @return true if the arc is successfully created (both nodes exists in the graph)
 */
template<class T1, class T2>
bool BipartiteGraph<T1, T2>::addArc(const T1& lNode, const T2& rNode)
{
	try {
		int uid = getIdLeftNode(lNode);
		int vid = getIdRightNode(rNode);
		assert((unsigned int) uid < nodesL.size());
		assert((unsigned int) vid < nodesR.size());
		nodesL[uid].addAdjacent(vid);
		nodesR[vid].addAdjacent(uid);
		return true;
	}
	catch (...) {
		return false;
	}
}

/**
 * @brief Adds a new node on the left side of the graph.
 * @param[in] info: the value associated to the new node
 * @return true if the node is correctly added, false otherwise (if the node already exists)
 */
template<class T1, class T2>
bool BipartiteGraph<T1, T2>::addLeftNode(const T1& info)
{
	if (mapL.find(info) == mapL.end()) {
		if (unusedLNodes.size() == 0) {
			mapL[info] = (unsigned int) nodesL.size();
			nodesL.emplace_back(info);
		}
		else {
			unsigned int id = *(unusedLNodes.begin());
			unusedLNodes.erase(unusedLNodes.begin());
			mapL[info] = id;
			nodesL[id] = UndirectedNode<T1>(info);
		}
		return true;
	}
	else
		return false;
}

/**
 * @brief Adds a new node on the right side of the graph.
 * @param[in] info: the value associated to the new node
 * @return true if the node is correctly added, false otherwise (if the node already exists)
 */
template<class T1, class T2>
bool BipartiteGraph<T1, T2>::addRightNode(const T2& info)
{
	if (mapR.find(info) == mapR.end()) {
		if (unusedRNodes.size() == 0) {
			mapR[info] = (unsigned int) nodesR.size();
			nodesR.emplace_back(info);
		}
		else {
			unsigned int id = *(unusedRNodes.begin());
			unusedRNodes.erase(unusedRNodes.begin());
			mapR[info] = id;
			nodesR[id] = UndirectedNode<T2>(info);
		}
		return true;
	}
	else
		return false;
}

/**
 * @brief Removes the arc between lNode and rNode
 * @param lNode
 * @param rNode
 * @return true if the arc is successfully deleted (both nodes and the arc exists in the graph)
 */
template<class T1, class T2>
bool BipartiteGraph<T1, T2>::deleteArc(const T1& lNode, const T2& rNode)
{
	try {
		int uid = getIdLeftNode(lNode);
		int vid = getIdRightNode(rNode);
		assert((unsigned int) uid < nodesL.size());
		assert((unsigned int) vid < nodesR.size());
		nodesL[uid].deleteAdjacent(vid);
		nodesR[vid].deleteAdjacent(uid);
		return true;
	}
	catch (...) {
		return false;
	}
}

/**
 * @brief Removes all the arcs connected to lNode (lNode won't have adjacent nodes)
 * @param lNode
 * @return true if all the arcs are successfully removes
 */
template<class T1, class T2>
bool BipartiteGraph<T1, T2>::clearAdjacencesLeftNode(const T1& lNode)
{
	try {
		int uid = getIdLeftNode(lNode);
		for (unsigned int adj : nodesL[uid]) {
			nodesR[adj].deleteAdjacent(uid);
		}
		nodesL[uid].clearAdjacentNodes();
		return true;
	}
	catch (...) {
		return false;
	}
}

/**
 * @brief Removes all the arcs connected to rNode (lNode won't have adjacent nodes)
 * @param rNode
 * @return true if all the arcs are successfully removes
 */
template<class T1, class T2>
bool BipartiteGraph<T1, T2>::clearAdjacencesRightNode(const T2& rNode)
{
	try {
		int vid = getIdRightNode(rNode);
		for (unsigned int adj : nodesR[vid]) {
			nodesL[adj].deleteAdjacent(vid);
		}
		nodesR[vid].clearAdjacentNodes();
		return true;
	}
	catch (...) {
		return false;
	}
}

/**
 * @brief Sets the key of an lNode
 * @param old
 * @param newInfo
 * @return true if the key of the node is successfully modified
 */
template<class T1, class T2>
bool BipartiteGraph<T1, T2>::setLeftNode(const T1& old, const T1& newInfo)
{
	try {
		int uid     = getIdLeftNode(old);
		nodesL[uid] = UndirectedNode<T1>(newInfo);
		mapL.erase(old);
		mapL[newInfo] = uid;
		return true;
	}
	catch (...) {
		return false;
	}
}

/**
 * @brief Sets the key of a rNode
 * @param old
 * @param newInfo
 * @return true if the key of the node is successfully modified
 */
template<class T1, class T2>
bool BipartiteGraph<T1, T2>::setRightNode(const T2& old, const T2& newInfo)
{
	try {
		int vid     = getIdRightNode(old);
		nodesR[vid] = UndirectedNode<T2>(newInfo);
		mapR.erase(old);
		mapR[newInfo] = vid;
		return true;
	}
	catch (...) {
		return false;
	}
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::AdjacentLeftNodeIterator
BipartiteGraph<T1, T2>::adjacentLeftNodeBegin(const T1& lNode) const
{
	int uid = getIdLeftNode(lNode);
	return AdjacentLeftNodeIterator(*this, nodesL[uid].begin());
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::AdjacentLeftNodeIterator
BipartiteGraph<T1, T2>::adjacentLeftNodeEnd(const T1& lNode) const
{
	int uid = getIdLeftNode(lNode);
	return AdjacentLeftNodeIterator(*this, nodesL[uid].end());
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::AdjacentRightNodeIterator
BipartiteGraph<T1, T2>::adjacentRightNodeBegin(const T2& rNode) const
{
	int vid = getIdRightNode(rNode);
	return AdjacentRightNodeIterator(*this, nodesR[vid].begin());
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::AdjacentRightNodeIterator
BipartiteGraph<T1, T2>::adjacentRightNodeEnd(const T2& rNode) const
{
	int vid = getIdRightNode(rNode);
	return AdjacentRightNodeIterator(*this, nodesR[vid].end());
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::LeftNodeIterator BipartiteGraph<T1, T2>::leftNodeBegin() const
{
	return LeftNodeIterator(nodesL.begin());
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::LeftNodeIterator BipartiteGraph<T1, T2>::leftNodeEnd() const
{
	return LeftNodeIterator(nodesL.end());
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::RightNodeIterator BipartiteGraph<T1, T2>::rightNodeBegin() const
{
	return RightNodeIterator(nodesR.begin());
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::RightNodeIterator BipartiteGraph<T1, T2>::rightNodeEnd() const
{
	return RightNodeIterator(nodesR.end());
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::LeftNodeView BipartiteGraph<T1, T2>::leftNodes() const
{
	return LeftNodeView(leftNodeBegin(), leftNodeEnd());
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::RightNodeView BipartiteGraph<T1, T2>::rightNodes() const
{
	return RightNodeView(rightNodeBegin(), rightNodeEnd());
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::AdjacentLeftNodeView
BipartiteGraph<T1, T2>::adjacentLeftNodes(const T1& lNode) const
{
	return AdjacentLeftNodeView(adjacentLeftNodeBegin(lNode), adjacentLeftNodeEnd(lNode));
}

template<class T1, class T2>
typename BipartiteGraph<T1, T2>::AdjacentRightNodeView
BipartiteGraph<T1, T2>::adjacentRightNodes(const T2& rNode) const
{
	return AdjacentRightNodeView(adjacentRightNodeBegin(rNode), adjacentRightNodeEnd(rNode));
}

template<class T1, class T2>
int BipartiteGraph<T1, T2>::getIdLeftNode(const T1& uNode) const
{
	return mapL.at(uNode);
}

template<class T1, class T2>
int BipartiteGraph<T1, T2>::getIdRightNode(const T2& vNode) const
{
	return mapR.at(vNode);
}

} // namespace vcl

#endif // VCL_SPACE_GRAPH_BIPARTITE_BIPARTITE_GRAPH_H
