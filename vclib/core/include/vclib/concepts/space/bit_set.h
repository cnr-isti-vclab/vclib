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

#ifndef VCL_CONCEPTS_SPACE_BIT_SET_H
#define VCL_CONCEPTS_SPACE_BIT_SET_H

#include <vclib/concepts/const_correctness.h>

#include <concepts>

namespace vcl {

/**
 * @brief BitProxyConcept is satisfied only if a class provides the member
 * functions specified in this concept. These member functions allows to access
 * to a bool reference from a bit saved in a mask, and then allow assignment.
 *
 * @ingroup space_concepts
 */
template<typename T>
concept BitProxyConcept = requires (T&& obj) {
    requires std::convertible_to<T, bool>;

    obj = bool();
    obj |= bool();
    obj &= bool();
    obj /= bool();
};

/**
 * @brief BitSetConcept is satisfied only if a class provides the member
 * functions specified in this concept. These member functions allows to a list
 * of bits encoded in a integral type.
 *
 * @ingroup space_concepts
 */
template<typename T>
concept BitSetConcept = requires (T&& obj) {
    RemoveRef<T>();
    RemoveRef<T>({uint(), uint()});

    { obj.size() } -> std::same_as<std::size_t>;

    { obj.at(uint()) } -> std::convertible_to<bool>;
    { obj[uint()] } -> std::convertible_to<bool>;

    { obj.all() } -> std::same_as<bool>;
    { obj.any() } -> std::same_as<bool>;
    { obj.none() } -> std::same_as<bool>;

    { obj == obj } -> std::same_as<bool>;
    { obj <=> obj } -> std::convertible_to<std::partial_ordering>;

    // non const requirements
    requires IsConst<T> || requires {
        { obj.at(uint()) } -> BitProxyConcept;
        { obj[uint()] } -> BitProxyConcept;

        obj.set();
        obj.set(bool(), uint());
        obj.reset();
        obj.reset(uint());
        obj.flip();
        obj.flip(uint());
    };
};

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_BIT_SET_H
