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

} // namespace vcl::internal

template<typename T>
concept HalfEdgeConcept = hedge::IsDerivedFromHalfEdge<T>::value || hedge::IsAHalfEdge<T>::value;

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_HALF_EDGE_CONCEPT_H
