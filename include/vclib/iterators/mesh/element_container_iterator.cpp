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

namespace vcl::internal {

template<template<typename, typename...> typename Container, typename T, bool CONST>
ElemContIterator<Container, T, CONST>::ElemContIterator()
{
	increment     = &ElemContIterator::incrementFast;
	postIncrement = &ElemContIterator::postIncrementFast;
	decrement     = &ElemContIterator::decrementFast;
	postDecrement = &ElemContIterator::postDecrementFast;
}

template<template<typename, typename...> typename Container, typename T, bool CONST>
ElemContIterator<Container, T, CONST>::ElemContIterator(
	ContIt               it,
	const Container<T>& vec,
	bool                jumpDeleted) :
		it(it), vec(&vec)
{
	if (jumpDeleted) {
		increment     = &ElemContIterator::incrementJump;
		postIncrement = &ElemContIterator::postIncrementJump;
		decrement     = &ElemContIterator::decrementJump;
		postDecrement = &ElemContIterator::postDecrementJump;
	}
	else {
		increment     = &ElemContIterator::incrementFast;
		postIncrement = &ElemContIterator::postIncrementFast;
		decrement     = &ElemContIterator::decrementFast;
		postDecrement = &ElemContIterator::postDecrementFast;
	}
}

template<template<typename, typename...> typename Container, typename T, bool CONST>
typename ElemContIterator<Container, T, CONST>::reference
ElemContIterator<Container, T, CONST>::operator*() const
{
	return *it;
}

template<template<typename, typename...> typename Container, typename T, bool CONST>
typename ElemContIterator<Container, T, CONST>::pointer
ElemContIterator<Container, T, CONST>::operator->() const
{
	return &*it;
}

template<template<typename, typename...> typename Container, typename T, bool CONST>
bool ElemContIterator<Container, T, CONST>::operator==(const ElemContIterator& oi) const
{
	return it == oi.it;
}

template<template<typename, typename...> typename Container, typename T, bool CONST>
bool ElemContIterator<Container, T, CONST>::operator!=(const ElemContIterator& oi) const
{
	return it != oi.it;
}

template<template<typename, typename...> typename Container, typename T, bool CONST>
ElemContIterator<Container, T, CONST> ElemContIterator<Container, T, CONST>::operator++()
{
	return (this->*increment)();
}

template<template<typename, typename...> typename Container, typename T, bool CONST>
ElemContIterator<Container, T, CONST> ElemContIterator<Container, T, CONST>::operator++(int)
{
	return (this->*postIncrement)();
}

/**
 * @brief Increment function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CONST>
ElemContIterator<Container, T, CONST> ElemContIterator<Container, T, CONST>::incrementJump()
{
	do {
		++it;
	} while (it != vec->end() && it->isDeleted());
	return *this;
}

/**
 * @brief Post increment function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CONST>
ElemContIterator<Container, T, CONST> ElemContIterator<Container, T, CONST>::postIncrementJump()
{
	ElemContIterator old = *this;
	do {
		++it;
	} while (it != vec->end() && it->isDeleted());
	return old;
}

/**
 * @brief Increment function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CONST>
ElemContIterator<Container, T, CONST> ElemContIterator<Container, T, CONST>::incrementFast()
{
	++it;
	return *this;
}

/**
 * @brief Post increment function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CONST>
ElemContIterator<Container, T, CONST> ElemContIterator<Container, T, CONST>::postIncrementFast()
{
	ElemContIterator old = *this;
	++it;
	return old;
}

/**
 * @brief Decrement function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CONST>
ElemContIterator<Container, T, CONST> ElemContIterator<Container, T, CONST>::decrementJump()
{
	do {
		--it;
	} while (it != vec->begin() && it->isDeleted());
	return *this;
}

/**
 * @brief Post decrement function that will be called if we need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CONST>
ElemContIterator<Container, T, CONST> ElemContIterator<Container, T, CONST>::postDecrementJump()
{
	ElemContIterator old = *this;
	do {
		--it;
	} while (it != vec->begin() && it->isDeleted());
	return old;
}

/**
 * @brief Decrement function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CONST>
ElemContIterator<Container, T, CONST> ElemContIterator<Container, T, CONST>::decrementFast()
{
	--it;
	return *this;
}

/**
 * @brief Post decrement function that will be called if we don't need to jump deleted elements.
 */
template<template<typename, typename...> typename Container, typename T, bool CONST>
ElemContIterator<Container, T, CONST> ElemContIterator<Container, T, CONST>::postDecrementFast()
{
	ElemContIterator old = *this;
	--it;
	return old;
}

} // namespace vcl::internal
