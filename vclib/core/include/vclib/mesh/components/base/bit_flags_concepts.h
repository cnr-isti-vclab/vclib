// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_BASE_BIT_FLAGS_CONCEPTS_H
#define VCL_MESH_COMPONENTS_BASE_BIT_FLAGS_CONCEPTS_H

#include "component_predicates.h"

namespace vcl::comp {

class BitFlags;
class TriangleBitFlags;

template<int N>
class PolygonBitFlags;

namespace detail {
template<typename T>
struct IsPolygonBitFlags : std::false_type
{
};

template<int N>
struct IsPolygonBitFlags<PolygonBitFlags<N>> : std::true_type
{
};
} // namespace detail

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the
 * BitFlags component (COMP_ID = CompId::BIT_FLAGS).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasBitFlags = HasComponent<CompId::BIT_FLAGS, T>;

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the
 * TriangleBitFlags component.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasTriangleBitFlags =
    HasComponent<CompId::BIT_FLAGS, T> &&
    std::is_same_v<ComponentTypeT<CompId::BIT_FLAGS, T>, TriangleBitFlags>;

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the
 * PolygonBitFlags component.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasPolygonBitFlags =
    HasComponent<CompId::BIT_FLAGS, T> &&
    detail::IsPolygonBitFlags<ComponentTypeT<CompId::BIT_FLAGS, T>>::value;

/**
 * @brief Concept that checks whether a type T has either TriangleBitFlags or
 * PolygonBitFlags.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasFaceBitFlags = HasPolygonBitFlags<T> || HasTriangleBitFlags<T>;

/**
 * @brief Concept that checks whether an element type T has a parent container
 * that can adjust its active element count when deleted status changes.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept CanAdjustContainerElemCount = requires(T& elem, bool b1, bool b2) {
    elem.parentContainer().adjustElemCountOnDeletedChange(b1, b2);
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BASE_BIT_FLAGS_CONCEPTS_H
