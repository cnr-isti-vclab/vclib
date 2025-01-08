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

#ifndef ELEM_VERTEX_H
#define ELEM_VERTEX_H

#include <vclib/meshes.h>

void vertexStaticAsserts()
{
    using namespace vcl;

    using TMVertex  = trimesh::Vertex<float, false>;
    using TMVertexI = trimesh::Vertex<float, true>;

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

    static_assert(
        VertexConcept<TMVertexI>,
        "TMVertexI does not satisfy the VertexConcept");
    static_assert(
        VertexConcept<const TMVertexI>,
        "const TMVertexI does not satisfy the VertexConcept");
    static_assert(
        VertexConcept<TMVertexI&>,
        "TMVertexI& does not satisfy the VertexConcept");
    static_assert(
        VertexConcept<const TMVertexI&>,
        "const TMVertexI& does not satisfy the VertexConcept");
    static_assert(
        VertexConcept<TMVertexI&&>,
        "TMVertexI&& does not satisfy the VertexConcept");
}

#endif // ELEM_VERTEX_H
