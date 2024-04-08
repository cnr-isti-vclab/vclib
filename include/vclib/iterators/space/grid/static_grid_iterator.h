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

#ifndef VCL_ITERATORS_SPACE_GRID_STATIC_GRID_ITERATOR_H
#define VCL_ITERATORS_SPACE_GRID_STATIC_GRID_ITERATOR_H

#include <vclib/misc/pair.h>
#include <vclib/space/point.h>

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

    class ArrowHelper
    {
        T mValue;

    public:
        ArrowHelper(T value) : mValue(value) {}

        const T* operator->() const { return &mValue; }
    };

    ConstStaticGridIterator() = default;

    ConstStaticGridIterator(VecIt it, const GridType& g) : mVecIt(it), mGrid(&g)
    {
    }

    value_type operator*() const
    {
        KeyType cell = mGrid->cellOfIndex(mVecIt->first);
        return value_type(cell, mVecIt->second);
    }

    ArrowHelper operator->() const { return **this; }

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

#endif // VCL_ITERATORS_SPACE_GRID_STATIC_GRID_ITERATOR_H
