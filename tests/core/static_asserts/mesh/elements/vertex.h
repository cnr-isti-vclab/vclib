// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef ELEM_VERTEX_H
#define ELEM_VERTEX_H

#include <vclib/meshes.h>

void vertexStaticAsserts()
{
    using namespace vcl;

    using TMVertex = VertexType<TriMesh>;

    static_assert(
        VertexConcept<TMVertex>, "TMVertex does not satisfy the VertexConcept");
    static_assert(
        VertexConcept<const TMVertex>,
        "const TMVertex does not satisfy the VertexConcept");
    static_assert(
        VertexConcept<TMVertex&>,
        "TMVertex& does not satisfy the VertexConcept");
    static_assert(
        VertexConcept<const TMVertex&>,
        "const TMVertex& does not satisfy the VertexConcept");
    static_assert(
        VertexConcept<TMVertex&&>,
        "TMVertex&& does not satisfy the VertexConcept");
}

#endif // ELEM_VERTEX_H
