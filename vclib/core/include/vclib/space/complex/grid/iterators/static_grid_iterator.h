// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_SPACE_COMPLEX_GRID_ITERATORS_STATIC_GRID_ITERATOR_H
#define VCL_SPACE_COMPLEX_GRID_ITERATORS_STATIC_GRID_ITERATOR_H

#include <vclib/space/core.h>

namespace vcl {

template<typename KeyType, typename ValueType, typename GridType>
class StaticGridIterator
{
    using VecIt = std::vector<std::pair<uint, ValueType>>::iterator;

    VecIt           mVecIt;
    const GridType* mGrid = nullptr;

public:
    using T          = SecondRefPair<KeyType, ValueType>;
    using value_type = T;

    class ArrowHelper
    {
        T mValue;

    public:
        ArrowHelper(T value) : mValue(value) {}

        const T* operator->() const { return &mValue; }
    };

    StaticGridIterator() = default;

    StaticGridIterator(VecIt it, const GridType& g) : mVecIt(it), mGrid(&g) {}

    value_type operator*() const
    {
        KeyType cell = mGrid->cellOfIndex(mVecIt->first);
        return value_type(cell, mVecIt->second);
    }

    ArrowHelper operator->() const { return **this; }

    bool operator==(const StaticGridIterator& oi) const
    {
        return mVecIt == oi.mVecIt;
    }

    bool operator!=(const StaticGridIterator& oi) const
    {
        return mVecIt != oi.mVecIt;
    }

    StaticGridIterator operator++()
    {
        ++mVecIt;
        return *this;
    }

    StaticGridIterator operator++(int)
    {
        StaticGridIterator old = *this;
        ++mVecIt;
        return old;
    }
};

template<typename KeyType, typename ValueType, typename GridType>
class ConstStaticGridIterator
{
    using VecIt = std::vector<std::pair<uint, ValueType>>::const_iterator;

    VecIt           mVecIt;
    const GridType* mGrid = nullptr;

public:
    using T          = SecondRefPair<KeyType, const ValueType>;
    using value_type = T;

    ConstStaticGridIterator() = default;

    ConstStaticGridIterator(VecIt it, const GridType& g) : mVecIt(it), mGrid(&g)
    {
    }

    value_type operator*() const
    {
        KeyType cell = mGrid->cellOfIndex(mVecIt->first);
        return value_type(cell, mVecIt->second);
    }

    auto operator->() const { return FakePointerWithValue(**this); }

    bool operator==(const ConstStaticGridIterator& oi) const
    {
        return mVecIt == oi.mVecIt;
    }

    bool operator!=(const ConstStaticGridIterator& oi) const
    {
        return mVecIt != oi.mVecIt;
    }

    ConstStaticGridIterator operator++()
    {
        ++mVecIt;
        return *this;
    }

    ConstStaticGridIterator operator++(int)
    {
        ConstStaticGridIterator old = *this;
        ++mVecIt;
        return old;
    }
};

} // namespace vcl

#endif // VCL_SPACE_COMPLEX_GRID_ITERATORS_STATIC_GRID_ITERATOR_H
