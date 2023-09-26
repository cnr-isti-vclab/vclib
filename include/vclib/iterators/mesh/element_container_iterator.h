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

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

namespace vcl {

template<
	template<typename, typename...>
	typename Container,
	typename T,
	bool CNST = false>
class ElementContainerIterator
{
private:
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

	ElementContainerIterator();
	ElementContainerIterator(
		ContIt              it,
		const Container<T>& vec,
		bool                jumpDeleted = true);

	reference operator*() const;
	pointer   operator->() const;

	bool operator==(const ElementContainerIterator& oi) const;
	bool operator!=(const ElementContainerIterator& oi) const;

	ElementContainerIterator& operator++();
	ElementContainerIterator operator++(int);
	ElementContainerIterator& operator--();
	ElementContainerIterator operator--(int);

	ElementContainerIterator& operator+=(difference_type n);
	ElementContainerIterator& operator-=(difference_type n);

	ElementContainerIterator operator+(difference_type n) const;
	ElementContainerIterator operator-(difference_type n) const;
	difference_type  operator-(const ElementContainerIterator& oi) const;
	reference operator[](difference_type i);

	bool operator<(const ElementContainerIterator& oi) const;
	bool operator>(const ElementContainerIterator& oi) const;
	bool operator<=(const ElementContainerIterator& oi) const;
	bool operator>=(const ElementContainerIterator& oi) const;

private:
	ElementContainerIterator& incrementJump();
	ElementContainerIterator postIncrementJump();
	ElementContainerIterator& incrementFast();
	ElementContainerIterator postIncrementFast();

	ElementContainerIterator& decrementJump();
	ElementContainerIterator postDecrementJump();
	ElementContainerIterator& decrementFast();
	ElementContainerIterator postDecrementFast();

	ElementContainerIterator& assignSumJump(difference_type n);
	ElementContainerIterator& assignSumFast(difference_type n);

	difference_type diffJump(const ElementContainerIterator& oi) const;
	difference_type diffFast(const ElementContainerIterator& oi) const;

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

	ContIt              it;  // the actual iterator
	const Container<T>* vec = nullptr; // needed to check end when jump elements
};

template<template<typename, typename...> typename Container, typename T>
using ConstElementContainerIterator =
	ElementContainerIterator<Container, T, true>;

} // namespace vcl

template<template<typename, typename...> typename Container, typename T, bool C>
vcl::ElementContainerIterator<Container, T, C> operator+(
	typename vcl::ElementContainerIterator<Container, T, C>::difference_type n,
	const vcl::ElementContainerIterator<Container, T, C>& it);

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
auto ElementContainerIterator<Container, T, CNST>::operator++() -> ElementContainerIterator&
{
	return (this->*increment)();
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
auto ElementContainerIterator<Container, T, CNST>::operator++(int) -> ElementContainerIterator
{
	return (this->*postIncrement)();
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
auto ElementContainerIterator<Container, T, CNST>::operator--() -> ElementContainerIterator&
{
	return (this->*decrement)();
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
auto ElementContainerIterator<Container, T, CNST>::operator--(int) -> ElementContainerIterator
{
	return (this->*postDecrement)();
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
auto ElementContainerIterator<Container, T, CNST>::operator+=(difference_type n) -> ElementContainerIterator&
{
	return (this->*assignSum)(n);
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
auto ElementContainerIterator<Container, T, CNST>::operator-=(difference_type n) -> ElementContainerIterator&
{
	return (this->*assignSum)(-n);
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
auto ElementContainerIterator<Container, T, CNST>::operator+(difference_type n) const -> ElementContainerIterator
{
	ElementContainerIterator temp = *this;
	return temp += n;
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
auto ElementContainerIterator<Container, T, CNST>::operator-(difference_type n) const -> ElementContainerIterator
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
auto ElementContainerIterator<Container, T, CNST>::incrementJump() -> ElementContainerIterator&
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
auto ElementContainerIterator<Container, T, CNST>::postIncrementJump() -> ElementContainerIterator
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
auto ElementContainerIterator<Container, T, CNST>::incrementFast() -> ElementContainerIterator&
{
	++it;
	return *this;
}

/**
 * @brief Post increment function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CNST>
auto ElementContainerIterator<Container, T, CNST>::postIncrementFast() -> ElementContainerIterator
{
	ElementContainerIterator old = *this;
	++it;
	return old;
}

/**
 * @brief Decrement function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CNST>
auto ElementContainerIterator<Container, T, CNST>::decrementJump() -> ElementContainerIterator&
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
auto ElementContainerIterator<Container, T, CNST>::postDecrementJump() -> ElementContainerIterator
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
auto ElementContainerIterator<Container, T, CNST>::decrementFast() -> ElementContainerIterator&
{
	--it;
	return *this;
}

/**
 * @brief Post decrement function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CNST>
auto ElementContainerIterator<Container, T, CNST>::postDecrementFast() -> ElementContainerIterator
{
	ElementContainerIterator old = *this;
	--it;
	return old;
}

template<template<typename, typename...> typename Container, typename T, bool CNST>
auto ElementContainerIterator<Container, T, CNST>::assignSumJump(difference_type n) -> ElementContainerIterator&
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
auto ElementContainerIterator<Container, T, CNST>::assignSumFast(difference_type n) -> ElementContainerIterator&
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

#endif // VCL_ITERATORS_MESH_ELEMENT_CONTAINER_ITERATOR_H
