/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#ifndef VCL_ITERATORS_CONTAINER_ITERATOR_H
#define VCL_ITERATORS_CONTAINER_ITERATOR_H

#include <cstddef>
#include <vector>

namespace vcl {

/**
 * @brief The ContainerIterator class is an iterator designed to iterate along a Container of
 * Elements (e.g. the containers of Vertices, Faces, ...) that will compose a Mesh.
 *
 * Since these Containers can contain deleted Elements, this iterator is designed to jump
 * automatically deleted element, if the argument jumpDeleted of the constructor is true.
 * If the jumpDeleted argument of the constructor is false, the increment and postIncrement
 * operators will be as fast as a normal increment, without any overhead for checking if the
 * iterator needs to jump elements.
 */
template<template<typename, typename...> class Container, typename T>
class ContainerIterator
{
private:
	using VecIt = typename std::vector<T>::iterator;

public:
	using difference_type   = ptrdiff_t;
	using value_type        = T;
	using reference         = T&;
	using pointer           = T*;
	using iterator_category = std::forward_iterator_tag;

	ContainerIterator();
	ContainerIterator(VecIt it, const Container<T>& vec, bool jumpDeleted = true);

	reference operator*() const;
	pointer   operator->() const;

	bool operator==(const ContainerIterator& oi) const;
	bool operator!=(const ContainerIterator& oi) const;

	ContainerIterator operator++();
	ContainerIterator operator++(int);

private:
	ContainerIterator incrementJump();
	ContainerIterator postIncrementJump();
	ContainerIterator incrementFast();
	ContainerIterator postIncrementFast();

	// pointer to increment function, assigned in the constructor
	ContainerIterator (ContainerIterator::*increment)();
	// pointer to post increment function, assigned in the constructor
	ContainerIterator (ContainerIterator::*postIncrement)();

	VecIt               it;  // the actual iterator
	const Container<T>* vec = nullptr; // needed to check for end when jumping elements
};

/**
 * @brief The ConstContainerIterator class is the const alternative of the ContainerIterator, that
 * allows to iterate over a const Container and get only const Elements.
 */
template<template<typename, typename...> class Container, typename T>
class ConstContainerIterator
{
private:
	using VecIt = typename std::vector<T>::const_iterator;

public:
	using difference_type   = ptrdiff_t;
	using value_type        = T;
	using reference         = const T&;
	using pointer           = const T*;
	using iterator_category = std::forward_iterator_tag;

	ConstContainerIterator();
	ConstContainerIterator(VecIt it, const Container<T>& vec, bool jumpDeleted = true);
	ConstContainerIterator(const ContainerIterator<Container, T>& it);

	reference operator*() const;
	pointer   operator->() const;

	bool operator==(const ConstContainerIterator& oi) const;
	bool operator!=(const ConstContainerIterator& oi) const;

	ConstContainerIterator operator++();
	ConstContainerIterator operator++(int);

private:
	ConstContainerIterator incrementJump();
	ConstContainerIterator postIncrementJump();
	ConstContainerIterator incrementFast();
	ConstContainerIterator postIncrementFast();

	// pointer to increment function, assigned in the constructor
	ConstContainerIterator (ConstContainerIterator::*increment)();
	// pointer to post increment function, assigned in the constructor
	ConstContainerIterator (ConstContainerIterator::*postIncrement)();

	VecIt               it;  // the actual iterator
	const Container<T>* vec = nullptr; // needed to check for end when jumping elements
};

} // namespace vcl

#include "container_iterator.cpp"

#endif // VCL_ITERATORS_CONTAINER_ITERATOR_H
