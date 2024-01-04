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

#ifndef VCL_SPACE_GRAPH_UNDIRECTED_NODE_H
#define VCL_SPACE_GRAPH_UNDIRECTED_NODE_H

#include <unordered_set>

namespace vcl {

template<class T>
class UndirectedNode
{
protected:
    T                                inf;
    std::unordered_set<unsigned int> adjacentNodes;

public:
    using InfoType = T;

    UndirectedNode() {}

    UndirectedNode(const T& info) : inf(info) {}

    const T& info() const { return inf; }

    T& info() { return inf; }

    void addAdjacent(unsigned int node) { adjacentNodes.insert(node); }

    bool isAdjacent(unsigned int node) const
    {
        return adjacentNodes.find(node) != adjacentNodes.end();
    }

    void deleteAdjacent(unsigned int node) { adjacentNodes.erase(node); }

    void clearAdjacentNodes() { adjacentNodes.clear(); }

    std::unordered_set<unsigned int>::const_iterator begin() const
    {
        return adjacentNodes.begin();
    }

    std::unordered_set<unsigned int>::const_iterator end() const
    {
        return adjacentNodes.end();
    }

    unsigned int sizeAdjacentNodes() const
    {
        return (unsigned int) adjacentNodes.size();
    }
};

} // namespace vcl

#endif // VCL_SPACE_GRAPH_UNDIRECTED_NODE_H
