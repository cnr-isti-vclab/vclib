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

#ifndef VCL_CONCEPTS_MESH_ELEMENTS_VERTEX_H
#define VCL_CONCEPTS_MESH_ELEMENTS_VERTEX_H

#include <vclib/concepts/mesh/components.h>

#include "element.h"

/**
 * @defgroup vert_concepts Vertex Concepts
 * @ingroup element_concepts
 *
 * @brief List of concepts for types related to the Vertex Element.
 */

namespace vcl {

template<typename, typename...>
class Vertex;

namespace vert {

// checks if a type derives from vcl::Vertex<Args...>
template<typename Derived>
using IsDerivedFromVertex = IsDerivedFromTemplateSpecialization<Derived, Vertex>;

// checks if a type is a vcl::Vertex<Args...>
template<class T>
struct IsAVertex : // Default case, no pattern match
		std::false_type
{
};

template<class... Args> // note: here the templated types are the components of the Vertex
struct IsAVertex<Vertex<Args...>> : // For types matching the pattern Vertex<Args...>
		std::true_type
{
};

/**
 * @ingroup vert_concepts vert_components
 * @{
 */

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
concept HasAdjacentVertices = comp::HasAdjacentVertices<T>;
template<typename T>
concept HasOptionalAdjacentVertices = comp::HasOptionalAdjacentVertices<T>;
template<typename T>
concept HasBitFlags = comp::HasBitFlags<T>;
template<typename T>
concept HasColor = comp::HasColor<T>;
template<typename T>
concept HasOptionalColor = comp::HasOptionalColor<T>;
template<typename T>
concept HasCustomComponents = comp::HasCustomComponents<T>;
template<typename T>
concept HasCoordinate = comp::HasCoordinate<T>;
template<typename T>
concept HasHalfEdgePointer = comp::HasVertexHalfEdgePointer<T>;
template<typename T>
concept HasMark = comp::HasMark<T>;
template<typename T>
concept HasOptionalMark = comp::HasOptionalMark<T>;
template<typename T>
concept HasNormal = comp::HasNormal<T>;
template<typename T>
concept HasOptionalNormal = comp::HasOptionalNormal<T>;
template<typename T>
concept HasPrincipalCurvature = comp::HasPrincipalCurvature<T>;
template<typename T>
concept HasOptionalPrincipalCurvature = comp::HasOptionalPrincipalCurvature<T>;
template<typename T>
concept HasQuality = comp::HasQuality<T>;
template<typename T>
concept HasOptionalQuality = comp::HasOptionalQuality<T>;
template<typename T>
concept HasTexCoord = comp::HasTexCoord<T>;
template<typename T>
concept HasOptionalTexCoord = comp::HasOptionalTexCoord<T>;

/** @} */ // end of group

} // namespace vcl::vert

/**
 * @brief The VertexConcept describes how a Vertex element that can be used for a VertexContainer
 * should be organized.
 *
 * The Vertex concept is satisfied for a class V if ALL the following sentences are true:
 * - The class V is a vcl::Vertex, or derives from it;
 * - The class V has the BitFlags component (or a derivate);
 * - The class V has the Coordinate component (or a derivate);
 *
 * @ingroup vert_concepts
 */
template<typename T>
concept VertexConcept =
	T::ELEMENT_TYPE == VERTEX &&
	(vert::IsDerivedFromVertex<T>::value || vert::IsAVertex<T>::value) &&
	vert::HasBitFlags<T> &&
	vert::HasCoordinate<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_ELEMENTS_VERTEX_H
