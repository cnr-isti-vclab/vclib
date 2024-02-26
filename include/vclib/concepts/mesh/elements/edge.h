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

#ifndef VCL_CONCEPTS_MESH_ELEMENTS_EDGE_H
#define VCL_CONCEPTS_MESH_ELEMENTS_EDGE_H

#include <vclib/concepts/mesh/components.h>

#include "element.h"

/**
 * @defgroup edge_concepts Edge Concepts
 * @ingroup element_concepts
 *
 * @brief List of concepts for types related to the Edge Element.
 */

namespace vcl {

template<typename, typename...>
class Edge;

namespace edge {

// checks if a type derives from vcl::Edge<Args...>
template<typename Derived>
using IsDerivedFromEdge = IsDerivedFromTemplateSpecialization<Derived, Edge>;

// checks if a type is a vcl::Edge<Args...>
template<class T>
struct IsAEdge : // Default case, no pattern match
        std::false_type
{
};

template<class... Args>
struct IsAEdge<Edge<Args...>> : // For types matching the pattern Edge<Args...>
        std::true_type
{
};

/**
 * @ingroup edge_concepts edge_components
 * @{
 */

/* Port concepts into the edge namespace */
template<typename T>
concept HasAdjacentEdges = comp::HasAdjacentEdges<T>;
template<typename T>
concept HasOptionalAdjacentEdges = comp::HasOptionalAdjacentEdges<T>;
template<typename T>
concept HasAdjacentFaces = comp::HasAdjacentFaces<T>;
template<typename T>
concept HasOptionalAdjacentFaces = comp::HasOptionalAdjacentFaces<T>;
template<typename T>
concept HasBitFlags = comp::HasBitFlags<T>;
template<typename T>
concept HasColor = comp::HasColor<T>;
template<typename T>
concept HasOptionalColor = comp::HasOptionalColor<T>;
template<typename T>
concept HasCustomComponents = comp::HasCustomComponents<T>;
template<typename T>
concept HasMark = comp::HasMark<T>;
template<typename T>
concept HasOptionalMark = comp::HasOptionalMark<T>;
template<typename T>
concept HasQuality = comp::HasQuality<T>;
template<typename T>
concept HasOptionalQuality = comp::HasOptionalQuality<T>;
template<typename T>
concept HasVertexPointers = comp::HasVertexPointers<T>;

/** @} */ // end of group

} // namespace edge

/**
 * @brief The EdgeConcept describes how a Edge element that can be
 * used for a EdgeContainer should be organized.
 *
 * The Edge concept is satisfied for a class E if ALL the following
 * sentences are true:
 * - The class E is vcl::Edge, or derives from it;
 * - The class E has the BitFlags component (or an equivalent one);
 * - The class E has the VertexPointers component (or a derivate);
 * - The number of vertices of the VertexPointers is 2.
 *
 * @ingroup edge_concepts
 */
template<typename T>
concept EdgeConcept =
    ElementConcept<T> && T::ELEMENT_ID == ElemId::EDGE &&
    (edge::IsDerivedFromEdge<T>::value || edge::IsAEdge<T>::value) &&
    edge::HasBitFlags<T> && edge::HasVertexPointers<T> && T::VERTEX_NUMBER == 2;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_ELEMENTS_EDGE_H
