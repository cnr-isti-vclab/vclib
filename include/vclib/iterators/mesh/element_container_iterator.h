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

/**
 * @brief The ElementContainerIterator class is an iterator designed to iterate along a Container of
 * Elements (e.g. the containers of Vertices, Faces, ...) that will compose a Mesh.
 *
 * Since these Containers can contain deleted Elements, this iterator is designed to jump
 * automatically deleted element, if the argument jumpDeleted of the constructor is true.
 * If the jumpDeleted argument of the constructor is false, the increment and postIncrement
 * operators will be as fast as a normal increment, without any overhead for checking if the
 * iterator needs to jump elements.
 */
template<template<typename, typename...> typename Container, typename T>
class ElementContainerIterator
{
private:
	using ContIt = typename Container<T>::iterator;

public:
	using difference_type   = ptrdiff_t;
	using value_type        = T;
	using reference         = T&;
	using pointer           = T*;
	using iterator_category = std::bidirectional_iterator_tag;

	ElementContainerIterator();
	ElementContainerIterator(ContIt it, const Container<T>& vec, bool jumpDeleted = true);

	reference operator*() const;
	pointer   operator->() const;

	bool operator==(const ElementContainerIterator& oi) const;
	bool operator!=(const ElementContainerIterator& oi) const;

	ElementContainerIterator operator++();
	ElementContainerIterator operator++(int);

private:
	ElementContainerIterator incrementJump();
	ElementContainerIterator postIncrementJump();
	ElementContainerIterator incrementFast();
	ElementContainerIterator postIncrementFast();
	ElementContainerIterator decrementJump();
	ElementContainerIterator postDecrementJump();
	ElementContainerIterator decrementFast();
	ElementContainerIterator postDecrementFast();

	// pointer to increment function, assigned in the constructor
	ElementContainerIterator (ElementContainerIterator::*increment)();
	// pointer to post increment function, assigned in the constructor
	ElementContainerIterator (ElementContainerIterator::*postIncrement)();
	// pointer to decrement function, assigned in the constructor
	ElementContainerIterator (ElementContainerIterator::*decrement)();
	// pointer to post decrement function, assigned in the constructor
	ElementContainerIterator (ElementContainerIterator::*postDecrement)();

	ContIt              it;  // the actual iterator
	const Container<T>* vec = nullptr; // needed to check for end when jumping elements
};

/**
 * @brief The ConstElementContainerIterator class is the const alternative of the
 * ElementContainerIterator, that allows to iterate over a const Container and get only const
 * Elements.
 */
template<template<typename, typename...> typename Container, typename T>
class ConstElementContainerIterator
{
private:
	using ContIt = typename Container<T>::const_iterator;

public:
	using difference_type   = ptrdiff_t;
	using value_type        = T;
	using reference         = const T&;
	using pointer           = const T*;
	using iterator_category = std::bidirectional_iterator_tag;

	ConstElementContainerIterator();
	ConstElementContainerIterator(ContIt it, const Container<T>& vec, bool jumpDeleted = true);
	ConstElementContainerIterator(const ElementContainerIterator<Container, T>& it);

	reference operator*() const;
	pointer   operator->() const;

	bool operator==(const ConstElementContainerIterator& oi) const;
	bool operator!=(const ConstElementContainerIterator& oi) const;

	ConstElementContainerIterator operator++();
	ConstElementContainerIterator operator++(int);

private:
	ConstElementContainerIterator incrementJump();
	ConstElementContainerIterator postIncrementJump();
	ConstElementContainerIterator incrementFast();
	ConstElementContainerIterator postIncrementFast();
	ConstElementContainerIterator decrementJump();
	ConstElementContainerIterator postDecrementJump();
	ConstElementContainerIterator decrementFast();
	ConstElementContainerIterator postDecrementFast();

	// pointer to increment function, assigned in the constructor
	ConstElementContainerIterator (ConstElementContainerIterator::*increment)();
	// pointer to post increment function, assigned in the constructor
	ConstElementContainerIterator (ConstElementContainerIterator::*postIncrement)();
	// pointer to decrement function, assigned in the constructor
	ConstElementContainerIterator (ConstElementContainerIterator::*decrement)();
	// pointer to post decrement function, assigned in the constructor
	ConstElementContainerIterator (ConstElementContainerIterator::*postDecrement)();

	ContIt              it;  // the actual iterator
	const Container<T>* vec = nullptr; // needed to check for end when jumping elements
};

} // namespace vcl

#include "element_container_iterator.cpp"

#endif // VCL_ITERATORS_MESH_ELEMENT_CONTAINER_ITERATOR_H
