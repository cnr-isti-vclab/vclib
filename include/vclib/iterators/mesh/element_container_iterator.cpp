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

namespace vcl {

/*
 * ContainerIterator
 */

template<template<typename, typename...> typename Container, typename T>
ElementContainerIterator<Container, T>::ElementContainerIterator()
{
	increment     = &ElementContainerIterator::incrementFast;
	postIncrement = &ElementContainerIterator::postIncrementFast;
	decrement     = &ElementContainerIterator::decrementFast;
	postDecrement = &ElementContainerIterator::postDecrementFast;
}

template<template<typename, typename...> typename Container, typename T>
ElementContainerIterator<Container, T>::ElementContainerIterator(
	ContIt               it,
	const Container<T>& vec,
	bool                jumpDeleted) :
		it(it), vec(&vec)
{
	if (jumpDeleted) {
		increment     = &ElementContainerIterator::incrementJump;
		postIncrement = &ElementContainerIterator::postIncrementJump;
		decrement     = &ElementContainerIterator::decrementJump;
		postDecrement = &ElementContainerIterator::postDecrementJump;
	}
	else {
		increment     = &ElementContainerIterator::incrementFast;
		postIncrement = &ElementContainerIterator::postIncrementFast;
		decrement     = &ElementContainerIterator::decrementFast;
		postDecrement = &ElementContainerIterator::postDecrementFast;
	}
}

template<template<typename, typename...> typename Container, typename T>
typename ElementContainerIterator<Container, T>::reference
ElementContainerIterator<Container, T>::operator*() const
{
	return *it;
}

template<template<typename, typename...> typename Container, typename T>
typename ElementContainerIterator<Container, T>::pointer
ElementContainerIterator<Container, T>::operator->() const
{
	return &*it;
}

template<template<typename, typename...> typename Container, typename T>
bool ElementContainerIterator<Container, T>::operator==(const ElementContainerIterator& oi) const
{
	return it == oi.it;
}

template<template<typename, typename...> typename Container, typename T>
bool ElementContainerIterator<Container, T>::operator!=(const ElementContainerIterator& oi) const
{
	return it != oi.it;
}

template<template<typename, typename...> typename Container, typename T>
ElementContainerIterator<Container, T> ElementContainerIterator<Container, T>::operator++()
{
	return (this->*increment)();
}

template<template<typename, typename...> typename Container, typename T>
ElementContainerIterator<Container, T> ElementContainerIterator<Container, T>::operator++(int)
{
	return (this->*postIncrement)();
}

/**
 * @brief Increment function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T>
ElementContainerIterator<Container, T> ElementContainerIterator<Container, T>::incrementJump()
{
	do {
		++it;
	} while (it != vec->end() && it->isDeleted());
	return *this;
}

/**
 * @brief Post increment function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T>
ElementContainerIterator<Container, T> ElementContainerIterator<Container, T>::postIncrementJump()
{
	ElementContainerIterator old = *this;
	do {
		++it;
	} while (it != vec->end() && it->isDeleted());
	return old;
}

/**
 * @brief Increment function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T>
ElementContainerIterator<Container, T> ElementContainerIterator<Container, T>::incrementFast()
{
	++it;
	return *this;
}

/**
 * @brief Post increment function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T>
ElementContainerIterator<Container, T> ElementContainerIterator<Container, T>::postIncrementFast()
{
	ElementContainerIterator old = *this;
	++it;
	return old;
}

/**
 * @brief Decrement function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T>
ElementContainerIterator<Container, T> ElementContainerIterator<Container, T>::decrementJump()
{
	do {
		--it;
	} while (it != vec->begin() && it->isDeleted());
	return *this;
}

/**
 * @brief Post decrement function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T>
ElementContainerIterator<Container, T> ElementContainerIterator<Container, T>::postDecrementJump()
{
	ElementContainerIterator old = *this;
	do {
		--it;
	} while (it != vec->begin() && it->isDeleted());
	return old;
}

/**
 * @brief Decrement function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T>
ElementContainerIterator<Container, T> ElementContainerIterator<Container, T>::decrementFast()
{
	--it;
	return *this;
}

/**
 * @brief Post decrement function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T>
ElementContainerIterator<Container, T> ElementContainerIterator<Container, T>::postDecrementFast()
{
	ElementContainerIterator old = *this;
	--it;
	return old;
}

/*
 * ConstContainerIterator
 */

template<template<typename, typename...> typename Container, typename T>
ConstElementContainerIterator<Container, T>::ConstElementContainerIterator()
{
	increment     = &ConstElementContainerIterator::incrementFast;
	postIncrement = &ConstElementContainerIterator::postIncrementFast;
	decrement     = &ConstElementContainerIterator::decrementFast;
	postDecrement = &ConstElementContainerIterator::postDecrementFast;
}

template<template<typename, typename...> typename Container, typename T>
ConstElementContainerIterator<Container, T>::ConstElementContainerIterator(
	ContIt               it,
	const Container<T>& vec,
	bool                jumpDeleted) :
		it(it), vec(&vec)
{
	if (jumpDeleted) {
		increment     = &ConstElementContainerIterator::incrementJump;
		postIncrement = &ConstElementContainerIterator::postIncrementJump;
		decrement     = &ConstElementContainerIterator::decrementJump;
		postDecrement = &ConstElementContainerIterator::postDecrementJump;
	}
	else {
		increment     = &ConstElementContainerIterator::incrementFast;
		postIncrement = &ConstElementContainerIterator::postIncrementFast;
		decrement     = &ConstElementContainerIterator::decrementFast;
		postDecrement = &ConstElementContainerIterator::postDecrementFast;
	}
}

template<template<typename, typename...> typename Container, typename T>
ConstElementContainerIterator<Container, T>::ConstElementContainerIterator(
	const ElementContainerIterator<Container, T>& it) :
		ConstElementContainerIterator(it.it, it.vec, it.jumpDeleted)
{
}

template<template<typename, typename...> typename Container, typename T>
typename ConstElementContainerIterator<Container, T>::reference
ConstElementContainerIterator<Container, T>::operator*() const
{
	return *it;
}

template<template<typename, typename...> typename Container, typename T>
typename ConstElementContainerIterator<Container, T>::pointer
ConstElementContainerIterator<Container, T>::operator->() const
{
	return &*it;
}

template<template<typename, typename...> typename Container, typename T>
bool ConstElementContainerIterator<Container, T>::operator==(const ConstElementContainerIterator& oi) const
{
	return it == oi.it;
}

template<template<typename, typename...> typename Container, typename T>
bool ConstElementContainerIterator<Container, T>::operator!=(const ConstElementContainerIterator& oi) const
{
	return it != oi.it;
}

template<template<typename, typename...> typename Container, typename T>
ConstElementContainerIterator<Container, T> ConstElementContainerIterator<Container, T>::operator++()
{
	return (this->*increment)();
}

template<template<typename, typename...> typename Container, typename T>
ConstElementContainerIterator<Container, T> ConstElementContainerIterator<Container, T>::operator++(int)
{
	return (this->*postIncrement)();
}

/**
 * @brief Increment function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T>
ConstElementContainerIterator<Container, T> ConstElementContainerIterator<Container, T>::incrementJump()
{
	do {
		++it;
	} while (it != vec->end() && it->isDeleted());
	return *this;
}

/**
 * @brief Post increment function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T>
ConstElementContainerIterator<Container, T> ConstElementContainerIterator<Container, T>::postIncrementJump()
{
	ConstElementContainerIterator old = *this;
	do {
		++it;
	} while (it != vec->end() && it->isDeleted());
	return old;
}

/**
 * @brief Increment function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T>
ConstElementContainerIterator<Container, T> ConstElementContainerIterator<Container, T>::incrementFast()
{
	++it;
	return *this;
}

/**
 * @brief Post increment function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T>
ConstElementContainerIterator<Container, T> ConstElementContainerIterator<Container, T>::postIncrementFast()
{
	ConstElementContainerIterator old = *this;
	++it;
	return old;
}

/**
 * @brief Decrement function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T>
ConstElementContainerIterator<Container, T> ConstElementContainerIterator<Container, T>::decrementJump()
{
	do {
		--it;
	} while (it != vec->begin() && it->isDeleted());
	return *this;
}

/**
 * @brief Post decrement function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T>
ConstElementContainerIterator<Container, T> ConstElementContainerIterator<Container, T>::postDecrementJump()
{
	ConstElementContainerIterator old = *this;
	do {
		--it;
	} while (it != vec->begin() && it->isDeleted());
	return old;
}

/**
 * @brief Decrement function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T>
ConstElementContainerIterator<Container, T> ConstElementContainerIterator<Container, T>::decrementFast()
{
	--it;
	return *this;
}

/**
 * @brief Post decrement function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T>
ConstElementContainerIterator<Container, T> ConstElementContainerIterator<Container, T>::postDecrementFast()
{
	ConstElementContainerIterator old = *this;
	--it;
	return old;
}

} // namespace vcl
