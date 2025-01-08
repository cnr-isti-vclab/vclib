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
