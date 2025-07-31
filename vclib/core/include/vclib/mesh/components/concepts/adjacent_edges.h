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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_EDGES_H
#define VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_EDGES_H

#include "component.h"
#include "predicates.h"

#include <vclib/types.h>

namespace vcl::comp {

template<bool, typename, int, bool, typename, bool, bool>
class AdjacentEdges;

/**
 * @brief A concept that checks whether a type T (that should be a Element) has
 * the AdjacentEdges component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::comp::AdjacentEdges, with any template arguments.
 *
 * Note that this concept does not discriminate between the Horizontal
 * AdjacentEdges component and the vertical OptionalAdjacentEdges component,
 * therefore it does not guarantee that a template Element type that satisfies
 * this concept provides AdjacentEdges component at runtime (it is guaranteed
 * only that the proper member functions are available at compile time).
 *
 * @tparam T: The type to be tested for conformity to the HasAdjacentEdges.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasAdjacentEdges =
    BTIBTBB::IsDerivedFromSpecializationOfV<T, AdjacentEdges>;

/**
 * @brief A concept that checks whether a type T (that should be a Element) has
 * the AdjacentEdges component (inherits from it), and that the component is
 * optional.
 *
 * @tparam T: The type to be tested for conformity to the
 * HasOptionalAdjacentEdges.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalAdjacentEdges =
    HasAdjacentEdges<T> &&
    IsOptionalComponent<typename RemoveRef<T>::AdjacentEdges>;

/**
 * @private
 * @brief HasRightNumberOfAdjacentEdges concept
 *
 * This concept is designed to be used with Face components, where the number of
 * adjacent edges, if tied to the vertex number, must be consistent w.r.t. the
 * number of vertices of the face.
 *
 * This concept is satisfied only if static number of adjacent edges is the same
 * of the static number of vertices.
 */
template<typename T>
concept HasRightNumberOfAdjacentEdges =
    !comp::IsTiedToVertexNumber<typename RemoveRef<T>::AdjacentEdges> ||
    RemoveRef<T>::VERTEX_NUMBER == RemoveRef<T>::ADJ_EDGE_NUMBER;

/**
 * @private
 * @brief SanityCheckAdjacentEdges concept
 *
 * This concept is designed to be used with Face components, where the number of
 * adjacent edges must be consistent w.r.t. the number of vertices of the face.
 *
 * It is satisfied if:
 * - the component does *not* have adjacent edges, or
 * - in case it has adjacent edges, they have the same number of vertices of the
 * face.
 */
template<typename T>
concept SanityCheckAdjacentEdges =
    !HasAdjacentEdges<T> || HasRightNumberOfAdjacentEdges<T>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_EDGES_H
