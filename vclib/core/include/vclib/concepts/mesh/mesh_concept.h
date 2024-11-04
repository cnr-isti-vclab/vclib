/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_CONCEPTS_MESH_MESH_CONCEPT_H
#define VCL_CONCEPTS_MESH_MESH_CONCEPT_H

#include "containers.h"
#include "elements/element.h"

namespace vcl {

/**
 * @brief The Mesh Concept is evaluated to true when the type is a Mesh.
 *
 * A type T is a Mesh when it contains a VertexContainer, which is the only
 * container that is mandatory in a vcl::Mesh, and provides:
 *
 * - A type definition called Containers, which is a TypeWrapper that contains
 *   all the containers that the Mesh has.
 * - A type definition called Components, which is a TypeWrapper that contains
 *   all the components that the Mesh has.
 * - A method called clear() that clears the mesh.
 * - A method called isCompact() that returns a boolean indicating if the mesh
 *   is compact.
 * - A method called compact() that compacts the mesh.
 * - A method called enableSameOptionalComponentsOf() that enables the same
 *   optional components of another mesh.
 * - A method called importFrom() that imports the mesh from another mesh.
 * - A method called swap() that swaps the mesh with another mesh.
 *
 * @ingroup mesh_concepts
 */
template<typename T>
concept MeshConcept =
    mesh::HasVertexContainer<T> && requires (T o, const T& co) {
        typename T::Containers;
        typename T::Components;

        { o.clear() } -> std::same_as<void>;
        { co.isCompact() } -> std::same_as<bool>;
        { o.compact() } -> std::same_as<void>;
        { o.enableSameOptionalComponentsOf(T()) } -> std::same_as<void>;
        { o.importFrom(T()) } -> std::same_as<void>;
        { o.swap(o) } -> std::same_as<void>;
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
