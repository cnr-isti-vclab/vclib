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

#ifndef VCL_BASE_COMPARATORS_H
#define VCL_BASE_COMPARATORS_H

#include <algorithm>
#include <utility>

namespace vcl {

/**
 * @brief The PairComparator struct is an utility comparator to allow to sort
 * pairs in lexical order
 *
 * It first compare the first element, and in case they are equal it compares
 * the second element.
 */
template<typename Pair>
struct PairComparator
{
    bool operator()(const Pair& p1, const Pair& p2) const
    {
        if (p1.first == p2.first) {
            return p1.second < p2.second;
        }
        return p1.first < p2.first;
    }
};

/**
 * @brief The UnorderedPairComparator struct is an utility comparator to allow
 * to sort unordered std::pair<T, T>, that means that it is not important the
 * order of the first and the second element of the pair. This menas that pairs
 * (1, 2) and (2, 1) are considered equal, and (3, 1) < (2, 3).
 */
template<typename T>
struct UnorderedPairComparator
{
    bool operator()(const std::pair<T, T>& p1, const std::pair<T, T>& p2) const
    {
        if (std::min(p1.first, p1.second) < std::min(p2.first, p2.second)) {
            return true;
        }
        else if (
            std::min(p1.first, p1.second) == std::min(p2.first, p2.second)) {
            return std::max(p1.first, p1.second) <
                   std::max(p2.first, p2.second);
        }
        else {
            return false;
        }
    }
};

/**
 * @brief The FirstElementPairComparator struct is an utility comparator that
 * allow to sort std::pair (or any type that has a sortable member called
 * 'first') using only its first element. The second element of the Pair type is
 * ignored and won't be considered by the comparator. This means that pairs
 * having equal first elements but different second elements will be considered
 * as equals.
 */
template<typename Pair>
struct FirstElementPairComparator
{
    bool operator()(const Pair& p1, const Pair& p2) const
    {
        return p1.first < p2.first;
    }
};

} // namespace vcl

#endif // VCL_BASE_COMPARATORS_H
