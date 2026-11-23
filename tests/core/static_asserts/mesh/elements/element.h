// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef ELEM_ELEMENT_H
#define ELEM_ELEMENT_H

#include <vclib/meshes.h>

#include <concepts>

void elementStaticAsserts()
{
    using namespace vcl;

    using TMVertex      = VertexType<TriMesh>;
    using TMConstVertex = ConstVertexType<TriMesh>;
    using TMFace        = FaceType<TriMesh>;
    using TMConstFace   = ConstFaceType<TriMesh>;
    using EMEdge        = EdgeType<EdgeMesh>;
    using EMConstEdge   = ConstEdgeType<EdgeMesh>;

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

    // Proxy handle swappability and const/rvalue safety
    static_assert(std::swappable<TMVertex>);
    static_assert(std::swappable<TMConstVertex>);
    static_assert(!std::swappable<const TMVertex>);
    static_assert(!std::swappable<const TMConstVertex>);

    static_assert(std::swappable<TMFace>);
    static_assert(std::swappable<TMConstFace>);
    static_assert(!std::swappable<const TMFace>);
    static_assert(!std::swappable<const TMConstFace>);

    static_assert(std::swappable<EMEdge>);
    static_assert(std::swappable<EMConstEdge>);
    static_assert(!std::swappable<const EMEdge>);
    static_assert(!std::swappable<const EMConstEdge>);
}

#endif // ELEM_ELEMENT_H
