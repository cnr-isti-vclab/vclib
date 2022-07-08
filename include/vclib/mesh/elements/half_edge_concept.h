/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_MESH_ELEMENTS_HALF_EDGE_CONCEPT_H
#define VCL_MESH_ELEMENTS_HALF_EDGE_CONCEPT_H

#include "half_edge_components.h"
#include "half_edge_components_optional.h"

namespace vcl {

template<typename...>
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

/* Port concepts into the vert namespace */
template<typename T>
concept HasBitFlags = comp::HasBitFlags<T>;
template<typename T>
concept HasColor = comp::HasColor<T>;
template<typename T>
concept HasHalfEdgeReferences = comp::HasHalfEdgeReferences<T>;
template<typename T>
concept HasMark = comp::HasMark<T>;
template<typename T>
concept HasScalar = comp::HasScalar<T>;
template<typename T>
concept HasVerticalComponent = comp::HasVerticalComponent<T>;

} // namespace vcl::hedge

/**
 * @brief HalfEdgeConcept
 *
 * The HalfEdge concept describes how a HalfEdge element that can be used for a HalfEdgeContainer
 * should be organized.
 *
 * The HalfEdge concept is satisfied for a class HE if ALL the following sentences are true:
 * - The class HE is a vcl::HalfEdge, or derives from it;
 * - The class HE has the BitFlags component (or a derivate);
 * - The class HE has the HalfEdgeReferences component (or a derivate);
 */
template<typename T>
concept HalfEdgeConcept =
	hedge::IsDerivedFromHalfEdge<T>::value || hedge::IsAHalfEdge<T>::value &&
	hedge::HasBitFlags<T> &&
	hedge::HasHalfEdgeReferences<T>;

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_HALF_EDGE_CONCEPT_H
