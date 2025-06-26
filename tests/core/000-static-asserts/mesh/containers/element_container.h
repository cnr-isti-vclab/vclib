/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef CONT_ELEMENT_CONTAINER_H
#define CONT_ELEMENT_CONTAINER_H

#include <vclib/meshes.h>

void elementContainerStaticAsserts()
{
    using namespace vcl;

    using VertexContainer =
        mesh::VertexContainer<trimesh::Vertex<float, false>>;
    using FaceContainer = mesh::FaceContainer<trimesh::Face<float, false>>;
    using EdgeContainer = mesh::EdgeContainer<edgemesh::Edge<float, false>>;

    static_assert(
        mesh::ElementContainerConcept<VertexContainer>,
        "VertexContainer does not satisfy the ElementContainerConcept");
    static_assert(
        mesh::ElementContainerConcept<const VertexContainer>,
        "const VertexContainer does not satisfy the ElementContainerConcept");
    static_assert(
        mesh::ElementContainerConcept<VertexContainer&>,
        "VertexContainer& does not satisfy the ElementContainerConcept");
    static_assert(
        mesh::ElementContainerConcept<const VertexContainer&>,
        "const VertexContainer& does not satisfy the ElementContainerConcept");
    static_assert(
        mesh::ElementContainerConcept<VertexContainer&&>,
        "VertexContainer&& does not satisfy the ElementContainerConcept");

    static_assert(
        std::random_access_iterator<VertexContainer::VertexIterator>,
        "The VertexIterator of VertexContainer is not a random access "
        "iterator");

    static_assert(
        mesh::ElementContainerConcept<FaceContainer>,
        "FaceContainer does not satisfy the ElementContainerConcept");
    static_assert(
        mesh::ElementContainerConcept<const FaceContainer>,
        "const FaceContainer does not satisfy the ElementContainerConcept");
    static_assert(
        mesh::ElementContainerConcept<FaceContainer&>,
        "FaceContainer& does not satisfy the ElementContainerConcept");
    static_assert(
        mesh::ElementContainerConcept<const FaceContainer&>,
        "const FaceContainer& does not satisfy the ElementContainerConcept");
    static_assert(
        mesh::ElementContainerConcept<FaceContainer&&>,
        "FaceContainer&& does not satisfy the ElementContainerConcept");

    static_assert(
        mesh::ElementContainerConcept<EdgeContainer>,
        "EdgeContainer does not satisfy the ElementContainerConcept");
    static_assert(
        mesh::ElementContainerConcept<const EdgeContainer>,
        "const EdgeContainer does not satisfy the ElementContainerConcept");
    static_assert(
        mesh::ElementContainerConcept<EdgeContainer&>,
        "EdgeContainer& does not satisfy the ElementContainerConcept");
    static_assert(
        mesh::ElementContainerConcept<const EdgeContainer&>,
        "const EdgeContainer& does not satisfy the ElementContainerConcept");
    static_assert(
        mesh::ElementContainerConcept<EdgeContainer&&>,
        "EdgeContainer&& does not satisfy the ElementContainerConcept");
}

#endif // CONT_ELEMENT_CONTAINER_H
