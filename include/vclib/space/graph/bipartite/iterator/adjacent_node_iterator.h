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

#ifndef VCL_SPACE_GRAPH_BIPARTITE_ITERATOR_ADJACENT_NODE_ITERATOR_H
#define VCL_SPACE_GRAPH_BIPARTITE_ITERATOR_ADJACENT_NODE_ITERATOR_H

namespace vcl::detail {

template<typename Graph, typename Iterator>
class AdjacentNodeIterator
{
protected:
    const Graph* g = nullptr;
    Iterator     it;

public:
    AdjacentNodeIterator() {}

    AdjacentNodeIterator(const Graph& g, Iterator it) : g(&g), it(it) {}

    bool operator==(const AdjacentNodeIterator& otherIterator) const
    {
        return (g == otherIterator.g && it == otherIterator.it);
    }

    bool operator!=(const AdjacentNodeIterator& otherIterator) const
    {
        return !(*this == otherIterator);
    }

    AdjacentNodeIterator operator++()
    {
        ++it;
        return *this;
    }

    AdjacentNodeIterator operator++(int)
    {
        AdjacentNodeIterator tmp;
        ++it;
        return tmp;
    }
};

} // namespace vcl::detail

#endif // VCL_SPACE_GRAPH_BIPARTITE_ITERATOR_ADJACENT_NODE_ITERATOR_H
