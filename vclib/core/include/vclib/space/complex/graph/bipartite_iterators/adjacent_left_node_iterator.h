// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_SPACE_COMPLEX_GRAPH_BIPARTITE_ITERATORS_ADJACENT_LEFT_NODE_ITERATOR_H
#define VCL_SPACE_COMPLEX_GRAPH_BIPARTITE_ITERATORS_ADJACENT_LEFT_NODE_ITERATOR_H

#include "adjacent_node_iterator.h"

namespace vcl::detail {

template<typename Graph, typename Iterator>
class AdjacentLeftNodeIterator : public AdjacentNodeIterator<Graph, Iterator>
{
    using Base = AdjacentNodeIterator<Graph, Iterator>;

public:
    using value_type = Graph::RightType;
    using reference  = const value_type&;
    using pointer    = const value_type*;

    using AdjacentNodeIterator<Graph, Iterator>::AdjacentNodeIterator;

    reference operator*() const
    {
        return Base::mGraph->mNodesR[*(Base::mIt)].info();
    }

    pointer operator->() const
    {
        return &Base::mGraph->mNodesR[*(Base::mIt)].info();
    }
};

} // namespace vcl::detail

#endif // VCL_SPACE_COMPLEX_GRAPH_BIPARTITE_ITERATORS_ADJACENT_LEFT_NODE_ITERATOR_H
