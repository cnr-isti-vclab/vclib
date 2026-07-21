// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_SPACE_COMPLEX_GRAPH_NODES_UNDIRECTED_NODE_H
#define VCL_SPACE_COMPLEX_GRAPH_NODES_UNDIRECTED_NODE_H

#include <unordered_set>

namespace vcl {

template<class T>
class UndirectedNode
{
protected:
    T                                mInfo;
    std::unordered_set<unsigned int> mAdjacentNodes;

public:
    using InfoType = T;

    UndirectedNode() {}

    UndirectedNode(const T& info) : mInfo(info) {}

    const T& info() const { return mInfo; }

    T& info() { return mInfo; }

    void addAdjacent(unsigned int node) { mAdjacentNodes.insert(node); }

    bool isAdjacent(unsigned int node) const
    {
        return mAdjacentNodes.find(node) != mAdjacentNodes.end();
    }

    void deleteAdjacent(unsigned int node) { mAdjacentNodes.erase(node); }

    void clearAdjacentNodes() { mAdjacentNodes.clear(); }

    std::unordered_set<unsigned int>::const_iterator begin() const
    {
        return mAdjacentNodes.begin();
    }

    std::unordered_set<unsigned int>::const_iterator end() const
    {
        return mAdjacentNodes.end();
    }

    unsigned int sizeAdjacentNodes() const
    {
        return (unsigned int) mAdjacentNodes.size();
    }
};

} // namespace vcl

#endif // VCL_SPACE_COMPLEX_GRAPH_NODES_UNDIRECTED_NODE_H
