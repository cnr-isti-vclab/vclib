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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_NORMAL_H
#define VCL_CONCEPTS_MESH_COMPONENTS_NORMAL_H

#include "component.h"

namespace vcl::comp {

/**
 * @brief HasNormal concept is satisfied only if a Element class provides the
 * types and member functions specified in this concept. These types and member
 * functions allow to access to a Normal component of a given element.
 *
 * Note that this concept does not discriminate between the Horizontal Normal
 * component and the vertical OptionalNormal component, therefore it does not
 * guarantee that a template Element type that satisfies this concept provides
 * Normal component at runtime (it is guaranteed only that the proper member
 * functions are available at compile time).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasNormal = requires(
    T o,
    const T& co)
{
    typename T::NormalType;
    { o.normal() } -> std::same_as<typename T::NormalType&>;
    { co.normal() } -> std::same_as<const typename T::NormalType&>;
};

/**
 * @brief HasOptionalNormal concept is satisfied only if a class satisfis the
 * HasNormal concept and the static boolean constant IS_OPTIONAL is set to true.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalNormal =
    HasNormal<T> && IsOptionalComponent<typename T::Normal>;

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_NORMAL_H
