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

#ifndef VCL_CONCEPTS_SPACE_TEX_COORD_H
#define VCL_CONCEPTS_SPACE_TEX_COORD_H

#include <vclib/concepts/const_correctness.h>
#include <vclib/types.h>

namespace vcl {

/**
 * @brief A concept representing a Texture Coordinate.
 *
 * @tparam T: The type to be tested for conformity to the BoxConcept.
 */
template<typename T>
concept TexCoordConcept = requires (
    T&&                                obj,
    typename RemoveRef<T>::ScalarType  s,
    typename RemoveRef<T>::ScalarType& sR) {
    typename RemoveRef<T>::ScalarType;

    RemoveRef<T>();
    RemoveRef<T>(s, s);

    { obj.u() } -> std::convertible_to<decltype(s)>;
    { obj.v() } -> std::convertible_to<decltype(s)>;

    { obj == obj } -> std::same_as<bool>;

    { obj(uint()) } -> std::convertible_to<decltype(s)>;
    { obj[uint()] } -> std::convertible_to<decltype(s)>;

    // non const requirements
    requires vcl::IsConst<T> || requires {
        { obj.u() } -> std::same_as<decltype(sR)>;
        { obj.v() } -> std::same_as<decltype(sR)>;
        { obj.setU(s) } -> std::same_as<void>;
        { obj.setV(s) } -> std::same_as<void>;
        { obj.set(s, s) } -> std::same_as<void>;

        { obj(uint()) } -> std::same_as<decltype(sR)>;
        { obj[uint()] } -> std::same_as<decltype(sR)>;
    };
};

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_TEX_COORD_H
