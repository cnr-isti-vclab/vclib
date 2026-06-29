// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef ELEM_ELEMENT_H
#define ELEM_ELEMENT_H

#include <vclib/meshes.h>

void elementStaticAsserts()
{
    using namespace vcl;

    using TMVertex  = trimesh::Vertex<float, false>;
    using TMVertexI = trimesh::Vertex<float, true>;

    static_assert(
        ElementConcept<TMVertex>,
        "TMVertex does not satisfy the ElementConcept");
    static_assert(
        ElementConcept<const TMVertex>,
        "const TMVertex does not satisfy the ElementConcept");
    static_assert(
        ElementConcept<TMVertex&>,
        "TMVertex& does not satisfy the ElementConcept");
    static_assert(
        ElementConcept<const TMVertex&>,
        "const TMVertex& does not satisfy the ElementConcept");
    static_assert(
        ElementConcept<TMVertex&&>,
        "TMVertex&& does not satisfy the ElementConcept");

    static_assert(
        ElementConcept<TMVertexI>,
        "TMVertexI does not satisfy the ElementConcept");
    static_assert(
        ElementConcept<const TMVertexI>,
        "const TMVertexI does not satisfy the ElementConcept");
    static_assert(
        ElementConcept<TMVertexI&>,
        "TMVertexI& does not satisfy the ElementConcept");
    static_assert(
        ElementConcept<const TMVertexI&>,
        "const TMVertexI& does not satisfy the ElementConcept");
    static_assert(
        ElementConcept<TMVertexI&&>,
        "TMVertexI&& does not satisfy the ElementConcept");
}

#endif // ELEM_ELEMENT_H
