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

#ifndef VCL_TYPES_POINTERS_H
#define VCL_TYPES_POINTERS_H

#include <type_traits>
#include <utility>

namespace vcl {

/*
 * Utility to get clean type from an input type that could have a reference or a pointer.
 */
template<typename T>
using RemoveCVRefAndPointer =
	typename std::remove_cvref_t<std::remove_pointer_t<T>>;

/*
 * Utility to get a pointer type without the constness.
 * e.g. If T is const int*, the resulting type is int*
 * If the type is not a pointer, it is left as it was
 */
template<typename T>
using RemoveConstFromPointer =
	std::conditional_t<std::is_pointer_v<T>, 
	std::add_pointer_t<typename std::remove_cv_t<typename std::remove_pointer_t<T>>>,
	T>;

template<typename T>
T&& dereference(T&& obj)
{
	return std::forward<T>(obj);
}

template<typename T>
T& dereference(T* obj)
{
	return *obj;
}

template<typename T>
const T& dereference(const T* obj)
{
	return *obj;
}

template<typename T>
auto addressOf(T&& obj)
{
	return &(std::forward<T>(obj));
}

template<typename T>
T* addressOf(T* obj)
{
	return obj;
}

template<typename T>
const T* addressOf(const T* obj)
{
	return obj;
}

} // namespace vcl

#endif // VCL_TYPES_POINTERS_H
