/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <vclib/space/graph/undirected_node.h>
#include <vclib/types.h>
#include <vclib/views/view.h>

#include "iterator/adjacent_left_node_iterator.h"
#include "iterator/adjacent_right_node_iterator.h"
#include "iterator/node_iterator.h"

namespace vcl {

template<class T1, class T2>
class BipartiteGraph
{
protected:
    std::map<T1, unsigned int> mapL;
    std::map<T2, unsigned int> mapR;

    std::vector<UndirectedNode<T1>> nodesL;
    std::vector<UndirectedNode<T2>> nodesR;

    std::set<unsigned int> unusedLNodes;
    std::set<unsigned int> unusedRNodes;

public:
    using LeftType      = T1;
    using RightType     = T2;
    using LeftNodeType  = UndirectedNode<T1>;
    using RightNodeType = UndirectedNode<T2>;

    using LeftNodeIterator =
        NodeIterator<typename std::vector<UndirectedNode<T1>>::const_iterator>;
    using RightNodeIterator =
        NodeIterator<typename std::vector<UndirectedNode<T2>>::const_iterator>;

    using LeftNodeView  = vcl::View<LeftNodeIterator>;
    using RightNodeView = vcl::View<RightNodeIterator>;

    using AdjacentLeftNodeIterator = detail::AdjacentLeftNodeIterator<
        BipartiteGraph<T1, T2>,
        std::unordered_set<unsigned int>::const_iterator>;
    using AdjacentRightNodeIterator = detail::AdjacentRightNodeIterator<
        BipartiteGraph<T1, T2>,
        std::unordered_set<unsigned int>::const_iterator>;

    using AdjacentLeftNodeView  = vcl::View<AdjacentLeftNodeIterator>;
    using AdjacentRightNodeView = vcl::View<AdjacentRightNodeIterator>;

    /**
     * @brief Default constructor. It creates an empty Bipartite Graph.
     */
    BipartiteGraph() {}

    /**
     * @brief Checks if a node exists on the left side of the graph
     * @param lNode
     * @return true if the node exists
     */
    bool leftNodeExists(const T1& lNode) const
    {
        return mapL.find(lNode) != mapL.end();
    }

    /**
     * @brief Checks if a node exists on the right side of the graph
     * @param rNode
     * @return true if the node exists
     */
    bool rightNodeExists(const T2& rNode) const
    {
        return mapR.find(rNode) != mapR.end();
    }

    /**
     * @brief Returns the number of left nodes of the graph.
     * @return the number of nodes on the left side of the graph
     */
    uint leftNodesNumber() const
    {
        return (unsigned int) (nodesL.size() - unusedLNodes.size());
    }

    /**
     * @brief Returns the number of right nodes of the graph.
     * @return the number of nodes on the right side of the graph
     */
    uint rightNodesNumber() const
    {
        return (unsigned int) (nodesR.size() - unusedRNodes.size());
    }

    /**
     * @brief Returns the number of adjacent nodes to `lNode`.
     * @param lNode
     * @return the number of adjacent nodes to lNode
     */
    uint adjacentLeftNodeNumber(const T1& lNode) const
    {
        int uid = getIdLeftNode(lNode);
        return nodesL[uid].sizeAdjacentNodes();
    }

    /**
     * @brief Returns the number of adjacent nodes to `rNode`.
     * @param rNode
     * @return the number of adjacent nodes to rNode
     */
    uint adjacentRightNodeNumber(const T2& rNode) const
    {
        int vid = getIdRightNode(rNode);
        return nodesR[vid].sizeAdjacentNodes();
    }

    /**
     * @brief Adds a new node on the left side of the graph.
     * @param[in] info: the value associated to the new node
     * @return true if the node is correctly added, false otherwise (if the node
     * already exists)
     */
    bool addLeftNode(const T1& info)
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
     * @return true if the node is correctly added, false otherwise (if the node
     * already exists)
     */
    bool addRightNode(const T2& info)
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
     * @brief Removes lNode and all its arcs from the graph
     * @param lNode
     * @return true if the node is successfully deleted
     */
    bool deleteLeftNode(const T1& lNode)
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
    bool deleteRightNode(const T2& rNode)
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
     * @return true if the arc is successfully created (both nodes exists in the
     * graph)
     */
    bool addArc(const T1& lNode, const T2& rNode)
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
     * @brief Removes the arc between lNode and rNode
     * @param lNode
     * @param rNode
     * @return true if the arc is successfully deleted (both nodes and the arc
     * exists in the graph)
     */
    bool deleteArc(const T1& lNode, const T2& rNode)
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
     * @brief Removes all the arcs connected to lNode (lNode won't have adjacent
     * nodes)
     * @param lNode
     * @return true if all the arcs are successfully removes
     */
    bool clearAdjacencesLeftNode(const T1& lNode)
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
     * @brief Removes all the arcs connected to rNode (lNode won't have adjacent
     * nodes)
     * @param rNode
     * @return true if all the arcs are successfully removes
     */
    bool clearAdjacencesRightNode(const T2& rNode)
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
    bool setLeftNode(const T1& old, const T1& newInfo)
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
    bool setRightNode(const T2& old, const T2& newInfo)
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

    AdjacentLeftNodeIterator adjacentLeftNodeBegin(const T1& lNode) const
    {
        int uid = getIdLeftNode(lNode);
        return AdjacentLeftNodeIterator(*this, nodesL[uid].begin());
    }

    AdjacentLeftNodeIterator adjacentLeftNodeEnd(const T1& lNode) const
    {
        int uid = getIdLeftNode(lNode);
        return AdjacentLeftNodeIterator(*this, nodesL[uid].end());
    }

    AdjacentRightNodeIterator adjacentRightNodeBegin(const T2& rNode) const
    {
        int vid = getIdRightNode(rNode);
        return AdjacentRightNodeIterator(*this, nodesR[vid].begin());
    }

    AdjacentRightNodeIterator adjacentRightNodeEnd(const T2& rNode) const
    {
        int vid = getIdRightNode(rNode);
        return AdjacentRightNodeIterator(*this, nodesR[vid].end());
    }

    LeftNodeIterator leftNodeBegin() const
    {
        return LeftNodeIterator(nodesL.begin());
    }

    LeftNodeIterator leftNodeEnd() const
    {
        return LeftNodeIterator(nodesL.end());
    }

    RightNodeIterator rightNodeBegin() const
    {
        return RightNodeIterator(nodesR.begin());
    }

    RightNodeIterator rightNodeEnd() const
    {
        return RightNodeIterator(nodesR.end());
    }

    LeftNodeView leftNodes() const
    {
        return LeftNodeView(leftNodeBegin(), leftNodeEnd());
    }

    RightNodeView rightNodes() const
    {
        return RightNodeView(rightNodeBegin(), rightNodeEnd());
    }

    AdjacentLeftNodeView adjacentLeftNodes(const T1& lNode) const
    {
        return AdjacentLeftNodeView(
            adjacentLeftNodeBegin(lNode), adjacentLeftNodeEnd(lNode));
    }

    AdjacentRightNodeView adjacentRightNodes(const T2& rNode) const
    {
        return AdjacentRightNodeView(
            adjacentRightNodeBegin(rNode), adjacentRightNodeEnd(rNode));
    }

protected:
    int getIdLeftNode(const T1& uNode) const { return mapL.at(uNode); }

    int getIdRightNode(const T2& vNode) const { return mapR.at(vNode); }
};

} // namespace vcl

#endif // VCL_SPACE_GRAPH_BIPARTITE_BIPARTITE_GRAPH_H
