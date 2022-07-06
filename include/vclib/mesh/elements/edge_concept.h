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

#ifndef VCL_MESH_ELEMENTS_EDGE_CONCEPT_H
#define VCL_MESH_ELEMENTS_EDGE_CONCEPT_H

#include "edge_components.h"

namespace vcl {

template<typename...>
class Edge;

namespace internal {

// check if a type derives from Edge<T...>
template<typename Derived>
using IsDerivedFromEdge = IsDerivedFromTemplateSpecialization<Derived, Edge>;

// check if a type is an Edge<T...>
template<class T>
struct IsAnEdge : // Default case, no pattern match
		std::false_type
{
};

template<class... T>
struct IsAnEdge<Edge<T...>> : // For types matching the pattern Edge<T...>
		std::true_type
{
};

} // namespace internal

template<typename T>
concept EdgeConcept = internal::IsDerivedFromEdge<T>::value || internal::IsAnEdge<T>::value;

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_EDGE_CONCEPT_H
