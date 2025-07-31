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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_FACES_H
#define VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_FACES_H

#include "component.h"
#include "predicates.h"

#include <vclib/types.h>

namespace vcl::comp {

template<bool, typename, int, bool, typename, bool, bool>
class AdjacentFaces;

/**
 * @brief A concept that checks whether a type T (that should be a Element) has
 * the AdjacentFaces component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::comp::AdjacentFaces, with any template arguments.
 *
 * Note that this concept does not discriminate between the Horizontal
 * AdjacentFaces component and the vertical OptionalAdjacentFaces component,
 * therefore it does not guarantee that a template Element type that satisfies
 * this concept provides AdjacentFaces component at runtime (it is guaranteed
 * only that the proper member functions are available at compile time).
 *
 * @tparam T: The type to be tested for conformity to the HasAdjacentFaces.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasAdjacentFaces =
    BTIBTBB::IsDerivedFromSpecializationOfV<T, AdjacentFaces>;

/**
 * @brief A concept that checks whether a type T (that should be a Element) has
 * the AdjacentFaces component (inherits from it), and that the component is
 * optional.
 *
 * @tparam T: The type to be tested for conformity to the
 * HasOptionalAdjacentFaces.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalAdjacentFaces =
    HasAdjacentFaces<T> &&
    IsOptionalComponent<typename RemoveRef<T>::AdjacentFaces>;

/**
 * @private
 * @brief HasRightNumberOfAdjacentFaces concept
 *
 * This concept is designed to be used with Face components, where the number of
 * adjacent faces, if tied to the vertex number, must be consisted w.r.t. the
 * number of vertices of the face.
 *
 * This concept is satisfied only if static number of adjacent faces is the same
 * of the static number of vertices.
 */
template<typename T>
concept HasRightNumberOfAdjacentFaces =
    !comp::IsTiedToVertexNumber<typename RemoveRef<T>::AdjacentFaces> ||
    RemoveRef<T>::VERTEX_NUMBER == RemoveRef<T>::ADJ_FACE_NUMBER;

/**
 * @private
 * @brief SanityCheckAdjacentFaces concept
 *
 * This concept is designed to be used with Face components, where the number of
 * adjacent faces must be consistent w.r.t. the number of vertices of the face.
 *
 * It is satisfied if:
 * - the component does *not* have adjacent faces;
 * - in case it has adjacent faces, they have the same number of vertices of the
 * face.
 */
template<typename T>
concept SanityCheckAdjacentFaces =
    !HasAdjacentFaces<T> || HasRightNumberOfAdjacentFaces<T>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_FACES_H
