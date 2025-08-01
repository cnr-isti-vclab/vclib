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

#ifndef VCL_BASE_NESTED_INITIALIZER_LISTS_H
#define VCL_BASE_NESTED_INITIALIZER_LISTS_H

/* @link https://zcrou.com/blog/dev/nested-initializers */

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <list>

namespace vcl {

// Traits
namespace detail {

// recursive case: a nested list is a list of L-1 levels
template<typename T, std::size_t L>
struct NestedInitializerListsTraits
{
    using type = std::initializer_list<
        typename NestedInitializerListsTraits<T, L - 1>::type>;
};

// base case: no list, only the type T
template<typename T>
struct NestedInitializerListsTraits<T, 0>
{
    using type = T;
};

} // namespace detail

// Processors that allow to automatically iterate through nested initializer
// lists

/**
 * @brief The NestedInitializerLists class manages nested
 * std::initializer_list<T> types of L levels L must be known at compile time,
 * and compilation will fail if the NestedInitializerLists used does not
 * correspond to the number of used levels.
 */
template<typename T, std::size_t L>
using NestedInitializerLists = detail::NestedInitializerListsTraits<T, L>::type;

// Recursive part.

/**
 * @brief The NestedInitializerListsProcessor class
 *
 * Proposes some helper static functions that allows to process
 * vcl::NestedInitializerLists of L levels.
 */
template<typename T, std::size_t L>
class NestedInitializerListsProcessor
{
public:
    /**
     * @brief Returns a list containing the maximum size of elements for every
     * dimension.
     *
     * @code{.cpp}
     * std::list<size_t> list = vcl::NestedInitializerListsProcessor<int, 2>::
     *                               maxDimensionsLevels(
     *                                   {{2, 3, 5, 4}, {2}, {2, 4, 6}});
     *                          //the list will contain the values 3 and 4.
     *
     * std::list<size_t> list = vcl::NestedInitializerListsProcessor<int, 3>::
     *                               maxDimensionsLevels(
     *                                   {
     *                                     {{2, 3}, {5, 4}, {6}, {2, 5}},
     *                                     {{2}, {3, 4, 6}}
     *                                   } );
     *                          //the list will contain the values 2, 4 and 3.
     * @endcode
     *
     * @param[in] values: the nested initializer lists
     * @return a list of L numbers, where every number is the maximum size of
     * elements for every dimension.
     */
    static std::list<size_t> maxDimensionsLevels(
        NestedInitializerLists<T, L> values)
    {
        std::list<size_t> final;
        bool              first = true;
        for (auto nested : values) {
            std::list<size_t> l =
                NestedInitializerListsProcessor<T, L - 1>::maxDimensionsLevels(
                    nested);

            if (first) {
                first = false;
                for (size_t s : l)
                    final.push_back(s);
            }
            else {
                assert(l.size() == final.size());
                std::list<size_t>::iterator it = l.begin();
                for (size_t& s : final) {
                    if (s < *it)
                        s = *it;
                    ++it;
                }
            }
        }
        final.push_front(values.size());
        return final;
    }

    /**
     * @brief Applies the lambda function passed as parameter to all the
     * elements of the NestedInitializerLists
     *
     * Example: save all the elements in a vector:
     *
     * @code{.cpp}
     * std::vector<T> v;
     * // resize vector proprerly, see
     * // NestedInitializerListsProcessor<T, N>::maxDimensionsLevel
     * typename std::vector<T>::iterator iterator = v.begin();
     * NestedInitializerListsProcessor<T, N>::processElements(values,
     *     [&iterator](T value) { *(iterator++) = value; });
     * @endcode
     *
     * @note This function does not take into account the sizes of every list.
     * If some values are missing from the lists they will be skipped and
     * function is not applied.
     * @param values
     * @param function: a lambda function that takes an argument of type T as
     * input.
     */
    template<typename T_Function>
    static void processElements(
        NestedInitializerLists<T, L> values,
        T_Function                   function)
    {
        for (auto nested : values) {
            NestedInitializerListsProcessor<T, L - 1>::processElements(
                nested, function);
        }
    }

    /**
     * @brief Applies the lambda function passed as parameter to all the
     * elements of the NestedInitializerLists
     *
     * Example: save all the elements in a vector:
     *
     * @code{.cpp}
     * std::vector<T> v;
     * // resize vector proprerly, see
     * //NestedInitializerListsProcessor<T,N>::maxDimensionsLevel
     * std::list<std::size_t> sizes;
     * // push values properly in sizes (example:
     * //   sizes = NestedInitializerListsProcessor<T, N>::maxDimensionsLevel)
     *
     * typename std::vector<T>::iterator iterator = v.begin();
     * NestedInitializerListsProcessor<T, N>::processElements(values,
     *     [&iterator](T value) { *(iterator++) = value; });
     * @endcode
     *
     * @note This function takes into account the sizes of every list fits every
     * list in the given sizes. If some values are missing in a list, a zero
     * vale casted to the type T is assed to the function.
     * @param values
     * @param function: a lambda function that takes an argument of type T as
     * input.
     * @param sizes
     */
    template<typename T_Function>
    static void processElements(
        NestedInitializerLists<T, L> values,
        T_Function                   function,
        std::list<size_t>            sizes)
    {
        size_t curr_size = sizes.front();
        sizes.pop_front();
        for (auto nested : values) {
            NestedInitializerListsProcessor<T, L - 1>::processElements(
                nested, function, sizes);
        }

        // 0 in left values
        if (values.size() < curr_size) {
            std::size_t count = 1;
            for (size_t s : sizes)
                count *= s;

            while (count-- > 0)
                function(static_cast<T>(0));
        }
    }
};

// Last level.

template<typename T>
class NestedInitializerListsProcessor<T, 1>
{
public:
    using InitializerList = NestedInitializerLists<T, 1>;

    static std::list<size_t> maxDimensionsLevels(InitializerList values)
    {
        std::list<size_t> dim;
        dim.push_back(values.size());
        return dim;
    }

    template<typename T_Function>
    static void processElements(
        InitializerList   values,
        T_Function        function,
        std::list<size_t> sizes)
    {
        size_t row_size = 1;
        for (size_t s : sizes)
            row_size *= s;
        std::for_each(values.begin(), values.end(), function);

        // 0 in left values
        if (values.size() < row_size) {
            std::size_t count = row_size - values.size();
            while (count-- > 0) {
                function(static_cast<T>(0));
            }
        }
    }

    template<typename T_Function>
    static void processElements(InitializerList values, T_Function function)
    {
        std::for_each(values.begin(), values.end(), function);
    }
};

} // namespace vcl

#endif // VCL_BASE_NESTED_INITIALIZER_LISTS_H
