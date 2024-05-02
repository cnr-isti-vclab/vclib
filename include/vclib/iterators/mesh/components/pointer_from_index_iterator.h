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

#ifndef VCL_ITERATORS_MESH_COMPONENTS_POINTER_FROM_INDEX_ITERATOR_H
#define VCL_ITERATORS_MESH_COMPONENTS_POINTER_FROM_INDEX_ITERATOR_H

#include <iterator>

#include <vclib/types.h>

namespace vcl {

/**
 * @brief This iterator is used to iterate over a container of pointers to
 * elements and return the index of the element.
 *
 * @tparam Iterator: The iterator type of the container of pointers to elements.
 */
template<typename Iterator, typename ElementType, typename MeshType>
class PointerFromIndexIterator
{
    static constexpr uint ELEM_ID = ElementType::ELEM_ID;

    Iterator mIt;
    const ElementType* mCurrent = nullptr;
    const MeshType* parentMesh = nullptr;

public:
    using difference_type   = ptrdiff_t;
    using value_type        = const ElementType*;
    using reference         = value_type&;
    using pointer           = value_type*;
    using iterator_category = std::random_access_iterator_tag;

    PointerFromIndexIterator() = default;

    /**
     * @brief Constructor for the begin iterator - ParentMesh required
     * @param it
     * @param mesh
     */
    PointerFromIndexIterator(const Iterator& it, const MeshType* mesh) :
            mIt(it), parentMesh(mesh)
    {
        updateCurrent();
    }

    /**
     * @brief Constructor for the end iterator, ParentMesh not required
     * @param it
     * @param end
     */
    PointerFromIndexIterator(const Iterator& it) : mIt(it)
    {
    }

    value_type operator*() const { return mCurrent; }

    pointer operator->() const { return &mCurrent; }

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
        updateCurrent();
        return *this;
    }

    PointerFromIndexIterator operator++(int)
    {
        auto it = *this;
        ++mIt;
        updateCurrent();
        return it;
    }

    PointerFromIndexIterator& operator--()
    {
        --mIt;
        updateCurrent();
        return *this;
    }

    PointerFromIndexIterator operator--(int)
    {
        auto it = *this;
        --mIt;
        updateCurrent();
        return it;
    }

    PointerFromIndexIterator& operator+=(difference_type n)
    {
        mIt += n;
        updateCurrent();
        return *this;
    }

    PointerFromIndexIterator& operator-=(difference_type n)
    {
        mIt -= n;
        updateCurrent();
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

private:
    void updateCurrent()
    {
        uint e = *mIt;
        if (e == UINT_NULL) [[unlikely]]
            mCurrent = nullptr;
        else
            mCurrent = &(parentMesh->template element<ELEM_ID>(e));
    }
};

} // namespace vcl

#endif // VCL_ITERATORS_MESH_COMPONENTS_POINTER_FROM_INDEX_ITERATOR_H
