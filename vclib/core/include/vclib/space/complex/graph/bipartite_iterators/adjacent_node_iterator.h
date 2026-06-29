// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_SPACE_COMPLEX_GRAPH_BIPARTITE_ITERATORS_ADJACENT_NODE_ITERATOR_H
#define VCL_SPACE_COMPLEX_GRAPH_BIPARTITE_ITERATORS_ADJACENT_NODE_ITERATOR_H

namespace vcl::detail {

template<typename Graph, typename Iterator>
class AdjacentNodeIterator
{
protected:
    const Graph* mGraph = nullptr;
    Iterator     mIt;

public:
    AdjacentNodeIterator() {}

    AdjacentNodeIterator(const Graph& g, Iterator it) : mGraph(&g), mIt(it) {}

    bool operator==(const AdjacentNodeIterator& otherIterator) const
    {
        return (mGraph == otherIterator.mGraph && mIt == otherIterator.mIt);
    }

    bool operator!=(const AdjacentNodeIterator& otherIterator) const
    {
        return !(*this == otherIterator);
    }

    AdjacentNodeIterator operator++()
    {
        ++mIt;
        return *this;
    }

    AdjacentNodeIterator operator++(int)
    {
        AdjacentNodeIterator tmp;
        ++mIt;
        return tmp;
    }
};

} // namespace vcl::detail

#endif // VCL_SPACE_COMPLEX_GRAPH_BIPARTITE_ITERATORS_ADJACENT_NODE_ITERATOR_H
