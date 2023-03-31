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

namespace vcl {

/*
 * Utility Pointer concept to check if a type is a Pointer
 */
template<typename T>
concept IsPointer = std::is_pointer_v<T>;

/*
 * Utility to get clean type from an input type that could have a reference or a pointer.
 */
template<typename T>
using RemoveRefAndPointer =
	typename std::remove_pointer_t<typename std::remove_reference_t<T>>;

} // namespace vcl

#endif // VCL_TYPES_POINTERS_H
