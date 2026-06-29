// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_MIN_MAX_H
#define VCL_BASE_MIN_MAX_H

#include <algorithm>

namespace vcl {

/**
 * @brief Returns the minimum between the two parameters.
 *
 * @param[in] p1: The first parameter.
 * @param[in] p2: The second parameter.
 * @return The minimum between the two parameters.
 *
 * @ingroup base
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
 * @ingroup base
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
 * @ingroup base
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
 * @ingroup base
 */
template<typename Head, typename... Tail>
constexpr auto max(const Head& head0, const Head& head1, const Tail&... tail)
    requires (sizeof...(tail) > 0)
{
    return max(max(head0, head1), tail...);
}

} // namespace vcl

#endif // VCL_BASE_MIN_MAX_H
