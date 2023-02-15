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
#include <vector>

namespace vcl {

namespace internal {

template<template<typename, typename...> typename Container, typename T, bool CONST = false>
class ElemContIterator
{
private:
	using ContIt = typename std::conditional_t<
		CONST,
		typename Container<T>::const_iterator,
		typename Container<T>::iterator>;

public:
	using difference_type   = ptrdiff_t;
	using value_type        = T;
	using reference         = typename std::conditional_t<CONST, const T&, T&>;
	using pointer           = typename std::conditional_t<CONST, const T*, T*>;
	using iterator_category = std::bidirectional_iterator_tag;

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

private:
	ElemContIterator incrementJump();
	ElemContIterator postIncrementJump();
	ElemContIterator incrementFast();
	ElemContIterator postIncrementFast();
	ElemContIterator decrementJump();
	ElemContIterator postDecrementJump();
	ElemContIterator decrementFast();
	ElemContIterator postDecrementFast();

	// pointer to increment function, assigned in the constructor
	ElemContIterator (ElemContIterator::*increment)();
	// pointer to post increment function, assigned in the constructor
	ElemContIterator (ElemContIterator::*postIncrement)();
	// pointer to decrement function, assigned in the constructor
	ElemContIterator (ElemContIterator::*decrement)();
	// pointer to post decrement function, assigned in the constructor
	ElemContIterator (ElemContIterator::*postDecrement)();

	ContIt              it;  // the actual iterator
	const Container<T>* vec = nullptr; // needed to check for end when jumping elements
};

} // namespace vcl::internal

template<template<typename, typename...> typename Container, typename T>
using ElementContainerIterator = internal::ElemContIterator<Container, T, false>;

template<template<typename, typename...> typename Container, typename T>
using ConstElementContainerIterator = internal::ElemContIterator<Container, T, true>;

} // namespace vcl

#include "element_container_iterator.cpp"

#endif // VCL_ITERATORS_MESH_ELEMENT_CONTAINER_ITERATOR_H
