/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#ifndef VCLIB_CONCEPTS_RANGES_RANGE_H
#define VCLIB_CONCEPTS_RANGES_RANGE_H

#include <ranges>

namespace vcl {

/**
 * @brief Utility concept that is evaluated true if T is a range, e.g. if has a
 * begin and an end.
 *
 * It is equivalent to std::ranges::range.
 *
 * @ingroup uconcepts
 */
template<typename T>
concept Range = std::ranges::range<T>;

/**
 * @brief Utility concept that is evaluated true the Range R has a value_type
 * that is T.
 *
 * @ingroup uconcepts
 */
template<typename R, typename T>
concept RangeOf = Range<R> && std::is_same_v<std::ranges::range_value_t<R>, T>;

/**
 * @brief Utility concept that is evaluated true the Range R has a value_type
 * that is convertible to T.
 *
 * @ingroup uconcepts
 */
template<typename R, typename T>
concept RangeOfConvertibleTo =
    Range<R> && std::convertible_to<std::ranges::range_value_t<R>, T>;

} // namespace vcl

#endif // VCLIB_CONCEPTS_RANGES_RANGE_H
