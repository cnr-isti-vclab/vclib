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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_TEX_COORD_H
#define VCL_CONCEPTS_MESH_COMPONENTS_TEX_COORD_H

#include "component.h"

namespace vcl::comp {

/**
 * @brief HasTexCoord concept is satisfied only if a Element class provides the
 * types and member functions specified in this concept. These types and member
 * functions allow to access to a TexCoord component of a given element.
 *
 * Note that this concept does not discriminate between the Horizontal TexCoord
 * component and the vertical OptionalTexCoord component, therefore it does not
 * guarantee that a template Element type that satisfies this concept provides
 * TexCoord component at runtime (it is guaranteed only that the proper member
 * functions are available at compile time).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasTexCoord = requires(
    T o,
    const T& co)
{
    typename T::TexCoordType;
    { o.texCoord() } -> std::same_as<typename T::TexCoordType&>;
    { co.texCoord() } -> std::same_as<const typename T::TexCoordType&>;
};

/**
 * @brief HasOptionalTexCoord concept is satisfied only if a class satisfis the
 * HasTexCoord concept and the static boolean constant IS_OPTIONAL is set to
 * true.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalTexCoord =
    HasTexCoord<T> && IsOptionalComponent<typename T::TexCoord>;

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_TEX_COORD_H
