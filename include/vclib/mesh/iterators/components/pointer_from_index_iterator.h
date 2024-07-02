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

#ifndef VCL_MESH_ITERATORS_COMPONENTS_POINTER_FROM_INDEX_ITERATOR_H
#define VCL_MESH_ITERATORS_COMPONENTS_POINTER_FROM_INDEX_ITERATOR_H

#include <iterator>

#include <vclib/types.h>

namespace vcl {

/**
 * @brief This iterator is used to iterate over a container of pointers to
 * elements and return the index of the element.
 *
 * @tparam Iterator: The iterator type of the container of pointers to elements.
 */
template<
    typename Iterator,
    typename ElementType,
    typename ParentElement,
    bool CNST = false>
class PointerFromIndexIterator
{
    template<typename, typename, typename, bool>
    friend class PointerFromIndexIterator;

    static constexpr uint ELEM_ID = ElementType::ELEMENT_ID;

    using VT = std::conditional_t<CNST, const ElementType, ElementType>;
    using PE = std::conditional_t<CNST, const ParentElement*, ParentElement*>;

    Iterator mIt;
    PE       parentElement = nullptr;

public:
    using difference_type   = ptrdiff_t;
    using value_type        = VT*;
    using reference         = VT*;
    using pointer           = VT**;
    using iterator_category = std::random_access_iterator_tag;

    PointerFromIndexIterator() = default;

    /**
     * @brief Constructor for the begin iterator - ParentMesh required
     * @param it
     * @param mesh
     */
    PointerFromIndexIterator(Iterator it, PE pElem) :
            mIt(it), parentElement(pElem)
    {
    }

    /**
     * @brief Constructor for the end iterator, ParentMesh not required
     * @param it
     * @param end
     */
    PointerFromIndexIterator(Iterator it) : mIt(it) {}

    PointerFromIndexIterator(const PointerFromIndexIterator&) = default;

    /**
     * @brief Constructor from a non-const iterator
     * @param oi
     */
    PointerFromIndexIterator(const PointerFromIndexIterator<
                             Iterator,
                             ElementType,
                             ParentElement,
                             false>& oi) requires (CNST == true)
            : mIt(oi.mIt), parentElement(oi.parentElement) {};

    value_type operator*() const
    {
        uint e = *mIt;
        if (e == UINT_NULL) [[unlikely]]
            return nullptr;
        else
            return &(parentElement->parentMesh()->template element<ELEM_ID>(e));
    }

    auto operator->() const { return FakePointerWithValue(**this); }

    bool operator==(const PointerFromIndexIterator& oi) const
    {
        return mIt == oi.mIt;
    }

    bool operator!=(const PointerFromIndexIterator& oi) const
    {
        return !(*this == oi);
    }

    PointerFromIndexIterator& operator++()
    {
        ++mIt;
        return *this;
    }

    PointerFromIndexIterator operator++(int)
    {
        auto it = *this;
        ++mIt;
        return it;
    }

    PointerFromIndexIterator& operator--()
    {
        --mIt;
        return *this;
    }

    PointerFromIndexIterator operator--(int)
    {
        auto it = *this;
        --mIt;
        return it;
    }

    PointerFromIndexIterator& operator+=(difference_type n)
    {
        mIt += n;
        return *this;
    }

    PointerFromIndexIterator& operator-=(difference_type n)
    {
        mIt -= n;
        return *this;
    }

    PointerFromIndexIterator operator+(difference_type n) const
    {
        auto it = *this;
        it += n;
        return it;
    }

    PointerFromIndexIterator operator-(difference_type n) const
    {
        auto it = *this;
        it -= n;
        return it;
    }

    difference_type operator-(const PointerFromIndexIterator& oi) const
    {
        return mIt - oi.mIt;
    }

    value_type operator[](difference_type i) { return *(*this + i); }

    bool operator<(const PointerFromIndexIterator& oi) const
    {
        return mIt < oi.mIt;
    }

    bool operator>(const PointerFromIndexIterator& oi) const
    {
        return mIt > oi.mIt;
    }

    bool operator<=(const PointerFromIndexIterator& oi) const
    {
        return mIt <= oi.mIt;
    }

    bool operator>=(const PointerFromIndexIterator& oi) const
    {
        return mIt >= oi.mIt;
    }
};

template<typename Iterator, typename ElementType, typename ParentElement>
using ConstPointerFromIndexIterator =
    PointerFromIndexIterator<Iterator, ElementType, ParentElement, true>;

} // namespace vcl

#endif // VCL_MESH_ITERATORS_COMPONENTS_POINTER_FROM_INDEX_ITERATOR_H
