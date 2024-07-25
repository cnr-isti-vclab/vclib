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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_BIT_FLAGS_H
#define VCL_CONCEPTS_MESH_COMPONENTS_BIT_FLAGS_H

#include <concepts>

#include <vclib/types/base.h>

namespace vcl::comp {

/**
 * @brief HasBitFlags concept is satisfied only if a Element class provides the
 * member functions specified in this concept. These member functions allows to
 * access to a BitFlag component of a given element.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasBitFlags = requires (T o, const T& co) {
    // clang-format off
    { co.deleted() } -> std::same_as<bool>;
    { co.selected() } -> std::same_as<bool>;
    { co.onBorder() } -> std::same_as<bool>;
    { co.selected() } -> std::same_as<bool>;
    { co.userBit(uint()) } -> std::same_as<bool>;

    { o.resetBitFlags() } -> std::same_as<void>;
    { o.importFlagsFromVCGFormat(int())} -> std::same_as<void>;
    { co.exportFlagsToVCGFormat() } -> std::same_as<int>;
    // clang-format on
};

namespace detail {

/**
 * @private
 * @brief Concept for internal use - contains all the flags member functions
 * that are contained on both the BitFlags components for Face Elements.
 */
template<typename T>
concept FaceBitFlagsConcept = HasBitFlags<T> && requires (T o, const T& co) {
    // clang-format off
    { co.edgeOnBorder(uint()) } -> std::same_as<bool>;
    { co.edgeSelected(uint()) } -> std::same_as<bool>;
    { co.edgeVisited(uint()) } -> std::same_as<bool>;
    { co.edgeFaux(uint()) } -> std::same_as<bool>;
    // clang-format on
};

} // namespace detail

/**
 * @brief HasPolygonBitFlags concept is satisfied only if a Element class (that
 * should be a Face) provides the member functions specified in this concept.
 * These member functions allows to access to PolygonBitFlags component of a
 * given element.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasPolygonBitFlags = detail::FaceBitFlagsConcept<T> && requires (T o) {
    // clang-format off
    { o.__polygonBitFlags() } -> std::same_as<void>;
    // clang-format on
};

/**
 * @brief HasTriangleBitFlags concept is satisfied only if a Element class (that
 * should be a Face) provides the member functions specified in this concept.
 * These member functions allows to access to TriangleBitFlags component of a
 * given element.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasTriangleBitFlags = detail::FaceBitFlagsConcept<T> && requires (T o) {
    // clang-format off
    { o.__triangleBitFlags() } -> std::same_as<void>;
    // clang-format on
};

/**
 * @brief HasFaceBitFlags concept is satisfied if one between HasPolygonBitFlags
 * and HasTriangleBitFlags concept is satisfied. This concept allows to make
 * sure that a Face element has proper FaceBitFlags (Triangle or Polygon).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasFaceBitFlags = HasPolygonBitFlags<T> || HasTriangleBitFlags<T>;

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_BIT_FLAGS_H
