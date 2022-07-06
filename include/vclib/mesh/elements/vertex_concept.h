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

#ifndef VCL_MESH_ELEMENTS_VERTEX_CONCEPT_H
#define VCL_MESH_ELEMENTS_VERTEX_CONCEPT_H

#include "vertex_components.h"

namespace vcl {

template<typename...>
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

/* Port concepts into the vert namespace */
template<typename T>
concept HasAdjacentEdges = comp::HasAdjacentEdges<T>;
template<typename T>
concept HasAdjacentFaces = comp::HasAdjacentFaces<T>;
template<typename T>
concept HasAdjacentVertices = comp::HasAdjacentVertices<T>;
template<typename T>
concept HasBitFlags = comp::HasBitFlags<T>;
template<typename T>
concept HasColor = comp::HasColor<T>;
template<typename T>
concept HasCoordinate = comp::HasCoordinate<T>;
template<typename T>
concept HasHalfEdgeReference = comp::HasVertexHalfEdgeReference<T>;
template<typename T>
concept HasMark = comp::HasMark<T>;
template<typename T>
concept HasNormal = comp::HasNormal<T>;
template<typename T>
concept HasPrincipalCurvature = comp::HasPrincipalCurvature<T>;
template<typename T>
concept HasScalar = comp::HasScalar<T>;
template<typename T>
concept HasTexCoord = comp::HasTexCoord<T>;

} // namespace vcl::vert

/**
 * @brief VertexConcept is the concept that defines a Vertex class that can be given
 * as a template of the VertexContainer class.
 *
 * To satisfy this concept, a class must:
 * - be a vcl::Vertex<...> class, or derived from it
 * - have the following components:
 *   - BitFlags (or a derivate)   -> HasBitFlags concept
 *   - Coordinate (or a derivate) -> HasCoordinate concept
 */
template<typename T>
concept VertexConcept =
	(vert::IsDerivedFromVertex<T>::value || vert::IsAVertex<T>::value) &&
	vert::HasBitFlags<T> &&
	vert::HasCoordinate<T>;

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_VERTEX_CONCEPT_H
