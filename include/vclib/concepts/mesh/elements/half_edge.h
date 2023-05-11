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

#ifndef VCL_CONCEPTS_MESH_ELEMENTS_HALF_EDGE_H
#define VCL_CONCEPTS_MESH_ELEMENTS_HALF_EDGE_H

#include <vclib/concepts/mesh/components.h>

#include "element.h"

/**
 * @defgroup hedge_concepts HalfEdge Concepts
 * @ingroup element_concepts
 *
 * @brief List of concepts for types related to the HalfEdge Element.
 */

namespace vcl {

template<typename, typename...>
class HalfEdge;

namespace hedge {

// checks if a type derives from vcl::HalfEdge<Args...>
template<typename Derived>
using IsDerivedFromHalfEdge = IsDerivedFromTemplateSpecialization<Derived, HalfEdge>;

// checks if a type is a vcl::HalfEdge<Args...>
template<class T>
struct IsAHalfEdge : // Default case, no pattern match
		std::false_type
{
};

template<class... Args>
struct IsAHalfEdge<HalfEdge<Args...>> : // For types matching the pattern HalfEdge<Args...>
		std::true_type
{
};

/**
 * @ingroup hedge_concepts hedge_components
 * @{
 */

/* Port concepts into the hedge namespace */
template<typename T>
concept HasBitFlags = comp::HasBitFlags<T>;
template<typename T>
concept HasColor = comp::HasColor<T>;
template<typename T>
concept HasOptionalColor = comp::HasOptionalColor<T>;
template<typename T>
concept HasCustomComponents = comp::HasCustomComponents<T>;
template<typename T>
concept HasHalfEdgePointers = comp::HasHalfEdgePointers<T>;
template<typename T>
concept HasMark = comp::HasMark<T>;
template<typename T>
concept HasOptionalMark = comp::HasOptionalMark<T>;
template<typename T>
concept HasQuality = comp::HasQuality<T>;
template<typename T>
concept HasOptionalQuality = comp::HasOptionalQuality<T>;
template<typename T>
concept HasTexCoord = comp::HasTexCoord<T>;
template<typename T>
concept HasOptionalTexCoord = comp::HasOptionalTexCoord<T>;


/** @} */ // end of group

} // namespace vcl::hedge

/**
 * @brief The HalfEdgeConcept describes how a HalfEdge element that can be used for a
 * HalfEdgeContainer should be organized.
 *
 * The HalfEdge concept is satisfied for a class HE if ALL the following sentences are true:
 * - The class HE is a vcl::HalfEdge, or derives from it;
 * - The class HE has the BitFlags component (or a derivate);
 * - The class HE has the HalfEdgePointers component (or a derivate);
 *
 * @ingroup hedge_concepts
 */
template<typename T>
concept HalfEdgeConcept =
	T::ELEMENT_TYPE == HALF_EDGE &&
	(hedge::IsDerivedFromHalfEdge<T>::value || hedge::IsAHalfEdge<T>::value) &&
	hedge::HasBitFlags<T> &&
	hedge::HasHalfEdgePointers<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_ELEMENTS_HALF_EDGE_H
