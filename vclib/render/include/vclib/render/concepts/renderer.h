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

#ifndef CONCEPTS_RENDERER_H
#define CONCEPTS_RENDERER_H

#include <vclib/concepts.h>

namespace vcl {

template<typename T>
concept RendererConcept =
    requires (T&& obj, typename T::ParentType* pPtr, std::string s, uint u) {
        typename RemoveRef<T>::ParentType;

        T();
        T(pPtr);
        T(s, u, u);
        T(s, u, u, pPtr);

        // { obj.update() } -> std::same_as<void>;
        // { obj.draw() } -> std::same_as<void>;
        // { obj.drawContent() } -> std::same_as<void>;
    };

} // namespace vcl

#endif // CONCEPTS_RENDERER_H
