/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MISC_TUPLE_H
#define VCL_MISC_TUPLE_H

#include <vclib/types.h>

#include <tuple>

namespace vcl {

/**
 * @brief Returns the index of a type T in a std::tuple<U, Us...> passed as
 * parameter; Returns UINT_NULL if the type T is not found in the tuple
 */
template<typename T, typename U, typename... Us>
constexpr uint tupleIndexByType(const std::tuple<U, Us...>&)
{
    return IndexInTypes<T, U, Us...>::value;
}

} // namespace vcl

#endif // VCL_MISC_TUPLE_H
