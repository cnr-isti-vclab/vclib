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

#ifndef VCL_ITERATORS_SPACE_GRID_CELL_ITERATOR_H
#define VCL_ITERATORS_SPACE_GRID_CELL_ITERATOR_H

#include <vclib/space/point.h>

namespace vcl {

template<int N>
class CellIterator
{
    vcl::Point<uint, N> it;
    vcl::Point<uint, N> first, end;

public:
    using difference_type   = ptrdiff_t;
    using value_type        = vcl::Point<uint, N>;
    using reference         = const vcl::Point<uint, N>&;
    using pointer           = const vcl::Point<uint, N>*;
    using iterator_category = std::forward_iterator_tag;

    CellIterator()
    {
        it.setConstant(-1);
        first = end = it;
    }

    CellIterator(
        const vcl::Point<uint, N>& first,
        const vcl::Point<uint, N>& end) :
            it(first),
            first(first), end(end)
    {
    }

    reference operator*() const { return it; }

    pointer operator->() const { return &it; }

    bool operator==(const CellIterator& oi) const { return (it == oi.it); }

    bool operator!=(const CellIterator& oi) const { return (it != oi.it); }

    CellIterator operator++()
    {
        uint d = N - 1;
        while (d != -1 && it(d) == end(d) - 1) {
            it(d) = first(d);
            d--;
        }
        if (d != -1)
            it(d)++;
        else
            it.setConstant(-1);
        return *this;
    }

    CellIterator operator++(int)
    {
        CellIterator<N> oit = it;
        ++(*this);
        return oit;
    }
};

} // namespace vcl

#endif // VCL_ITERATORS_SPACE_GRID_CELL_ITERATOR_H
