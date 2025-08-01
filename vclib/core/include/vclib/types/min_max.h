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

#ifndef VCL_TYPES_MIN_MAX_H
#define VCL_TYPES_MIN_MAX_H

#include <algorithm>

namespace vcl {

/**
 * @brief Returns the minimum between the two parameters.
 *
 * @param[in] p1: The first parameter.
 * @param[in] p2: The second parameter.
 * @return The minimum between the two parameters.
 *
 * @ingroup types
 */
template<typename T>
constexpr auto min(const T& p1, const T& p2)
{
    return std::min(p1, p2);
}

/**
 * @brief Returns the minimum between several parameters.
 *
 * Given a list of parameters, this function returns the minimum between all of
 * them.
 *
 * Example:
 *
 * @code{.cpp}
 * auto m = vcl::min(1, 2, 3, 4, 5); // m = 1
 * @endcode
 *
 * @param[in] head0: The first parameter.
 * @param[in] head1: The second parameter.
 * @param[in] tail: The rest of the parameters.
 * @return The minimum between the parameters.
 *
 * @ingroup types
 */
template<typename Head, typename... Tail>
constexpr auto min(const Head& head0, const Head& head1, const Tail&... tail)
    requires (sizeof...(tail) > 0)
{
    return min(min(head0, head1), tail...);
}

/**
 * @brief Returns the maximum between the two parameters.
 *
 * @param[in] p1: The first parameter.
 * @param[in] p2: The second parameter.
 * @return The maximum between the two parameters.
 *
 * @ingroup types
 */
template<typename T>
constexpr auto max(const T& p1, const T& p2)
{
    return std::max(p1, p2);
}

/**
 * @brief Returns the maximum between several parameters.
 *
 * Given a list of parameters, this function returns the maximum between all of
 * them.
 *
 * Example:
 *
 * @code{.cpp}
 * auto m = vcl::max(1, 2, 3, 4, 5); // m = 5
 * @endcode
 *
 * @param[in] head0: The first parameter.
 * @param[in] head1: The second parameter.
 * @param[in] tail: The rest of the parameters.
 * @return The maximum between the parameters.
 *
 * @ingroup types
 */
template<typename Head, typename... Tail>
constexpr auto max(const Head& head0, const Head& head1, const Tail&... tail)
    requires (sizeof...(tail) > 0)
{
    return max(max(head0, head1), tail...);
}

} // namespace vcl

#endif // VCL_TYPES_MIN_MAX_H
