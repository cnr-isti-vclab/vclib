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

#ifndef VCL_TYPES_TYPE_WRAPPER_H
#define VCL_TYPES_TYPE_WRAPPER_H

#include "variadic_templates.h"

namespace vcl {

/**
 * @brief A simple structure that wraps a list of variadic templates, without
 * instantiating anything. Useful when you need to wrap a list of types, and
 * consider them as a single type.
 *
 * @ingroup types
 */
template<typename... Args>
struct TypeWrapper
{
    static constexpr uint size() { return sizeof...(Args); }
};

/**
 * @copydoc FirstType
 *
 * @ingroup types
 */
template<typename... Args>
struct FirstType<TypeWrapper<Args...>>
{
    using type = std::tuple_element<0, std::tuple<Args...>>::type;
};

template<typename... Args>
uint indexInTypePack(std::type_index ti, TypeWrapper<Args...>)
{
    return indexInTypePack<Args...>(ti);
}

template<typename T, typename... Us>
struct IndexInTypes<T, TypeWrapper<Us...>>
{
    static constexpr uint value = indexInTypePack<T, Us...>();
};

template<typename... Args>
struct NumberOfTypes<TypeWrapper<Args...>> : public NumberOfTypes<Args...>
{
};

template<typename... T>
struct ForEachType<TypeWrapper<T...>> : public ForEachType<T...>
{
};

} // namespace vcl

#endif // VCL_TYPES_TYPE_WRAPPER_H
