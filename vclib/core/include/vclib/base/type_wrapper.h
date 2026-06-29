// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
