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

#ifndef VCL_MESH_ITERATORS_ELEMENT_CONTAINER_ITERATOR_H
#define VCL_MESH_ITERATORS_ELEMENT_CONTAINER_ITERATOR_H

#include <iterator>
#include <type_traits>

namespace vcl {

template<
    template<typename, typename...>
    typename Container,
    typename T,
    bool CNST = false>
class ElementContainerIterator
{
    using ContIt = std::conditional_t<
        CNST,
        typename Container<T>::const_iterator,
        typename Container<T>::iterator>;

public:
    using difference_type   = ptrdiff_t;
    using value_type        = T;
    using reference         = std::conditional_t<CNST, const T&, T&>;
    using pointer           = std::conditional_t<CNST, const T*, T*>;
    using iterator_category = std::random_access_iterator_tag;

private:
    ContIt mIt; // the actual iterator

    const Container<T>* mVec = nullptr; // need to check end when jump elements

    /* Pointers to functions */

    // pointer to mIncrementFun function
    ElementContainerIterator& (ElementContainerIterator::*mIncrementFun)() =
        &ElementContainerIterator::incrementJump;

    // pointer to post increment function
    ElementContainerIterator (ElementContainerIterator::*mPostIncrementFun)() =
        &ElementContainerIterator::postIncrementJump;

    // pointer to decrement function
    ElementContainerIterator& (ElementContainerIterator::*mDecrementFun)() =
        &ElementContainerIterator::decrementJump;

    // pointer to post decrement function
    ElementContainerIterator (ElementContainerIterator::*mPostDecrementFun)() =
        &ElementContainerIterator::postDecrementJump;

    // pointer to assignment sum function
    ElementContainerIterator& (ElementContainerIterator::*mAssignSumFun)(
        difference_type) = &ElementContainerIterator::assignSumJump;

    // pointer to diff between iterators function
    difference_type (ElementContainerIterator::*mDiffFun)(
        const ElementContainerIterator& oi) const =
        &ElementContainerIterator::diffJump;

public:
    ElementContainerIterator() = default;

    ElementContainerIterator(
        ContIt              it,
        const Container<T>& vec,
        bool                jumpDeleted = true) : mIt(it), mVec(&vec)
    {
        if (!jumpDeleted) {
            mIncrementFun     = &ElementContainerIterator::incrementFast;
            mPostIncrementFun = &ElementContainerIterator::postIncrementFast;
            mDecrementFun     = &ElementContainerIterator::decrementFast;
            mPostDecrementFun = &ElementContainerIterator::postDecrementFast;
            mAssignSumFun     = &ElementContainerIterator::assignSumFast;
            mDiffFun          = &ElementContainerIterator::diffFast;
        }
        else {
            // if the user asked to jump the deleted elements, and the first
            // element is deleted, we need to move forward until we find the
            // first non-deleted element
            while (mIt != mVec->end() && mIt->deleted()) {
                ++mIt;
            }
        }
    }

    reference operator*() const { return *mIt; }

    pointer operator->() const { return &*mIt; }

    bool operator==(const ElementContainerIterator& oi) const
    {
        return mIt == oi.mIt;
    }

    bool operator!=(const ElementContainerIterator& oi) const
    {
        return mIt != oi.mIt;
    }

    ElementContainerIterator& operator++() { return (this->*mIncrementFun)(); }

    ElementContainerIterator operator++(int)
    {
        return (this->*mPostIncrementFun)();
    }

    ElementContainerIterator& operator--() { return (this->*mDecrementFun)(); }

    ElementContainerIterator operator--(int)
    {
        return (this->*mPostDecrementFun)();
    }

    ElementContainerIterator& operator+=(difference_type n)
    {
        return (this->*mAssignSumFun)(n);
    }

    ElementContainerIterator& operator-=(difference_type n)
    {
        return (this->*mAssignSumFun)(-n);
    }

    ElementContainerIterator operator+(difference_type n) const
    {
        ElementContainerIterator temp = *this;
        return temp += n;
    }

