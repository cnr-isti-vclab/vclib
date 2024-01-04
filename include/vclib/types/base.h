/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_TYPES_BASE_H
#define VCL_TYPES_BASE_H

#include <assert.h>
#include <concepts>
#include <numeric>

using uint   = unsigned int;
using ushort = unsigned short;

namespace vcl {

/*
 * Represent a null value of uintm that is the maximum value that can be
 * represented with unsigned int. Allows to fully use all the possible values
 * (except one) that can be represented in an unsigned int, but with the
 * possibility to flag is a value is not initialized or is set to null.
 */
const uint UINT_NULL = std::numeric_limits<uint>::max();

/**
 * @brief A simple type that enumerates the main primitive types.
 *
 * It is used mostly for I/O operations, but can be useful in any context where
 * a type must be described in a variable at runtime.
 */
typedef enum {
    CHAR,
    UCHAR,
    SHORT,
    USHORT,
    INT,
    UINT,
    FLOAT,
    DOUBLE,
    NONE
} PrimitiveType;

} // namespace vcl

#endif // VCL_TYPES_BASE_H
