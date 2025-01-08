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

#ifndef VCL_CONCEPTS_RANGES_RANGE_H
#define VCL_CONCEPTS_RANGES_RANGE_H

#include <ranges>

namespace vcl {

/**
 * @brief Utility concept that is evaluated true if T is a range, e.g. if has a
 * begin and an end.
 *
 * It is equivalent to std::ranges::range.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept Range = std::ranges::range<T>;

/**
 * @brief Utility concept that is evaluated true the Range R has a value_type
 * that is exactly T.
 *
 * This concept is useful when you need to check that a range has a value_type
 * that is exactly T, and not convertible to T. When this concept is satisfied
 * the following code will always compile:
 *
 * @code
 * for (const T& value : range) {
 *     // do something with value
 *     ...
 * }
 * @endcode
 *
 * @note If the type of the range is not T, this concept will be evaluated
 * false, even if it is convertible to T. If you need to check that the range
 * has a value_type that is convertible to T, use the InputRange concept.
 *
 * @tparam R The range type.
 * @tparam T The value type.
 *
 * @ingroup util_concepts
 */
template<typename R, typename T>
concept RangeOf = Range<R> && std::same_as<std::ranges::range_value_t<R>, T>;

/**
 * @brief Utility concept that is evaluated true the Range R is an Input Range
 * and has a value_type that is convertible to T.
 *
 * This concept is useful when you need to check that a range has a value_type
 * that is convertible to T. When this concept is satisfied the following code
 * will always compile:
 *
 * @code
 * for (T value : range) {
 *     // do something with value
 *     ...
 * }
 * @endcode
 *
 * @tparam R The range type.
 * @tparam T The value type.
 *
 * @ingroup util_concepts
 */
template<typename R, typename T>
concept InputRange =
    Range<R> && std::convertible_to<std::ranges::range_value_t<R>, T>;

/**
 * @brief Utility concept that is evaluated true the Range R is an Output Range
 * and has a value_type that is T.
 *
 * This concept is useful when you need to check that a range has a value_type
 * that is exactly T, and it can be modified. When this concept is satisfied
 * the following code will always compile:
 *
 * @code
 * for (T& value : range) {
 *     // do something with value
 *     ...
 * }
 * @endcode
 *
 * @tparam R The range type.
 * @tparam T The value type.
 *
 * @ingroup util_concepts
 */
template<typename R, typename T>
concept OutputRange = Range<R> && std::ranges::output_range<R, T> &&
                      std::same_as<std::ranges::range_value_t<R>, T>;

} // namespace vcl

#endif // VCL_CONCEPTS_RANGES_RANGE_H
