// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
