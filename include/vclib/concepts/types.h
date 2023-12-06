/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_CONCEPTS_TYPES_H
#define VCL_CONCEPTS_TYPES_H

#include <concepts>

namespace vcl {

/**
 * @brief Concept for types that can be used as indices.
 */
template<typename T>
concept IntegralOrEnum = std::integral<T> || std::is_enum_v<T>;

/**
 * @brief Concept for types that can be used as indices, excluding bool.
 */
template<typename T>
concept NonBoolIntegralOrEnum = IntegralOrEnum<T> && !std::same_as<T, bool>;

} // namespace vcl

#endif // VCL_CONCEPTS_TYPES_H
