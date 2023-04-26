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

template<template<typename, typename...> typename Container, typename T, bool CNST = false>
class ElementContainerIterator
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

	ElementContainerIterator();
	ElementContainerIterator(ContIt it, const Container<T>& vec, bool jumpDeleted = true);

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
	ElementContainerIterator& (ElementContainerIterator::*assignSum)(difference_type);
	// pointer to difference between iterators function, assigned in the constructor
	difference_type (ElementContainerIterator::*diff)(const ElementContainerIterator& oi) const;

	ContIt              it;  // the actual iterator
	const Container<T>* vec = nullptr; // needed to check for end when jumping elements
};

template<template<typename, typename...> typename Container, typename T>
using ConstElementContainerIterator = ElementContainerIterator<Container, T, true>;

} // namespace vcl

template<template<typename, typename...> typename Container, typename T, bool C>
vcl::ElementContainerIterator<Container, T, C> operator+(
	typename vcl::ElementContainerIterator<Container, T, C>::difference_type n,
	const vcl::ElementContainerIterator<Container, T, C>& it);

#include "element_container_iterator.cpp"

#endif // VCL_MESH_ITERATORS_ELEMENT_CONTAINER_ITERATOR_H
