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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_COORDINATE_H
#define VCL_CONCEPTS_MESH_COMPONENTS_COORDINATE_H

#include "component.h"

namespace vcl::comp {

/**
 * @brief HasCoordinate concept is satisfied only if a Element class provides
 * the types and member functions specified in this concept. These types and
 * member functions allow to access to a Coordinate component of a given
 * element.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasCoordinate = requires (T o, const T& co) {
    // clang-format off
    typename T::CoordType;
    { o.coord() } -> std::same_as<typename T::CoordType&>;
    { co.coord() } -> std::same_as<const typename T::CoordType&>;
    // clang-format on
};

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_COORDINATE_H
