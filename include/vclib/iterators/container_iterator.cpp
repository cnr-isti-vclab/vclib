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

#include "container_iterator.h"

namespace vcl {

/*
 * ContainerIterator
 */

template<template<typename, typename...> class Container, typename T>
ContainerIterator<Container, T>::ContainerIterator()
{
	increment     = &ContainerIterator::incrementFast;
	postIncrement = &ContainerIterator::incrementFast;
}

template<template<typename, typename...> class Container, typename T>
ContainerIterator<Container, T>::ContainerIterator(
	VecIt               it,
	const Container<T>& vec,
	bool                jumpDeleted) :
		it(it), vec(&vec)
{
	if (jumpDeleted) {
		increment     = &ContainerIterator::incrementJump;
		postIncrement = &ContainerIterator::postIncrementJump;
	}
	else {
		increment     = &ContainerIterator::incrementFast;
		postIncrement = &ContainerIterator::incrementFast;
	}
}

template<template<typename, typename...> class Container, typename T>
typename ContainerIterator<Container, T>::reference
ContainerIterator<Container, T>::operator*() const
{
	return *it;
}

template<template<typename, typename...> class Container, typename T>
typename ContainerIterator<Container, T>::pointer
ContainerIterator<Container, T>::operator->() const
{
	return &*it;
}

template<template<typename, typename...> class Container, typename T>
bool ContainerIterator<Container, T>::operator==(const ContainerIterator& oi) const
{
	return it == oi.it;
}

template<template<typename, typename...> class Container, typename T>
bool ContainerIterator<Container, T>::operator!=(const ContainerIterator& oi) const
{
	return it != oi.it;
}

template<template<typename, typename...> class Container, typename T>
ContainerIterator<Container, T> ContainerIterator<Container, T>::operator++()
{
	return (this->*increment)();
}

template<template<typename, typename...> class Container, typename T>
ContainerIterator<Container, T> ContainerIterator<Container, T>::operator++(int)
{
	return (this->*postIncrement)();
}

/**
 * @brief Increment function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> class Container, typename T>
ContainerIterator<Container, T> ContainerIterator<Container, T>::incrementJump()
{
	do {
		++it;
	} while (it != vec->end() && it->isDeleted());
	return *this;
}

/**
 * @brief Post increment function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> class Container, typename T>
ContainerIterator<Container, T> ContainerIterator<Container, T>::postIncrementJump()
{
	ContainerIterator old = *this;
	do {
		++it;
	} while (it != vec->end() && it->isDeleted());
	return old;
}

/**
 * @brief Increment function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> class Container, typename T>
ContainerIterator<Container, T> ContainerIterator<Container, T>::incrementFast()
{
	++it;
	return *this;
}

/**
 * @brief Post increment function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> class Container, typename T>
ContainerIterator<Container, T> ContainerIterator<Container, T>::postIncrementFast()
{
	ContainerIterator old = *this;
	++it;
	return old;
}

/*
 * ConstContainerIterator
 */

template<template<typename, typename...> class Container, typename T>
ConstContainerIterator<Container, T>::ConstContainerIterator()
{
	increment     = &ConstContainerIterator::incrementFast;
	postIncrement = &ConstContainerIterator::incrementFast;
}

template<template<typename, typename...> class Container, typename T>
ConstContainerIterator<Container, T>::ConstContainerIterator(
	VecIt               it,
	const Container<T>& vec,
	bool                jumpDeleted) :
		it(it), vec(&vec)
{
	if (jumpDeleted) {
		increment     = &ConstContainerIterator::incrementJump;
		postIncrement = &ConstContainerIterator::postIncrementJump;
	}
	else {
		increment     = &ConstContainerIterator::incrementFast;
		postIncrement = &ConstContainerIterator::incrementFast;
	}
}

template<template<typename, typename...> class Container, typename T>
ConstContainerIterator<Container, T>::ConstContainerIterator(
	const ContainerIterator<Container, T>& it) :
		ConstContainerIterator(it.it, it.vec, it.jumpDeleted)
{
}

template<template<typename, typename...> class Container, typename T>
typename ConstContainerIterator<Container, T>::reference
ConstContainerIterator<Container, T>::operator*() const
{
	return *it;
}

template<template<typename, typename...> class Container, typename T>
typename ConstContainerIterator<Container, T>::pointer
ConstContainerIterator<Container, T>::operator->() const
{
	return &*it;
}

template<template<typename, typename...> class Container, typename T>
bool ConstContainerIterator<Container, T>::operator==(const ConstContainerIterator& oi) const
{
	return it == oi.it;
}

template<template<typename, typename...> class Container, typename T>
bool ConstContainerIterator<Container, T>::operator!=(const ConstContainerIterator& oi) const
{
	return it != oi.it;
}

template<template<typename, typename...> class Container, typename T>
ConstContainerIterator<Container, T> ConstContainerIterator<Container, T>::operator++()
{
	return (this->*increment)();
}

template<template<typename, typename...> class Container, typename T>
ConstContainerIterator<Container, T> ConstContainerIterator<Container, T>::operator++(int)
{
	return (this->*postIncrement)();
}

/**
 * @brief Increment function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> class Container, typename T>
ConstContainerIterator<Container, T> ConstContainerIterator<Container, T>::incrementJump()
{
	do {
		++it;
	} while (it != vec->end() && it->isDeleted());
	return *this;
}

/**
 * @brief Post increment function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> class Container, typename T>
ConstContainerIterator<Container, T> ConstContainerIterator<Container, T>::postIncrementJump()
{
	ConstContainerIterator old = *this;
	do {
		++it;
	} while (it != vec->end() && it->isDeleted());
	return old;
}

/**
 * @brief Increment function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> class Container, typename T>
ConstContainerIterator<Container, T> ConstContainerIterator<Container, T>::incrementFast()
{
	++it;
	return *this;
}

/**
 * @brief Post increment function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> class Container, typename T>
ConstContainerIterator<Container, T> ConstContainerIterator<Container, T>::postIncrementFast()
{
	ContainerIterator old = *this;
	++it;
	return old;
}

} // namespace vcl
