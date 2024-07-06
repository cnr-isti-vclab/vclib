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

#ifndef VCL_SPACE_COMPLEX_GRAPH_BIPARTITE_ITERATOR_NODE_ITERATOR_H
#define VCL_SPACE_COMPLEX_GRAPH_BIPARTITE_ITERATOR_NODE_ITERATOR_H

namespace vcl {

template<typename Iterator>
class NodeIterator
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

#endif // VCL_SPACE_COMPLEX_GRAPH_BIPARTITE_ITERATOR_NODE_ITERATOR_H
