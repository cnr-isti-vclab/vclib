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

#ifndef VCL_MESH_CONCEPT_H
#define VCL_MESH_CONCEPT_H

#include "containers/containers_concepts.h"
#include "mesh_components.h"

namespace vcl {

template<typename...>
class Mesh;

namespace mesh {

// checks if a type derives from vcl::Mesh<Args...>
template<typename Derived>
using IsDerivedFromMesh = IsDerivedFromTemplateSpecialization<Derived, Mesh>;

// checks if a type is a vcl::Mesh<Args...>
template<class T>
struct IsAMesh : // Default case, no pattern match
		 std::false_type
{
};

template<class... Args>
struct IsAMesh<Mesh<Args...>> : // For types matching the pattern Mesh<Args...>
		std::true_type
{
};

/* Port concepts into the mesh namespace */
template<typename T>
concept HasBoundingBox = comp::HasBoundingBox<T>;
template<typename T>
concept HasMark = comp::HasMark<T>;
template<typename T>
concept HasTexFileNames = comp::HasTexFileNames<T>;
template<typename T>
concept HasTransformMatrix = comp::HasTransformMatrix<T>;

} // namespace mesh

} // namespace vcl

#endif // VCL_MESH_CONCEPT_H
