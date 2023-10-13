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

#ifndef VCL_ITERATORS_MESH_ELEMENT_CONTAINER_ITERATOR_H
#define VCL_ITERATORS_MESH_ELEMENT_CONTAINER_ITERATOR_H

#include <cstddef>
#include <utility>
#include <vector>

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
    ContIt it; // the actual iterator

    const Container<T>* vec = nullptr; // needed to check end when jump elements

    /* Pointers to functions */

    // pointer to increment function, assigned in the constructor
    ElementContainerIterator& (ElementContainerIterator::*increment)();
    // pointer to post increment function, assigned in the constructor
    ElementContainerIterator (ElementContainerIterator::*postIncrement)();
    // pointer to decrement function, assigned in the constructor
    ElementContainerIterator& (ElementContainerIterator::*decrement)();
    // pointer to post decrement function, assigned in the constructor
    ElementContainerIterator (ElementContainerIterator::*postDecrement)();
    // pointer to assignment sum function, assigned in the constructor
    ElementContainerIterator& (ElementContainerIterator::*assignSum)(
        difference_type);
    // pointer to diff between iterators function, assigned in the constructor
    difference_type (ElementContainerIterator::*diff)(
        const ElementContainerIterator& oi) const;

public:
    ElementContainerIterator()
    {
        increment     = &ElementContainerIterator::incrementFast;
        postIncrement = &ElementContainerIterator::postIncrementFast;
        decrement     = &ElementContainerIterator::decrementFast;
        postDecrement = &ElementContainerIterator::postDecrementFast;
    }

    ElementContainerIterator(
        ContIt              it,
        const Container<T>& vec,
        bool                jumpDeleted = true) :
            it(it),
            vec(&vec)
    {
        if (jumpDeleted) {
            increment     = &ElementContainerIterator::incrementJump;
            postIncrement = &ElementContainerIterator::postIncrementJump;
            decrement     = &ElementContainerIterator::decrementJump;
            postDecrement = &ElementContainerIterator::postDecrementJump;
            assignSum     = &ElementContainerIterator::assignSumJump;
            diff          = &ElementContainerIterator::diffJump;
        }
        else {
            increment     = &ElementContainerIterator::incrementFast;
            postIncrement = &ElementContainerIterator::postIncrementFast;
            decrement     = &ElementContainerIterator::decrementFast;
            postDecrement = &ElementContainerIterator::postDecrementFast;
            assignSum     = &ElementContainerIterator::assignSumFast;
            diff          = &ElementContainerIterator::diffFast;
        }
    }

    reference operator*() const { return *it; }

    pointer operator->() const { return &*it; }

    bool operator==(const ElementContainerIterator& oi) const
    {
        return it == oi.it;
    }

    bool operator!=(const ElementContainerIterator& oi) const
    {
        return it != oi.it;
    }

    ElementContainerIterator& operator++() { return (this->*increment)(); }

    ElementContainerIterator operator++(int)
    {
        return (this->*postIncrement)();
    }

    ElementContainerIterator& operator--() { return (this->*decrement)(); }

    ElementContainerIterator operator--(int)
    {
        return (this->*postDecrement)();
    }

    ElementContainerIterator& operator+=(difference_type n)
    {
        return (this->*assignSum)(n);
    }

    ElementContainerIterator& operator-=(difference_type n)
    {
        return (this->*assignSum)(-n);
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
        return (this->*diff)(oi);
    }

    reference operator[](difference_type i) { return *(*this + i); }

    bool operator<(const ElementContainerIterator& oi) const
    {
        return it < oi.it;
    }

    bool operator>(const ElementContainerIterator& oi) const
    {
        return it > oi.it;
    }

    bool operator<=(const ElementContainerIterator& oi) const
    {
        return it <= oi.it;
    }

    bool operator>=(const ElementContainerIterator& oi) const
    {
        return it >= oi.it;
    }

private:
    /**
     * @brief Increment function that will be called if we need to jump deleted
     * elements.
     */
    ElementContainerIterator& incrementJump()
    {
        do {
            ++it;
        } while (it != vec->end() && it->deleted());
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
            ++it;
        } while (it != vec->end() && it->deleted());
        return old;
    }

    /**
     * @brief Increment function that will be called if we don't need to jump
     * deleted elements.
     */
    ElementContainerIterator& incrementFast()
    {
        ++it;
        return *this;
    }

    /**
     * @brief Post increment function that will be called if we don't need to
     * jump deleted elements.
     */
    ElementContainerIterator postIncrementFast()
    {
        ElementContainerIterator old = *this;
        ++it;
        return old;
    }

    /**
     * @brief Decrement function that will be called if we need to jump deleted
     * elements.
     */
    ElementContainerIterator& decrementJump()
    {
        do {
            --it;
        } while (it != vec->begin() && it->deleted());
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
            --it;
        } while (it != vec->begin() && it->deleted());
        return old;
    }

    /**
     * @brief Decrement function that will be called if we don't need to jump
     * deleted elements.
     */
    ElementContainerIterator& decrementFast()
    {
        --it;
        return *this;
    }

    /**
     * @brief Post decrement function that will be called if we don't need to
     * jump deleted elements.
     */
    ElementContainerIterator postDecrementFast()
    {
        ElementContainerIterator old = *this;
        --it;
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
        it += n;
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
        return this->it - oi.it;
    }
};

template<template<typename, typename...> typename Container, typename T>
using ConstElementContainerIterator =
    ElementContainerIterator<Container, T, true>;

} // namespace vcl

template<template<typename, typename...> typename Container, typename T, bool C>
vcl::ElementContainerIterator<Container, T, C> operator+(
    typename vcl::ElementContainerIterator<Container, T, C>::difference_type n,
    const vcl::ElementContainerIterator<Container, T, C>&                    it)
{
    return it + n;
}

#endif // VCL_ITERATORS_MESH_ELEMENT_CONTAINER_ITERATOR_H
