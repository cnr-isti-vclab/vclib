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

#ifndef MESH_CONCEPT_H
#define MESH_CONCEPT_H

#include <vclib/meshes.h>

void meshConceptStaticAsserts()
{
    using namespace vcl;

    using TMVertex = TriMesh::Vertex;

    static_assert(
        mesh::HasElementContainer<TriMesh, ElemId::VERTEX>,
        "TriMesh does not satisfy the HasElementContainer VERTEX");
    static_assert(
        mesh::HasElementContainer<const TriMesh, ElemId::VERTEX>,
        "const TriMesh does not satisfy the HasElementContainer VERTEX");
    static_assert(
        mesh::HasElementContainer<TriMesh&, ElemId::VERTEX>,
        "TriMesh& does not satisfy the HasElementContainer VERTEX");
    static_assert(
        mesh::HasElementContainer<const TriMesh&, ElemId::VERTEX>,
        "const TriMesh& does not satisfy the HasElementContainer VERTEX");
    static_assert(
        mesh::HasElementContainer<TriMesh&&, ElemId::VERTEX>,
        "TriMesh&& does not satisfy the HasElementContainer VERTEX");

    static_assert(
        !mesh::HasElementContainer<TriMesh, ElemId::EDGE>,
        "TriMesh does satisfy the HasElementContainer EDGE");
    static_assert(
        !mesh::HasElementContainer<const TriMesh, ElemId::EDGE>,
        "const TriMesh does satisfy the HasElementContainer EDGE");
    static_assert(
        !mesh::HasElementContainer<TriMesh&, ElemId::EDGE>,
        "TriMesh& does satisfy the HasElementContainer EDGE");
    static_assert(
        !mesh::HasElementContainer<const TriMesh&, ElemId::EDGE>,
        "const TriMesh& does satisfy the HasElementContainer EDGE");
    static_assert(
        !mesh::HasElementContainer<TriMesh&&, ElemId::EDGE>,
        "TriMesh&& does satisfy the HasElementContainer EDGE");

    static_assert(
        mesh::HasPerElementComponent<TriMesh, ElemId::VERTEX, CompId::POSITION>,
        "TriMesh does not satisfy the HasPerElementComponent "
        "VERTEX-POSITION");
    static_assert(
        mesh::HasPerElementComponent<
            const TriMesh,
            ElemId::VERTEX,
            CompId::POSITION>,
        "const TriMesh does not satisfy the HasPerElementComponent "
        "VERTEX-POSITION");
    static_assert(
        mesh::
            HasPerElementComponent<TriMesh&, ElemId::VERTEX, CompId::POSITION>,
        "TriMesh& does not satisfy the HasPerElementComponent "
        "VERTEX-POSITION");
    static_assert(
        mesh::HasPerElementComponent<
            const TriMesh&,
            ElemId::VERTEX,
            CompId::POSITION>,
        "const TriMesh& does not satisfy the HasPerElementComponent "
        "VERTEX-POSITION");
    static_assert(
        mesh::
            HasPerElementComponent<TriMesh&&, ElemId::VERTEX, CompId::POSITION>,
        "TriMesh&& does not satisfy the HasPerElementComponent "
        "VERTEX-POSITION");

    static_assert(
        !mesh::HasPerElementComponent<
            TriMesh,
            ElemId::VERTEX,
            CompId::WEDGE_COLORS>,
        "TriMesh does satisfy the HasPerElementComponent "
        "VERTEX-WEDGE_COLORS");
    static_assert(
        !mesh::HasPerElementComponent<
            const TriMesh,
            ElemId::VERTEX,
            CompId::WEDGE_COLORS>,
        "const TriMesh does satisfy the HasPerElementComponent "
        "VERTEX-WEDGE_COLORS");
    static_assert(
        !mesh::HasPerElementComponent<
            TriMesh&,
            ElemId::VERTEX,
            CompId::WEDGE_COLORS>,
        "TriMesh& does satisfy the HasPerElementComponent "
        "VERTEX-WEDGE_COLORS");
    static_assert(
        !mesh::HasPerElementComponent<
            const TriMesh&,
            ElemId::VERTEX,
            CompId::WEDGE_COLORS>,
        "const TriMesh& does satisfy the HasPerElementComponent "
        "VERTEX-WEDGE_COLORS");
    static_assert(
        !mesh::HasPerElementComponent<
            TriMesh&&,
            ElemId::VERTEX,
            CompId::WEDGE_COLORS>,
        "TriMesh&& does satisfy the HasPerElementComponent "
        "VERTEX-WEDGE_COLORS");

    static_assert(
        !mesh::
            HasPerElementComponent<TriMesh, ElemId::EDGE, CompId::WEDGE_COLORS>,
        "TriMesh does satisfy the HasPerElementComponent "
        "EDGE-WEDGE_COLORS");
    static_assert(
        !mesh::HasPerElementComponent<
            const TriMesh,
            ElemId::EDGE,
            CompId::WEDGE_COLORS>,
        "const TriMesh does satisfy the HasPerElementComponent "
        "EDGE-WEDGE_COLORS");
    static_assert(
        !mesh::HasPerElementComponent<
            TriMesh&,
            ElemId::EDGE,
            CompId::WEDGE_COLORS>,
        "TriMesh& does satisfy the HasPerElementComponent "
        "EDGE-WEDGE_COLORS");
    static_assert(
        !mesh::HasPerElementComponent<
            const TriMesh&,
            ElemId::EDGE,
            CompId::WEDGE_COLORS>,
        "const TriMesh& does satisfy the HasPerElementComponent "
        "EDGE-WEDGE_COLORS");
    static_assert(
        !mesh::HasPerElementComponent<
            TriMesh&&,
            ElemId::EDGE,
            CompId::WEDGE_COLORS>,
        "TriMesh&& does satisfy the HasPerElementComponent "
        "EDGE-WEDGE_COLORS");

    static_assert(
        !mesh::HasPerElementOptionalComponent<
            TriMesh,
            ElemId::VERTEX,
            CompId::POSITION>,
        "TriMesh does satisfy the HasPerElementOptionalComponent "
        "VERTEX-POSITION");
    static_assert(
        !mesh::HasPerElementOptionalComponent<
            const TriMesh,
            ElemId::VERTEX,
            CompId::POSITION>,
        "const TriMesh does satisfy the HasPerElementOptionalComponent "
        "VERTEX-POSITION");
    static_assert(
        !mesh::HasPerElementOptionalComponent<
            TriMesh&,
            ElemId::VERTEX,
            CompId::POSITION>,
        "TriMesh& does satisfy the HasPerElementOptionalComponent "
        "VERTEX-POSITION");
    static_assert(
        !mesh::HasPerElementOptionalComponent<
            const TriMesh&,
            ElemId::VERTEX,
            CompId::POSITION>,
        "const TriMesh& does satisfy the HasPerElementOptionalComponent "
        "VERTEX-POSITION");
    static_assert(
        !mesh::HasPerElementOptionalComponent<
            TriMesh&&,
            ElemId::VERTEX,
            CompId::POSITION>,
        "TriMesh&& does satisfy the HasPerElementOptionalComponent "
        "VERTEX-POSITION");

    static_assert(
        mesh::HasPerElementOptionalComponent<
            TriMesh,
            ElemId::VERTEX,
            CompId::COLOR>,
        "TriMesh does not satisfy the HasPerElementOptionalComponent "
        "VERTEX-COLOR");
    static_assert(
        mesh::HasPerElementOptionalComponent<
            const TriMesh,
            ElemId::VERTEX,
            CompId::COLOR>,
        "const TriMesh does not satisfy the HasPerElementOptionalComponent "
        "VERTEX-COLOR");
    static_assert(
        mesh::HasPerElementOptionalComponent<
            TriMesh&,
            ElemId::VERTEX,
            CompId::COLOR>,
        "TriMesh& does not satisfy the HasPerElementOptionalComponent "
        "VERTEX-COLOR");
    static_assert(
        mesh::HasPerElementOptionalComponent<
            const TriMesh&,
            ElemId::VERTEX,
            CompId::COLOR>,
        "const TriMesh& does not satisfy the HasPerElementOptionalComponent "
        "VERTEX-COLOR");
    static_assert(
        mesh::HasPerElementOptionalComponent<
            TriMesh&&,
            ElemId::VERTEX,
            CompId::COLOR>,
        "TriMesh&& does not satisfy the HasPerElementOptionalComponent "
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

    static_assert(
        ElementOrMeshConcept<TriMesh>,
        "TriMesh does not satisfy the ElementOrMeshConcept");
    static_assert(
        ElementOrMeshConcept<const TriMesh>,
        "const TriMesh does not satisfy the ElementOrMeshConcept");
    static_assert(
        ElementOrMeshConcept<TriMesh&>,
        "TriMesh& does not satisfy the ElementOrMeshConcept");
    static_assert(
        ElementOrMeshConcept<const TriMesh&>,
        "const TriMesh& does not satisfy the ElementOrMeshConcept");
    static_assert(
        ElementOrMeshConcept<TriMesh&&>,
        "TriMesh&& does not satisfy the ElementOrMeshConcept");

    static_assert(
        ElementOrMeshConcept<TMVertex>,
        "TriMesh Vertex does not satisfy the ElementOrMeshConcept");
    static_assert(
        ElementOrMeshConcept<const TMVertex>,
        "const TriMesh Vertex does not satisfy the ElementOrMeshConcept");
    static_assert(
        ElementOrMeshConcept<TMVertex&>,
        "TriMesh Vertex& does not satisfy the ElementOrMeshConcept");
    static_assert(
        ElementOrMeshConcept<const TMVertex&>,
        "const TriMesh Vertex& does not satisfy the ElementOrMeshConcept");
    static_assert(
        ElementOrMeshConcept<TMVertex&&>,
        "TriMesh Vertex&& does not satisfy the ElementOrMeshConcept");
}

#endif // MESH_CONCEPT_H
