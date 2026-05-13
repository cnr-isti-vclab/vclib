/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_BASE_CONCEPTS_PARALLEL_H
#define VCL_BASE_CONCEPTS_PARALLEL_H

// tbb and qt conflicts: if both are linked, we need to first undef Qt's
// emit - see: https://github.com/oneapi-src/oneTBB/issues/547
#if defined(emit)
#undef emit
#define VCLIB_EMIT_REDEFINED
#endif // emit

// Hack to compensate lack of support for c++17 parallel algorithms by
// several compilers. We use poolSTL.
#define POOLSTL_STD_SUPPLEMENT
#if __has_include(<poolstl/poolstl.hpp>)
#include <poolstl/poolstl.hpp>
#else
#include "../../../../../../3rdparty/poolSTL-0.3.5/include/poolstl/poolstl.hpp"
#endif

// Restore the definition of "emit" if it was defined before
#ifdef VCLIB_EMIT_REDEFINED
#undef VCLIB_EMIT_REDEFINED
#define emit // restore the macro definition of "emit", as it was
             // defined in gtmetamacros.h
#endif       // VCLIB_EMIT_REDEFINED

namespace vcl {

namespace detail {

// poolSTL does not provide the is_execution_policy type trait, so we define it
// here, using the one provided by poolSTL
// TODO: remove this when https://github.com/alugowski/poolSTL/pull/42 is merged
template<class T>
struct is_execution_policy : std::false_type
{
};

template<>
struct is_execution_policy<std::execution::sequenced_policy> : std::true_type
{
};

template<>
struct is_execution_policy<std::execution::parallel_policy> : std::true_type
{
};

template<class T>
constexpr bool is_execution_policy_v = is_execution_policy<T>::value;

} // namespace detail

/**
 * @brief The ExecutionPolicy is satisfied if T is a standard execution policy
 * type.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept ExecutionPolicy = detail::is_execution_policy_v<std::remove_cvref_t<T>>;

} // namespace vcl

#endif // VCL_BASE_CONCEPTS_PARALLEL_H