    ElementContainerIterator operator-(difference_type n) const
    {
        ElementContainerIterator temp = *this;
        return temp += -n;
    }

    difference_type operator-(const ElementContainerIterator& oi) const
    {
        return (this->*mDiffFun)(oi);
    }

    const reference operator[](difference_type i) const { return *(*this + i); }

    reference operator[](difference_type i) { return *(*this + i); }

    bool operator<(const ElementContainerIterator& oi) const
    {
        return mIt < oi.mIt;
    }

    bool operator>(const ElementContainerIterator& oi) const
    {
        return mIt > oi.mIt;
    }

    bool operator<=(const ElementContainerIterator& oi) const
    {
        return mIt <= oi.mIt;
    }

    bool operator>=(const ElementContainerIterator& oi) const
    {
        return mIt >= oi.mIt;
    }

private:
    /**
     * @brief Increment function that will be called if we need to jump deleted
     * elements.
     */
    ElementContainerIterator& incrementJump()
    {
        do {
            ++mIt;
        } while (mIt != mVec->end() && mIt->deleted());
        return *this;
    }

    /**
     * @brief Post increment function that will be called if we need to jump
     * deleted elements.
     */
    ElementContainerIterator postIncrementJump()
    {
        ElementContainerIterator old = *this;
        do {
            ++mIt;
        } while (mIt != mVec->end() && mIt->deleted());
        return old;
    }

    /**
     * @brief Increment function that will be called if we don't need to jump
     * deleted elements.
     */
    ElementContainerIterator& incrementFast()
    {
        ++mIt;
        return *this;
    }

    /**
     * @brief Post increment function that will be called if we don't need to
     * jump deleted elements.
     */
    ElementContainerIterator postIncrementFast()
    {
        ElementContainerIterator old = *this;
        ++mIt;
        return old;
    }

    /**
     * @brief Decrement function that will be called if we need to jump deleted
     * elements.
     */
    ElementContainerIterator& decrementJump()
    {
        do {
            --mIt;
        } while (mIt != mVec->begin() && mIt->deleted());
        return *this;
    }

    /**
     * @brief Post decrement function that will be called if we need to jump
     * deleted elements.
     */
    ElementContainerIterator postDecrementJump()
    {
        ElementContainerIterator old = *this;
        do {
            --mIt;
        } while (mIt != mVec->begin() && mIt->deleted());
        return old;
    }

    /**
     * @brief Decrement function that will be called if we don't need to jump
     * deleted elements.
     */
    ElementContainerIterator& decrementFast()
    {
        --mIt;
        return *this;
    }

    /**
     * @brief Post decrement function that will be called if we don't need to
     * jump deleted elements.
     */
    ElementContainerIterator postDecrementFast()
    {
        ElementContainerIterator old = *this;
        --mIt;
        return old;
    }

    ElementContainerIterator& assignSumJump(difference_type n)
    {
        difference_type m = n;

        if (m >= 0)
            while (m--)
                this->operator++();
        else
            while (m++)
                this->operator--();
        return *this;
    }

    ElementContainerIterator& assignSumFast(difference_type n)
    {
        mIt += n;
        return *this;
    }

    difference_type diffJump(const ElementContainerIterator& oi) const
    {
        ElementContainerIterator i    = oi;
        difference_type          diff = 0;
        while (i != *this) {
            ++diff;
            ++i;
        }
        return diff;
    }

    difference_type diffFast(const ElementContainerIterator& oi) const
    {
        return this->mIt - oi.mIt;
    }
};

template<
    template<typename, typename...>
    typename Container,
    typename T,
    bool C = false>
ElementContainerIterator<Container, T, C> operator+(
    typename ElementContainerIterator<Container, T, C>::difference_type n,
    const ElementContainerIterator<Container, T, C>&                    it)
{
    return it + n;
}

template<template<typename, typename...> typename Container, typename T>
using ConstElementContainerIterator =
    ElementContainerIterator<Container, T, true>;

} // namespace vcl

#endif // VCL_MESH_ITERATORS_ELEMENT_CONTAINER_ITERATOR_H
