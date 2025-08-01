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

#ifndef VCL_BASE_TYPE_WRAPPER_H
#define VCL_BASE_TYPE_WRAPPER_H

#include "variadic_templates.h"

namespace vcl {

/**
 * @brief A simple structure that wraps a list of variadic templates, without
 * instantiating anything. Useful when you need to wrap a list of types, and
 * consider them as a single type.
 *
 * @ingroup base
 */
template<typename... Args>
struct TypeWrapper
{
    static constexpr uint size() { return sizeof...(Args); }
};

/**
 * @copydoc FirstType
 *
 * @ingroup base
 */
// note: specialization from variadic_templates.h
template<typename... Args>
struct FirstType<TypeWrapper<Args...>>
{
    using type = std::tuple_element<0, std::tuple<Args...>>::type;
};

/**
 * @brief Function that returns the index of the type having the given
 * type_index in a TypeWrapper.
 *
 * The TypeWrapper must be given as a second argument to the function.
 *
 * @param[in] ti: the type_index of the type to search.
 * @param[in] tw: the TypeWrapper to search in.
 *
 * @ingroup base
 */
// note: specialization from variadic_templates.h
template<typename... Args>
uint indexInTypePack(std::type_index ti, TypeWrapper<Args...>)
{
    return indexInTypePack<Args...>(ti);
}

/**
 * @copydoc IndexInTypes
 *
 * @ingroup base
 */
// note: specialization from variadic_templates.h
template<typename T, typename... Us>
struct IndexInTypes<T, TypeWrapper<Us...>>
{
    static constexpr uint value = indexInTypePack<T, Us...>();
};

/**
 * @copydoc TypeAt
 *
 * @ingroup base
 */
// note: specialization from variadic_templates.h
template<uint I, typename... T>
struct TypeAt<I, TypeWrapper<T...>>
{
    using type = TypeAt<I, T...>::type;
};

/**
 * @copydoc NumberOfTypes
 *
 * @ingroup base
 */
// note: specialization from variadic_templates.h
template<typename... Args>
struct NumberOfTypes<TypeWrapper<Args...>> : public NumberOfTypes<Args...>
{
};

/**
 * @copydoc ForEachType
 *
 * @ingroup base
 */
// note: specialization from variadic_templates.h
template<typename... T>
struct ForEachType<TypeWrapper<T...>> : public ForEachType<T...>
{
};

} // namespace vcl

#endif // VCL_BASE_TYPE_WRAPPER_H
