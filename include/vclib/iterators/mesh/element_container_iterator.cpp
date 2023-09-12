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

#include "element_container_iterator.h"
#include <utility>

namespace vcl {

template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>::ElementContainerIterator()
{
	increment     = &ElementContainerIterator::incrementFast;
	postIncrement = &ElementContainerIterator::postIncrementFast;
	decrement     = &ElementContainerIterator::decrementFast;
	postDecrement = &ElementContainerIterator::postDecrementFast;
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>::ElementContainerIterator(
	ContIt              it,
	const Container<T>& vec,
	bool                jumpDeleted) :
		it(it), vec(&vec)
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

template<template<typename, typename...> typename Container, typename T, bool CNST>
auto ElementContainerIterator<Container, T, CNST>::operator*() const -> reference
{
	return *it;
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
auto ElementContainerIterator<Container, T, CNST>::operator->() const -> pointer
{
	return &*it;
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
bool ElementContainerIterator<Container, T, CNST>::operator==(
	const ElementContainerIterator& oi) const
{
	return it == oi.it;
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
bool ElementContainerIterator<Container, T, CNST>::operator!=(
	const ElementContainerIterator& oi) const
{
	return it != oi.it;
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>&
ElementContainerIterator<Container, T, CNST>::operator++()
{
	return (this->*increment)();
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>
ElementContainerIterator<Container, T, CNST>::operator++(int)
{
	return (this->*postIncrement)();
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>&
ElementContainerIterator<Container, T, CNST>::operator--()
{
	return (this->*decrement)();
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>
ElementContainerIterator<Container, T, CNST>::operator--(int)
{
	return (this->*postDecrement)();
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>&
ElementContainerIterator<Container, T, CNST>::operator+=(difference_type n)
{
	return (this->*assignSum)(n);
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>&
ElementContainerIterator<Container, T, CNST>::operator-=(difference_type n)
{
	return (this->*assignSum)(-n);
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>
ElementContainerIterator<Container, T, CNST>::operator+(difference_type n) const
{
	ElementContainerIterator temp = *this;
	return temp += n;
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>
ElementContainerIterator<Container, T, CNST>::operator-(difference_type n) const
{
	ElementContainerIterator temp = *this;
	return temp += -n;
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
auto ElementContainerIterator<Container, T, CNST>::operator-(const ElementContainerIterator& oi) const -> difference_type
{
	return (this->*diff)(oi);
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
auto ElementContainerIterator<Container, T, CNST>::operator[](difference_type i) -> reference
{
	return *(*this + i);
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
bool ElementContainerIterator<Container, T, CNST>::operator<(
	const ElementContainerIterator& oi) const
{
	return it < oi.it;
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
bool ElementContainerIterator<Container, T, CNST>::operator>(
	const ElementContainerIterator& oi) const
{
	return it > oi.it;
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
bool ElementContainerIterator<Container, T, CNST>::operator<=(
	const ElementContainerIterator& oi) const
{
	return it <= oi.it;
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
bool ElementContainerIterator<Container, T, CNST>::operator>=(
	const ElementContainerIterator& oi) const
{
	return it >= oi.it;
}

/**
 * @brief Increment function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>&
ElementContainerIterator<Container, T, CNST>::incrementJump()
{
	do {
		++it;
	} while (it != vec->end() && it->deleted());
	return *this;
}

/**
 * @brief Post increment function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>
ElementContainerIterator<Container, T, CNST>::postIncrementJump()
{
	ElementContainerIterator old = *this;
	do {
		++it;
	} while (it != vec->end() && it->deleted());
	return old;
}

/**
 * @brief Increment function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>&
ElementContainerIterator<Container, T, CNST>::incrementFast()
{
	++it;
	return *this;
}

/**
 * @brief Post increment function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>
ElementContainerIterator<Container, T, CNST>::postIncrementFast()
{
	ElementContainerIterator old = *this;
	++it;
	return old;
}

/**
 * @brief Decrement function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>&
ElementContainerIterator<Container, T, CNST>::decrementJump()
{
	do {
		--it;
	} while (it != vec->begin() && it->deleted());
	return *this;
}

/**
 * @brief Post decrement function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>
ElementContainerIterator<Container, T, CNST>::postDecrementJump()
{
	ElementContainerIterator old = *this;
	do {
		--it;
	} while (it != vec->begin() && it->deleted());
	return old;
}

/**
 * @brief Decrement function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>&
ElementContainerIterator<Container, T, CNST>::decrementFast()
{
	--it;
	return *this;
}

/**
 * @brief Post decrement function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>
ElementContainerIterator<Container, T, CNST>::postDecrementFast()
{
	ElementContainerIterator old = *this;
	--it;
	return old;
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>&
ElementContainerIterator<Container, T, CNST>::assignSumJump(difference_type n)
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

template<template<typename, typename...> typename Container, typename T, bool CNST>
ElementContainerIterator<Container, T, CNST>&
ElementContainerIterator<Container, T, CNST>::assignSumFast(difference_type n)
{
	it += n;
	return *this;
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
auto ElementContainerIterator<Container, T, CNST>::diffJump(const ElementContainerIterator& oi) const -> difference_type
{
	ElementContainerIterator i    = oi;
	difference_type          diff = 0;
	while (i != *this) {
		++diff;
		++i;
	}
	return diff;
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
auto ElementContainerIterator<Container, T, CNST>::diffFast(const ElementContainerIterator& oi) const -> difference_type
{
	return this->it - oi.it;
}

} // namespace vcl

template<template<typename, typename...> typename Container, typename T, bool C>
vcl::ElementContainerIterator<Container, T, C> operator+(
	typename vcl::ElementContainerIterator<Container, T, C>::difference_type n,
	const vcl::ElementContainerIterator<Container, T, C>&                    it)
{
	return it + n;
}
