/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_CONCEPTS_ITERATORS_H
#define VCL_CONCEPTS_ITERATORS_H

#include <iterator>
#include <type_traits>

namespace vcl {

/**
 * @brief Utility concept that is evaluated true if T is an iterator.
 *
 * @ingroup uconcepts
 */
template<typename T>
concept IteratorConcept = std::input_or_output_iterator<T>;

/**
 * @brief Utility concept that is evaluated true if T is an iterator having its
 * value_type that is a class.
 *
 * @ingroup uconcepts
 */
template<typename T>
concept IteratesOverClass =
    IteratorConcept<T> && std::is_class_v<typename T::value_type>;

/**
 * @brief Utility concept that is evaluated true if T is an iterator having its
 * value_type that is a pointer.
 *
 * @ingroup uconcepts
 */
template<typename T>
concept IteratesOverPointer =
    IteratorConcept<T> && std::is_pointer_v<typename T::value_type>;

} // namespace vcl

#endif // VCL_CONCEPTS_ITERATORS_H
