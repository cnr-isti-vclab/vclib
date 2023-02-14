/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#ifndef VCL_MESH_ELEMENTS_EDGE_CONCEPT_H
#define VCL_MESH_ELEMENTS_EDGE_CONCEPT_H

#include "edge_components.h"

namespace vcl {

template<typename, typename...>
class Edge;

namespace edge {

// checks if a type derives from vcl::Edge<Args...>
template<typename Derived>
using IsDerivedFromEdge = IsDerivedFromTemplateSpecialization<Derived, Edge>;

// checks if a type is an vcl::Edge<Args...>
template<class T>
struct IsAnEdge : // Default case, no pattern match
		std::false_type
{
};

template<class... Args>
struct IsAnEdge<Edge<Args...>> : // For types matching the pattern Edge<Args...>
		std::true_type
{
};

/* Port concepts into the vert namespace */
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
concept HasScalar = comp::HasScalar<T>;
template<typename T>
concept HasOptionalScalar = comp::HasOptionalScalar<T>;
template<typename T>
concept HasVertexReferences = comp::HasVertexReferences<T>;

} // namespace vcl::edge

/**
 * @brief EdgeConcept
 *
 * The Edge concept describes how a Edge element that can be used for a EdgeContainer should be
 * organized.
 *
 * The Edge concept is satisfied for a class E if ALL the following sentences are true:
 * - The class E is vcl::Edge, or derives from it;
 * - The class E has the BitFlags component (or a derivate);
 * - The class E has the VertexReferences component (or a derivate);
 * - The number of vertices of the VertexReferences is 2.
 */
template<typename T>
concept EdgeConcept =
	(edge::IsDerivedFromEdge<T>::value || edge::IsAnEdge<T>::value) &&
	edge::HasBitFlags<T> &&
	edge::HasVertexReferences<T> &&
	T::VERTEX_NUMBER == 2;

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_EDGE_CONCEPT_H
