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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_MATERIALS_H
#define VCL_CONCEPTS_MESH_COMPONENTS_MATERIALS_H

#include <vclib/concepts/space/material.h>
#include <vclib/concepts/iterators.h>
#include <vclib/concepts/ranges/range.h>

namespace vcl::comp {

/**
 * @brief HasMaterials concept is satisfied only if a Mesh class
 * provides the member functions specified in this concept. These member
 * functions allows to access to a @ref vcl::comp::Materials component of a
 * given mesh.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasMaterials =
    requires (T&& obj, typename RemoveRef<T>::MaterialType m) {

        { obj.materialNumber() } -> std::same_as<uint>;
        { obj.meshBasePath() } -> std::convertible_to<std::string>;

        { obj.material(uint()) } -> MaterialConcept;

        { obj.materialBegin() } -> InputIterator<decltype(m)>;
        { obj.materialEnd() } -> InputIterator<decltype(m)>;
        { obj.materials() } -> InputRange<decltype(m)>;

        // non const requirements
        requires IsConst<T> || requires {
            { obj.meshBasePath() } -> std::same_as<std::string&>;

            { obj.clearMaterials() } -> std::same_as<void>;
            { obj.pushMaterial(m) } -> std::same_as<void>;

            { obj.materialBegin() } -> OutputIterator<decltype(m)>;
            { obj.materialEnd() } -> OutputIterator<decltype(m)>;
            { obj.materials() } -> OutputRange<decltype(m)>;
        };
    };

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_MATERIALS_H
