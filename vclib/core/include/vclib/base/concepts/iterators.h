// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_CONCEPTS_ITERATORS_H
#define VCL_BASE_CONCEPTS_ITERATORS_H

#include <iterator>
#include <type_traits>

namespace vcl {

/**
 * @brief The IteratorConcept is satisfied if T is an input or output iterator.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept IteratorConcept = std::input_or_output_iterator<T>;

/**
 * @brief The InputIterator concept is satisfied if T is an input iterator
 * that implements the `operator*` returning a value convertible to V.
 *
 * @ingroup util_concepts
 */
template<typename T, typename V>
concept InputIterator =
    std::input_iterator<T> && std::indirectly_readable<T> && requires (T i) {
        { *i } -> std::convertible_to<V>;
    };

/**
 * @brief The OutputIterator concept is satisfied if T is an output iterator
 * that implements the `operator*` returning a reference to V.
 *
 * @see https://en.cppreference.com/w/cpp/iterator/output_iterator
 *
 * @ingroup util_concepts
 */
template<typename T, typename V>
concept OutputIterator = std::output_iterator<T, V>;

/**
 * @brief The IteratorOverClass concept is satisfied if T is an iterator having
 * its `value_type` that is a class.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept IteratesOverClass =
    IteratorConcept<T> && std::is_class_v<typename T::value_type>;

/**
 * @brief The IteratorOverPointer concept is satisfied if T is an iterator
 * having its `value_type ` that is a pointer.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept IteratesOverPointer =
    IteratorConcept<T> && std::is_pointer_v<typename T::value_type>;

} // namespace vcl

#endif // VCL_BASE_CONCEPTS_ITERATORS_H
