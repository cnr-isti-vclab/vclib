// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_BIT_FLAGS_H
#define VCL_MESH_COMPONENTS_CONCEPTS_BIT_FLAGS_H

#include "../base/predicates.h"

#include <vclib/base.h>
#include <vclib/space/core.h>

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
