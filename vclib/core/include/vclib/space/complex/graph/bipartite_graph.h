/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_SPACE_COMPLEX_GRAPH_BIPARTITE_GRAPH_H
#define VCL_SPACE_COMPLEX_GRAPH_BIPARTITE_GRAPH_H

#include "bipartite_iterators/adjacent_left_node_iterator.h"
#include "bipartite_iterators/adjacent_right_node_iterator.h"
#include "bipartite_iterators/node_iterator.h"
#include "nodes/undirected_node.h"

#include <vclib/types.h>

#include <cassert>
#include <map>
#include <set>
#include <vector>

namespace vcl {

namespace detail {

template<typename Graph, typename Iterator>
class AdjacentLeftNodeIterator;

template<typename Graph, typename Iterator>
class AdjacentRightNodeIterator;

} // namespace detail

template<class T1, class T2>
class BipartiteGraph
{
    template<typename Graph, typename Iterator>
    friend class detail::AdjacentLeftNodeIterator;

    template<typename Graph, typename Iterator>
    friend class detail::AdjacentRightNodeIterator;

protected:
    std::map<T1, unsigned int> mMapL;
    std::map<T2, unsigned int> mMapR;

    std::vector<UndirectedNode<T1>> mNodesL;
    std::vector<UndirectedNode<T2>> mNodesR;

    std::set<unsigned int> mUnusedLNodes;
    std::set<unsigned int> mUnusedRNodes;

public:
    using LeftType      = T1;
    using RightType     = T2;
    using LeftNodeType  = UndirectedNode<T1>;
    using RightNodeType = UndirectedNode<T2>;

    using LeftNodeIterator =
        NodeIterator<typename std::vector<UndirectedNode<T1>>::const_iterator>;
    using RightNodeIterator =
        NodeIterator<typename std::vector<UndirectedNode<T2>>::const_iterator>;

    using LeftNodeView  = View<LeftNodeIterator>;
    using RightNodeView = View<RightNodeIterator>;

    using AdjacentLeftNodeIterator = detail::AdjacentLeftNodeIterator<
        BipartiteGraph<T1, T2>,
        std::unordered_set<unsigned int>::const_iterator>;
    using AdjacentRightNodeIterator = detail::AdjacentRightNodeIterator<
        BipartiteGraph<T1, T2>,
        std::unordered_set<unsigned int>::const_iterator>;

    using AdjacentLeftNodeView  = View<AdjacentLeftNodeIterator>;
    using AdjacentRightNodeView = View<AdjacentRightNodeIterator>;

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
        return mMapL.find(lNode) != mMapL.end();
    }

    /**
     * @brief Checks if a node exists on the right side of the graph
     * @param rNode
     * @return true if the node exists
     */
    bool rightNodeExists(const T2& rNode) const
    {
        return mMapR.find(rNode) != mMapR.end();
    }

    /**
     * @brief Returns the number of left nodes of the graph.
     * @return the number of nodes on the left side of the graph
     */
    uint leftNodesNumber() const
    {
        return (unsigned int) (mNodesL.size() - mUnusedLNodes.size());
    }

    /**
     * @brief Returns the number of right nodes of the graph.
     * @return the number of nodes on the right side of the graph
     */
    uint rightNodesNumber() const
    {
        return (unsigned int) (mNodesR.size() - mUnusedRNodes.size());
    }

    /**
     * @brief Returns the number of adjacent nodes to `lNode`.
     * @param lNode
     * @return the number of adjacent nodes to lNode
     */
    uint adjacentLeftNodeNumber(const T1& lNode) const
    {
        int uid = getIdLeftNode(lNode);
        return mNodesL[uid].sizeAdjacentNodes();
    }

    /**
     * @brief Returns the number of adjacent nodes to `rNode`.
     * @param rNode
     * @return the number of adjacent nodes to rNode
     */
    uint adjacentRightNodeNumber(const T2& rNode) const
    {
        int vid = getIdRightNode(rNode);
        return mNodesR[vid].sizeAdjacentNodes();
    }

