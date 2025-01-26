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

#ifndef VCL_CONCEPTS_SPACE_TEX_COORD_INDEXED_H
#define VCL_CONCEPTS_SPACE_TEX_COORD_INDEXED_H

#include "tex_coord.h"

namespace vcl {

/**
 * @brief A concept representing a Texture Coordinate with an index.
 *
 * @tparam T: The type to be tested for conformity to the TexCoordIndexedConcept.
 */
template<typename T>
concept TexCoordIndexedConcept =
    TexCoordConcept<T> && requires (
                              T&&                                obj,
                              ushort                             u,
                              ushort&                            uR,
                              typename RemoveRef<T>::ScalarType  s) {
        typename RemoveRef<T>::ScalarType;

        RemoveRef<T>();
        RemoveRef<T>(s, s, u);

        { obj.index() } -> std::convertible_to<decltype(u)>;

        { obj <=> obj } -> std::convertible_to<std::partial_ordering>;

        // non const requirements
        requires IsConst<T> || requires {
            { obj.index() } -> std::same_as<decltype(uR)>;
            { obj.set(s, s, u) } -> std::same_as<void>;
        };
    };

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_TEX_COORD_INDEXED_H
