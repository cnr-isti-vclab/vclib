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

#ifndef VCL_MESH_ELEMENTS_CONCEPTS_VERTEX_H
#define VCL_MESH_ELEMENTS_CONCEPTS_VERTEX_H

#include "vertex_components.h"

namespace vcl {

template<typename...>
class Vertex;

namespace internal {

// check if a type derives from Vertex<T...>
template<typename Derived, typename... Args>
using IsDerivedFromVertex =
	typename vcl::IsDerivedFromTemplateSpecialization<Derived, Vertex<Args...>>::type;

// check if a type is a Vertex<T...>
template<class T>
struct isAVertex : // Default case, no pattern match
		std::false_type
{
};

template<class... T>
struct isAVertex<Vertex<T...>> : // For types matching the pattern Vertex<T...>
		std::true_type
{
};

} // namespace vcl::internal

template<typename T>
concept VertexConcept = internal::IsDerivedFromVertex<T>::value || internal::isAVertex<T>::value;

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_CONCEPTS_VERTEX_H