    /**
     * @brief Adds a new node on the left side of the graph.
     * @param[in] info: the value associated to the new node
     * @return true if the node is correctly added, false otherwise (if the node
     * already exists)
     */
    bool addLeftNode(const T1& info)
    {
        if (mMapL.find(info) == mMapL.end()) {
            if (mUnusedLNodes.size() == 0) {
                mMapL[info] = (unsigned int) mNodesL.size();
                mNodesL.emplace_back(info);
            }
            else {
                unsigned int id = *(mUnusedLNodes.begin());
                mUnusedLNodes.erase(mUnusedLNodes.begin());
                mMapL[info] = id;
                mNodesL[id] = UndirectedNode<T1>(info);
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
        if (mMapR.find(info) == mMapR.end()) {
            if (mUnusedRNodes.size() == 0) {
                mMapR[info] = (unsigned int) mNodesR.size();
                mNodesR.emplace_back(info);
            }
            else {
                unsigned int id = *(mUnusedRNodes.begin());
                mUnusedRNodes.erase(mUnusedRNodes.begin());
                mMapR[info] = id;
                mNodesR[id] = UndirectedNode<T2>(info);
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
        if (clearAdjacenciesLeftNode(lNode)) {
            mUnusedLNodes.insert(mMapL[lNode]);
            mMapL.erase(lNode);
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
        if (clearAdjacenciesRightNode(rNode)) {
            mUnusedRNodes.insert(mMapR[rNode]);
            mMapR.erase(rNode);
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
            assert((unsigned int) uid < mNodesL.size());
            assert((unsigned int) vid < mNodesR.size());
            mNodesL[uid].addAdjacent(vid);
            mNodesR[vid].addAdjacent(uid);
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
            assert((unsigned int) uid < mNodesL.size());
            assert((unsigned int) vid < mNodesR.size());
            mNodesL[uid].deleteAdjacent(vid);
            mNodesR[vid].deleteAdjacent(uid);
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
    bool clearAdjacenciesLeftNode(const T1& lNode)
    {
        try {
            int uid = getIdLeftNode(lNode);
            for (unsigned int adj : mNodesL[uid]) {
                mNodesR[adj].deleteAdjacent(uid);
            }
            mNodesL[uid].clearAdjacentNodes();
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
    bool clearAdjacenciesRightNode(const T2& rNode)
    {
        try {
            int vid = getIdRightNode(rNode);
            for (unsigned int adj : mNodesR[vid]) {
                mNodesL[adj].deleteAdjacent(vid);
            }
            mNodesR[vid].clearAdjacentNodes();
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
            int uid      = getIdLeftNode(old);
            mNodesL[uid] = UndirectedNode<T1>(newInfo);
            mMapL.erase(old);
            mMapL[newInfo] = uid;
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
            int vid      = getIdRightNode(old);
            mNodesR[vid] = UndirectedNode<T2>(newInfo);
            mMapR.erase(old);
            mMapR[newInfo] = vid;
            return true;
        }
        catch (...) {
            return false;
        }
    }

    AdjacentLeftNodeIterator adjacentLeftNodeBegin(const T1& lNode) const
    {
        int uid = getIdLeftNode(lNode);
        return AdjacentLeftNodeIterator(*this, mNodesL[uid].begin());
    }

    AdjacentLeftNodeIterator adjacentLeftNodeEnd(const T1& lNode) const
    {
        int uid = getIdLeftNode(lNode);
        return AdjacentLeftNodeIterator(*this, mNodesL[uid].end());
    }

    AdjacentRightNodeIterator adjacentRightNodeBegin(const T2& rNode) const
    {
        int vid = getIdRightNode(rNode);
        return AdjacentRightNodeIterator(*this, mNodesR[vid].begin());
    }

    AdjacentRightNodeIterator adjacentRightNodeEnd(const T2& rNode) const
    {
        int vid = getIdRightNode(rNode);
        return AdjacentRightNodeIterator(*this, mNodesR[vid].end());
    }

    LeftNodeIterator leftNodeBegin() const
    {
        return LeftNodeIterator(mNodesL.begin());
    }

    LeftNodeIterator leftNodeEnd() const
    {
        return LeftNodeIterator(mNodesL.end());
    }

    RightNodeIterator rightNodeBegin() const
    {
        return RightNodeIterator(mNodesR.begin());
    }

    RightNodeIterator rightNodeEnd() const
    {
        return RightNodeIterator(mNodesR.end());
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
    int getIdLeftNode(const T1& uNode) const { return mMapL.at(uNode); }

    int getIdRightNode(const T2& vNode) const { return mMapR.at(vNode); }
};

} // namespace vcl

#endif // VCL_SPACE_COMPLEX_GRAPH_BIPARTITE_GRAPH_H
