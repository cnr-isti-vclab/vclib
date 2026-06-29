// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_SPACE_COMPLEX_GRAPH_BIPARTITE_ITERATORS_NODE_ITERATOR_H
#define VCL_SPACE_COMPLEX_GRAPH_BIPARTITE_ITERATORS_NODE_ITERATOR_H

namespace vcl {

template<typename Iterator>
class NodeIterator : public Iterator
{
public:
    using Iterator::Iterator;

    using value_type = Iterator::value_type::InfoType;
    using reference  = const value_type&;
    using pointer    = const value_type*;

    NodeIterator(Iterator it) : Iterator(it) {}

    reference operator*() const { return Iterator::operator*().info(); }

    pointer operator->() const { return &Iterator::operator->().info(); }
};

} // namespace vcl

#endif // VCL_SPACE_COMPLEX_GRAPH_BIPARTITE_ITERATORS_NODE_ITERATOR_H
