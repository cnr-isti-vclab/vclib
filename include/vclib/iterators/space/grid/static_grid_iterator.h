/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
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

    VecIt vecIt;
    const GridType* g = nullptr;

public:
    using T = SecondRefPair<KeyType, ValueType>;
    using value_type = T;

    class ArrowHelper
    {
        T value;

    public:
        ArrowHelper(T value) : value(value) {}
        const T* operator->() const { return &value; }
    };

    StaticGridIterator() = default;

    StaticGridIterator(VecIt it, const GridType& g) : vecIt(it), g(&g) {}

    value_type operator*() const
    {
        KeyType cell = g->cellOfIndex(vecIt->first);
        return value_type(cell, vecIt->second);
    }

    ArrowHelper operator->() const { return **this; }

    bool operator==(const StaticGridIterator& oi) const
    {
        return vecIt == oi.vecIt;
    }

    bool operator!=(const StaticGridIterator& oi) const
    {
        return vecIt != oi.vecIt;
    }

    StaticGridIterator operator++()
    {
        ++vecIt;
        return *this;
    }

    StaticGridIterator operator++(int)
    {
        StaticGridIterator old = *this;
        ++vecIt;
        return old;
    }
};

template<typename KeyType, typename ValueType, typename GridType>
class ConstStaticGridIterator
{
    using VecIt = std::vector<std::pair<uint, ValueType>>::const_iterator;

    VecIt vecIt;
    const GridType* g = nullptr;

public:
    using T = SecondRefPair<KeyType, const ValueType>;
    using value_type = T;

    class ArrowHelper
    {
        T value;

    public:
        ArrowHelper(T value) : value(value) {}
        const T* operator->() const { return &value; }
    };

    ConstStaticGridIterator() = default;

    ConstStaticGridIterator(VecIt it, const GridType& g) : vecIt(it), g(&g) {}

    value_type  operator*() const
    {
        KeyType cell = g->cellOfIndex(vecIt->first);
        return value_type(cell, vecIt->second);
    }

    ArrowHelper operator->() const { return **this; }

    bool operator==(const ConstStaticGridIterator& oi) const
    {
        return vecIt == oi.vecIt;
    }

    bool operator!=(const ConstStaticGridIterator& oi) const
    {
        return vecIt != oi.vecIt;
    }

    ConstStaticGridIterator operator++()
    {
        ++vecIt;
        return *this;
    }

    ConstStaticGridIterator operator++(int)
    {
        ConstStaticGridIterator old = *this;
        ++vecIt;
        return old;
    }
};

} // namespace vcl

#endif // VCL_ITERATORS_SPACE_GRID_STATIC_GRID_ITERATOR_H
