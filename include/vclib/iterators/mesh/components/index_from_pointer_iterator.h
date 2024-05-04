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

#ifndef VCL_ITERATORS_MESH_COMPONENTS_INDEX_FROM_POINTER_ITERATOR_H
#define VCL_ITERATORS_MESH_COMPONENTS_INDEX_FROM_POINTER_ITERATOR_H

#include <iterator>

#include <vclib/types.h>

namespace vcl {

/**
 * @brief This iterator is used to iterate over a container of pointers to
 * elements and return the index of the element.
 *
 * @tparam Iterator: The iterator type of the container of pointers to elements.
 */
template<typename Iterator>
class IndexFromPointerIterator
{
    Iterator mIt, mEnd;
    uint     mCurrent = UINT_NULL;

public:
    using difference_type   = ptrdiff_t;
    using value_type        = uint;
    using reference         = const uint&;
    using pointer           = const uint*;
    using iterator_category = std::random_access_iterator_tag;

    IndexFromPointerIterator() = default;

    IndexFromPointerIterator(const Iterator& it, const Iterator& end) :
            mIt(it), mEnd(end)
    {
        if (mIt != mEnd)
            updateCurrent();
    }

    reference operator*() const { return mCurrent; }

    pointer operator->() const { return &mCurrent; }

    bool operator==(const IndexFromPointerIterator& oi) const
    {
        return mIt == oi.mIt;
    }

    bool operator!=(const IndexFromPointerIterator& oi) const
    {
        return !(*this == oi);
    }

    IndexFromPointerIterator& operator++()
    {
        ++mIt;
        updateCurrent();
        return *this;
    }

    IndexFromPointerIterator operator++(int)
    {
        auto it = *this;
        ++mIt;
        updateCurrent();
        return it;
    }

    IndexFromPointerIterator& operator--()
    {
        --mIt;
        updateCurrent();
        return *this;
    }

    IndexFromPointerIterator operator--(int)
    {
        auto it = *this;
        --mIt;
        updateCurrent();
        return it;
    }

    IndexFromPointerIterator& operator+=(difference_type n)
    {
        mIt += n;
        updateCurrent();
        return *this;
    }

    IndexFromPointerIterator& operator-=(difference_type n)
    {
        mIt -= n;
        updateCurrent();
        return *this;
    }

    IndexFromPointerIterator operator+(difference_type n) const
    {
        auto it = *this;
        it += n;
        return it;
    }

    IndexFromPointerIterator operator-(difference_type n) const
    {
        auto it = *this;
        it -= n;
        return it;
    }

    difference_type operator-(const IndexFromPointerIterator& oi) const
    {
        return mIt - oi.mIt;
    }

    value_type operator[](difference_type i) { return *(*this + i); }

    bool operator<(const IndexFromPointerIterator& oi) const
    {
        return mIt < oi.mIt;
    }

    bool operator>(const IndexFromPointerIterator& oi) const
    {
        return mIt > oi.mIt;
    }

    bool operator<=(const IndexFromPointerIterator& oi) const
    {
        return mIt <= oi.mIt;
    }

    bool operator>=(const IndexFromPointerIterator& oi) const
    {
        return mIt >= oi.mIt;
    }

private:
    void updateCurrent()
    {
        if (mIt != mEnd) [[likely]] {
            auto e = *mIt;
            if (e == nullptr) [[unlikely]]
                mCurrent = UINT_NULL;
            else
                mCurrent = e->index();
        }
        else {
            mCurrent = UINT_NULL;
        }
    }
};

} // namespace vcl

#endif // VCL_ITERATORS_MESH_COMPONENTS_INDEX_FROM_POINTER_ITERATOR_H
