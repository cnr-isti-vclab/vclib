// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_ITERATORS_COMPONENTS_INDEX_FROM_POINTER_ITERATOR_H
#define VCL_MESH_ITERATORS_COMPONENTS_INDEX_FROM_POINTER_ITERATOR_H

#include <vclib/base.h>

#include <iterator>

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
    Iterator mIt;

public:
    using difference_type   = ptrdiff_t;
    using value_type        = uint;
    using reference         = const uint&;
    using pointer           = const uint*;
    using iterator_category = std::random_access_iterator_tag;

    IndexFromPointerIterator() = default;

    IndexFromPointerIterator(const Iterator& it) : mIt(it) {}

    value_type operator*() const
    {
        auto e = *mIt;
        if (e == nullptr) [[unlikely]]
            return UINT_NULL;
        else
            return e->index();
    }

    auto operator->() const { return FakePointerWithValue(**this); }

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
        return *this;
    }

    IndexFromPointerIterator operator++(int)
    {
        auto it = *this;
        ++mIt;
        return it;
    }

    IndexFromPointerIterator& operator--()
    {
        --mIt;
        return *this;
    }

    IndexFromPointerIterator operator--(int)
    {
        auto it = *this;
        --mIt;
        return it;
    }

    IndexFromPointerIterator& operator+=(difference_type n)
    {
        mIt += n;
        return *this;
    }

    IndexFromPointerIterator& operator-=(difference_type n)
    {
        mIt -= n;
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
};

} // namespace vcl

#endif // VCL_MESH_ITERATORS_COMPONENTS_INDEX_FROM_POINTER_ITERATOR_H
