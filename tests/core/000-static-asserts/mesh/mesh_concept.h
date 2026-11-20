// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MESH_CONCEPT_H
#define MESH_CONCEPT_H

#include <vclib/meshes.h>

void meshConceptStaticAsserts()
{
    using namespace vcl;

    using TMVertex = TriMesh::Vertex;

    static_assert(
        HasContainerOf<TriMesh, ElemId::VERTEX>,
        "TriMesh does not satisfy the HasContainerOf VERTEX");
    static_assert(
        HasContainerOf<const TriMesh, ElemId::VERTEX>,
        "const TriMesh does not satisfy the HasContainerOf VERTEX");
    static_assert(
        HasContainerOf<TriMesh&, ElemId::VERTEX>,
        "TriMesh& does not satisfy the HasContainerOf VERTEX");
    static_assert(
        HasContainerOf<const TriMesh&, ElemId::VERTEX>,
        "const TriMesh& does not satisfy the HasContainerOf VERTEX");
    static_assert(
        HasContainerOf<TriMesh&&, ElemId::VERTEX>,
        "TriMesh&& does not satisfy the HasContainerOf VERTEX");

    static_assert(
        !HasContainerOf<TriMesh, ElemId::EDGE>,
        "TriMesh does satisfy the HasContainerOf EDGE");
    static_assert(
        !HasContainerOf<const TriMesh, ElemId::EDGE>,
        "const TriMesh does satisfy the HasContainerOf EDGE");
    static_assert(
        !HasContainerOf<TriMesh&, ElemId::EDGE>,
        "TriMesh& does satisfy the HasContainerOf EDGE");
    static_assert(
        !HasContainerOf<const TriMesh&, ElemId::EDGE>,
        "const TriMesh& does satisfy the HasContainerOf EDGE");
    static_assert(
        !HasContainerOf<TriMesh&&, ElemId::EDGE>,
        "TriMesh&& does satisfy the HasContainerOf EDGE");

    static_assert(
        HasElementComponent<TriMesh, ElemId::VERTEX, CompId::POSITION>,
        "TriMesh does not satisfy the HasElementComponent "
        "VERTEX-POSITION");
    static_assert(
        HasElementComponent<const TriMesh, ElemId::VERTEX, CompId::POSITION>,
        "const TriMesh does not satisfy the HasElementComponent "
        "VERTEX-POSITION");
    static_assert(
        HasElementComponent<TriMesh&, ElemId::VERTEX, CompId::POSITION>,
        "TriMesh& does not satisfy the HasElementComponent "
        "VERTEX-POSITION");
    static_assert(
        HasElementComponent<const TriMesh&, ElemId::VERTEX, CompId::POSITION>,
        "const TriMesh& does not satisfy the HasElementComponent "
        "VERTEX-POSITION");
    static_assert(
        HasElementComponent<TriMesh&&, ElemId::VERTEX, CompId::POSITION>,
        "TriMesh&& does not satisfy the HasElementComponent "
        "VERTEX-POSITION");

    static_assert(
        !HasElementComponent<TriMesh, ElemId::VERTEX, CompId::WEDGE_COLORS>,
        "TriMesh does satisfy the HasElementComponent "
        "VERTEX-WEDGE_COLORS");
    static_assert(
        !HasElementComponent<
            const TriMesh,
            ElemId::VERTEX,
            CompId::WEDGE_COLORS>,
        "const TriMesh does satisfy the HasElementComponent "
        "VERTEX-WEDGE_COLORS");
    static_assert(
        !HasElementComponent<TriMesh&, ElemId::VERTEX, CompId::WEDGE_COLORS>,
        "TriMesh& does satisfy the HasElementComponent "
        "VERTEX-WEDGE_COLORS");
    static_assert(
        !HasElementComponent<
            const TriMesh&,
            ElemId::VERTEX,
            CompId::WEDGE_COLORS>,
        "const TriMesh& does satisfy the HasElementComponent "
        "VERTEX-WEDGE_COLORS");
    static_assert(
        !HasElementComponent<TriMesh&&, ElemId::VERTEX, CompId::WEDGE_COLORS>,
        "TriMesh&& does satisfy the HasElementComponent "
        "VERTEX-WEDGE_COLORS");

    static_assert(
        !HasElementComponent<TriMesh, ElemId::EDGE, CompId::WEDGE_COLORS>,
        "TriMesh does satisfy the HasElementComponent "
        "EDGE-WEDGE_COLORS");
    static_assert(
        !HasElementComponent<const TriMesh, ElemId::EDGE, CompId::WEDGE_COLORS>,
        "const TriMesh does satisfy the HasElementComponent "
        "EDGE-WEDGE_COLORS");
    static_assert(
        !HasElementComponent<TriMesh&, ElemId::EDGE, CompId::WEDGE_COLORS>,
        "TriMesh& does satisfy the HasElementComponent "
        "EDGE-WEDGE_COLORS");
    static_assert(
        !HasElementComponent<
            const TriMesh&,
            ElemId::EDGE,
            CompId::WEDGE_COLORS>,
        "const TriMesh& does satisfy the HasElementComponent "
        "EDGE-WEDGE_COLORS");
    static_assert(
        !HasElementComponent<TriMesh&&, ElemId::EDGE, CompId::WEDGE_COLORS>,
        "TriMesh&& does satisfy the HasElementComponent "
        "EDGE-WEDGE_COLORS");

    static_assert(
        !HasElementOptionalComponent<TriMesh, ElemId::VERTEX, CompId::POSITION>,
        "TriMesh does satisfy the HasElementOptionalComponent "
        "VERTEX-POSITION");
    static_assert(
        !HasElementOptionalComponent<
            const TriMesh,
            ElemId::VERTEX,
            CompId::POSITION>,
        "const TriMesh does satisfy the HasElementOptionalComponent "
        "VERTEX-POSITION");
    static_assert(
        !HasElementOptionalComponent<
            TriMesh&,
            ElemId::VERTEX,
            CompId::POSITION>,
        "TriMesh& does satisfy the HasElementOptionalComponent "
        "VERTEX-POSITION");
    static_assert(
        !HasElementOptionalComponent<
            const TriMesh&,
            ElemId::VERTEX,
            CompId::POSITION>,
        "const TriMesh& does satisfy the HasElementOptionalComponent "
        "VERTEX-POSITION");
    static_assert(
        !HasElementOptionalComponent<
            TriMesh&&,
            ElemId::VERTEX,
            CompId::POSITION>,
        "TriMesh&& does satisfy the HasElementOptionalComponent "
        "VERTEX-POSITION");

    static_assert(
        HasElementOptionalComponent<TriMesh, ElemId::VERTEX, CompId::COLOR>,
        "TriMesh does not satisfy the HasElementOptionalComponent "
        "VERTEX-COLOR");
    static_assert(
        HasElementOptionalComponent<
            const TriMesh,
            ElemId::VERTEX,
            CompId::COLOR>,
        "const TriMesh does not satisfy the HasElementOptionalComponent "
        "VERTEX-COLOR");
    static_assert(
        HasElementOptionalComponent<TriMesh&, ElemId::VERTEX, CompId::COLOR>,
        "TriMesh& does not satisfy the HasElementOptionalComponent "
        "VERTEX-COLOR");
    static_assert(
        HasElementOptionalComponent<
            const TriMesh&,
            ElemId::VERTEX,
            CompId::COLOR>,
        "const TriMesh& does not satisfy the HasElementOptionalComponent "
        "VERTEX-COLOR");
    static_assert(
        HasElementOptionalComponent<TriMesh&&, ElemId::VERTEX, CompId::COLOR>,
        "TriMesh&& does not satisfy the HasElementOptionalComponent "
        "VERTEX-COLOR");

    static_assert(
        MeshConcept<TriMesh>, "TriMesh does not satisfy the MeshConcept");
    static_assert(
        MeshConcept<const TriMesh>,
        "const TriMesh does not satisfy the MeshConcept");
    static_assert(
        MeshConcept<TriMesh&>, "TriMesh& does not satisfy the MeshConcept");
    static_assert(
        MeshConcept<const TriMesh&>,
        "const TriMesh& does not satisfy the MeshConcept");
    static_assert(
        MeshConcept<TriMesh&&>, "TriMesh&& does not satisfy the MeshConcept");
}

#endif // MESH_CONCEPT_H
