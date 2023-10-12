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

#ifndef VCL_CONCEPTS_MESH_MESH_CONCEPT_H
#define VCL_CONCEPTS_MESH_MESH_CONCEPT_H

#include "containers.h"
#include "elements/element.h"

namespace vcl {

template<typename... Args>
class Mesh;

namespace mesh {

// checks if a type derives from vcl::Mesh<Args...>
template<typename Derived>
using IsDerivedFromMesh = IsDerivedFromTemplateSpecialization<Derived, Mesh>;

// checks if a type is a vcl::Mesh<Args...>
// Default case, no pattern match
template<class T>
struct IsAMesh : std::false_type
{
};

// For types matching the pattern Mesh<Args...>
template<class... Args>
struct IsAMesh<Mesh<Args...>> : std::true_type
{
};

} // namespace mesh

/**
 * @brief The Mesh Concept is evaluated to true when the type is a Mesh.
 *
 * A type T is a Mesh whem it inherits from or it is a template specialization
 * of the vcl::Mesh class, and it contains a VertexContainer, which is the only
 * container that is mandatory in a vcl::Mesh.
 *
 * @ingroup mesh_concepts
 */
template<typename T>
concept MeshConcept =
    (mesh::IsDerivedFromMesh<T>::value || mesh::IsAMesh<T>::value) &&
    mesh::HasVertexContainer<T> && requires (T o, const T& co) {
        // clang-format off
        { o.clear() } -> std::same_as<void>;
        { co.isCompact() } -> std::same_as<bool>;
        { o.compact() } -> std::same_as<void>;
        { o.enableSameOptionalComponentsOf(T()) } -> std::same_as<void>;
        { o.importFrom(T()) } -> std::same_as<void>;
        { o.swap(o) } -> std::same_as<void>;
        // clang-format on
    };

/**
 * @brief the ElementOrMesh Concept is evaluated to true when the type is either
 * a Mesh or an Element.
 *
 * @ingroup mesh_concepts
 */
template<typename T>
concept ElementOrMeshConcept = MeshConcept<T> || ElementConcept<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_MESH_CONCEPT_H
