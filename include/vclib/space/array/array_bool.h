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

#ifndef VCL_SPACE_ARRAY_ARRAY_BOOL_H
#define VCL_SPACE_ARRAY_ARRAY_BOOL_H

#include "array_t.h"

namespace vcl {

/*
 * Explicit specialization of the Array class for bool type.
 * Necessary to avoid the std::vector<bool> optimization.
 */
template<size_t N>
class Array<bool, N> : public Array<uint8_t, N>
{
};

} // namespace vcl

#endif // VCL_SPACE_ARRAY_ARRAY_BOOL_H
