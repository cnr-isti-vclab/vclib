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

#ifndef VCL_MESH_ITERATORS_ELEMENT_CONTAINER_ITERATOR_H
#define VCL_MESH_ITERATORS_ELEMENT_CONTAINER_ITERATOR_H

#include <cstddef>
#include <vector>

namespace vcl {

namespace internal {

template<template<typename, typename...> typename Container, typename T, bool CNST = false>
class ElemContIterator
{
private:
	using ContIt = typename std::conditional_t<
		CNST,
		typename Container<T>::const_iterator,
		typename Container<T>::iterator>;

public:
	using difference_type   = ptrdiff_t;
	using value_type        = T;
	using reference         = typename std::conditional_t<CNST, const T&, T&>;
	using pointer           = typename std::conditional_t<CNST, const T*, T*>;
	using iterator_category = std::random_access_iterator_tag;

	ElemContIterator();
	ElemContIterator(ContIt it, const Container<T>& vec, bool jumpDeleted = true);

	reference operator*() const;
	pointer   operator->() const;

	bool operator==(const ElemContIterator& oi) const;
	bool operator!=(const ElemContIterator& oi) const;

	ElemContIterator operator++();
	ElemContIterator operator++(int);
	ElemContIterator operator--();
	ElemContIterator operator--(int);

	ElemContIterator& operator+=(difference_type n);
	ElemContIterator& operator-=(difference_type n);

	ElemContIterator operator+(difference_type n) const;
	ElemContIterator operator-(difference_type n) const;
	difference_type  operator-(const ElemContIterator& oi) const;
	reference operator[](difference_type i);

	bool operator<(const ElemContIterator& oi) const;
	bool operator>(const ElemContIterator& oi) const;
	bool operator<=(const ElemContIterator& oi) const;
	bool operator>=(const ElemContIterator& oi) const;

private:
	ElemContIterator incrementJump();
	ElemContIterator postIncrementJump();
	ElemContIterator incrementFast();
	ElemContIterator postIncrementFast();

	ElemContIterator decrementJump();
	ElemContIterator postDecrementJump();
	ElemContIterator decrementFast();
	ElemContIterator postDecrementFast();

	ElemContIterator& assignSumJump(difference_type n);
	ElemContIterator& assignSumFast(difference_type n);

	difference_type diffJump(const ElemContIterator& oi) const;
	difference_type diffFast(const ElemContIterator& oi) const;

	// pointer to increment function, assigned in the constructor
	ElemContIterator (ElemContIterator::*increment)();
	// pointer to post increment function, assigned in the constructor
	ElemContIterator (ElemContIterator::*postIncrement)();
	// pointer to decrement function, assigned in the constructor
	ElemContIterator (ElemContIterator::*decrement)();
	// pointer to post decrement function, assigned in the constructor
	ElemContIterator (ElemContIterator::*postDecrement)();
	// pointer to assignment sum function, assigned in the constructor
	ElemContIterator& (ElemContIterator::*assignSum)(difference_type);
	// pointer to difference between iterators function, assigned in the constructor
	difference_type (ElemContIterator::*diff)(const ElemContIterator& oi) const;

	ContIt              it;  // the actual iterator
	const Container<T>* vec = nullptr; // needed to check for end when jumping elements
};

} // namespace vcl::internal

template<template<typename, typename...> typename Container, typename T>
using ElementContainerIterator = internal::ElemContIterator<Container, T, false>;

template<template<typename, typename...> typename Container, typename T>
using ConstElementContainerIterator = internal::ElemContIterator<Container, T, true>;

} // namespace vcl

template<template<typename, typename...> typename Container, typename T, bool C>
vcl::internal::ElemContIterator<Container, T, C> operator+(
	typename vcl::internal::ElemContIterator<Container, T, C>::difference_type n,
	const vcl::internal::ElemContIterator<Container, T, C>& it);

#include "element_container_iterator.cpp"

#endif // VCL_MESH_ITERATORS_ELEMENT_CONTAINER_ITERATOR_H
