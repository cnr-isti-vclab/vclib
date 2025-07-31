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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_BIT_FLAGS_H
#define VCL_MESH_COMPONENTS_CONCEPTS_BIT_FLAGS_H

#include "predicates.h"

#include <vclib/space/core.h>
#include <vclib/types.h>

namespace vcl::comp {

template<typename, bool>
class BitFlags;

template<int, typename, bool>
class PolygonBitFlags;

template<typename, bool>
class TriangleBitFlags;

/**
 * @brief A concept that checks whether a type T (that should be a Face Element)
 * has the PolygonBitFlags component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::comp::PolygonBitFlags, with any template arguments.
 *
 * @tparam T: The type to be tested for conformity to the HasPolygonBitFlags.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasPolygonBitFlags =
    ITB::IsDerivedFromSpecializationOfV<T, PolygonBitFlags>;

/**
 * @brief A concept that checks whether a type T (that should be a Face Element)
 * has the TriangleBitFlags component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::comp::TriangleBitFlags, with any template arguments.
 *
 * @tparam T: The type to be tested for conformity to the HasTriangleBitFlags.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasTriangleBitFlags =
    TB::IsDerivedFromSpecializationOfV<T, TriangleBitFlags>;

/**
 * @brief A concept that checks whether a type T (that should be a Face Element)
 * has either the PolygonBitFlags or the TriangleBitFlags component
 * (inherits from one of them).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::comp::PolygonBitFlags or vcl::comp::TriangleBitFlags, with any template
 * arguments.
 *
 * @tparam T: The type to be tested for conformity to the HasFaceBitFlags.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasFaceBitFlags = HasPolygonBitFlags<T> || HasTriangleBitFlags<T>;

/**
 * @brief A concept that checks whether a type T (that should be an Element)
 * has the one of the BitFlags components (inherits from one of them).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::comp::BitFlags, or vcl::comp::PolygonBitFlags, or
 * vcl::comp::TriangleBitFlags, with any template arguments.
 *
 * @tparam T: The type to be tested for conformity to the HasBitFlags.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasBitFlags =
    HasFaceBitFlags<T> || TB::IsDerivedFromSpecializationOfV<T, BitFlags>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_BIT_FLAGS_H
