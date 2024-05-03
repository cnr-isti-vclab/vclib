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

#ifndef VCL_ITERATORS_CONST_POINTER_ITERATOR_H
#define VCL_ITERATORS_CONST_POINTER_ITERATOR_H

#include <iterator>

#include <vclib/concepts/pointers.h>
#include <vclib/types/const_correctness.h>

namespace vcl {

/**
 * @brief The ConstPointerIterator class is a utility class that wraps an
 * iterator to a container of pointers, and returns a const pointer when
 * dereferencing the iterator.
 *
 * @tparam It The iterator type.
 */
template<typename It>
requires (vcl::IsPointer<typename std::iterator_traits<It>::value_type>)
class ConstPointerIterator
{
    using Base = It;

    It mIt;

public:
    using difference_type = std::iterator_traits<It>::difference_type;
    using value_type =
        MakeConstPointerT<typename std::iterator_traits<It>::value_type>;
    using reference = value_type&;
    using pointer = value_type*;
    using iterator_category = std::iterator_traits<It>::iterator_category;

    ConstPointerIterator() = default;

    ConstPointerIterator(It it) : mIt(it) {}

    value_type operator*() const { return *mIt; }
    pointer operator->() const { return &(*mIt); }

    bool operator==(const ConstPointerIterator& oi) const
    {
        return mIt == oi.mIt;
    }

    bool operator!=(const ConstPointerIterator& oi) const
    {
        return mIt != oi.mIt;
    }

    ConstPointerIterator& operator++()
    {
        ++mIt;
        return *this;
    }

    ConstPointerIterator operator++(int)
    {
        ConstPointerIterator tmp = *this;
        ++mIt;
        return tmp;
    }

    ConstPointerIterator& operator--() requires(std::bidirectional_iterator<It>)
    {
        --mIt;
        return *this;
    }

    ConstPointerIterator operator--(int)
        requires (std::bidirectional_iterator<It>)
    {
        ConstPointerIterator tmp = *this;
        --mIt;
        return tmp;
    }

    ConstPointerIterator& operator+=(difference_type n)
        requires (std::random_access_iterator<It>)
    {
        mIt += n;
        return *this;
    }

    ConstPointerIterator operator+(difference_type n) const
        requires (std::random_access_iterator<It>)
    {
        return ConstPointerIterator(mIt + n);
    }

    friend ConstPointerIterator operator+(difference_type n, const ConstPointerIterator& it)
        requires (std::random_access_iterator<It>)
    {
        return ConstPointerIterator(n + it.mIt);
    }

    ConstPointerIterator& operator-=(difference_type n)
        requires (std::random_access_iterator<It>)
    {
        mIt -= n;
        return *this;
    }

    ConstPointerIterator operator-(difference_type n) const
        requires (std::random_access_iterator<It>)
    {
        return ConstPointerIterator(mIt - n);
    }

    difference_type operator-(const ConstPointerIterator& oi) const
        requires (std::random_access_iterator<It>)
    {
        return mIt - oi.mIt;
    }

    value_type operator[](difference_type n) const
        requires (std::random_access_iterator<It>)
    {
        return mIt[n];
    }
};


} // namespace vcl

#endif // VCL_ITERATORS_CONST_POINTER_ITERATOR_H
