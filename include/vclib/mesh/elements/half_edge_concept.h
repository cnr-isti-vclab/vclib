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
class Halfedge;

namespace internal {

// check if a type derives from Halfedge<T...>
template<typename Derived, typename... Args>
using IsDerivedFromHalfedge =
	typename vcl::IsDerivedFromTemplateSpecialization<Derived, Halfedge<Args...>>::type;

// check if a type is a Halfedge<T...>
template<class T>
struct isAHalfedge : // Default case, no pattern match
		std::false_type
{
};

template<class... T>
struct isAHalfedge<Halfedge<T...>> : // For types matching the pattern Halfedge<T...>
		std::true_type
{
};

} // namespace vcl::internal

template<typename T>
concept HalfedgeConcept = internal::IsDerivedFromHalfedge<T>::value || internal::isAHalfedge<T>::value;

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_HALF_EDGE_CONCEPT_H
