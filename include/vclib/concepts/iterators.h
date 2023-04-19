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

#ifndef VCL_CONCEPTS_ITERATORS_H
#define VCL_CONCEPTS_ITERATORS_H

#include <iterator>
#include <type_traits>

namespace vcl {

template<typename T>
concept IteratorConcept = std::input_or_output_iterator<T>;

template<typename T>
concept IteratesOverClass = IteratorConcept<T> && std::is_class_v<typename T::value_type>;

template<typename T>
concept IteratesOverPointer = IteratorConcept<T> && std::is_pointer_v<typename T::value_type>;

} // namespace vcl

#endif // VCL_CONCEPTS_ITERATORS_H
