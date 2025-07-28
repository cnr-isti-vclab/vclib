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

#ifndef VCL_SPACE_COMPLEX_GRID_ITERATORS_CELL_ITERATOR_H
#define VCL_SPACE_COMPLEX_GRID_ITERATORS_CELL_ITERATOR_H

#include <vclib/space/core.h>

namespace vcl {

template<int N>
class CellIterator
{
    Point<uint, N> mIt;
    Point<uint, N> mFirst, mEnd;

public:
    using difference_type   = ptrdiff_t;
    using value_type        = Point<uint, N>;
    using reference         = const Point<uint, N>&;
    using pointer           = const Point<uint, N>*;
    using iterator_category = std::forward_iterator_tag;

    CellIterator()
    {
        mIt.setConstant(UINT_NULL);
        mFirst = mEnd = mIt;
    }

    CellIterator(const Point<uint, N>& first, const Point<uint, N>& end) :
            mIt(first), mFirst(first), mEnd(end)
    {
    }

    reference operator*() const { return mIt; }

    pointer operator->() const { return &mIt; }

    bool operator==(const CellIterator& oi) const { return (mIt == oi.mIt); }

    bool operator!=(const CellIterator& oi) const { return (mIt != oi.mIt); }

    CellIterator operator++()
    {
        uint d = N - 1;
        while (d != UINT_NULL && mIt(d) == mEnd(d) - 1) {
            mIt(d) = mFirst(d);
            d--;
        }
        if (d != -1)
            mIt(d)++;
        else
            mIt.setConstant(UINT_NULL);
        return *this;
    }

    CellIterator operator++(int)
    {
        CellIterator<N> oit = mIt;
        ++(*this);
        return oit;
    }
};

} // namespace vcl

#endif // VCL_SPACE_COMPLEX_GRID_ITERATORS_CELL_ITERATOR_H
