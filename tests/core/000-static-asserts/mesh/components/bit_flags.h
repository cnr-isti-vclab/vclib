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

#ifndef COMP_BIT_FLAGS_H
#define COMP_BIT_FLAGS_H

#include <vclib/meshes.h>

void bitFlagsComponentStaticAsserts()
{
    using namespace vcl;

    using TriMeshVertex = trimesh::Vertex<float, true>;
    using TriMeshFace   = trimesh::Face<float, true>;
    using PolyMeshFace  = polymesh::Face<float, true>;

    // test only the bit flag component
    static_assert(
        comp::HasBitFlags<vert::BitFlags>,
        "vert::BitFlags does not satisfy the HasBitFlags concept");
    static_assert(
        comp::HasBitFlags<const vert::BitFlags>,
        "const vert::BitFlags does not satisfy the HasBitFlags concept");
    static_assert(
        comp::HasBitFlags<vert::BitFlags&>,
        "vert::BitFlags& does not satisfy the HasBitFlags concept");
    static_assert(
        comp::HasBitFlags<const vert::BitFlags&>,
        "const vert::BitFlags& does not satisfy the HasBitFlags concept");
    static_assert(
        comp::HasBitFlags<vert::BitFlags&&>,
        "vert::BitFlags&& does not satisfy the HasBitFlags concept");

    static_assert(
        comp::HasBitFlags<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasBitFlags concept");
    static_assert(
        comp::HasBitFlags<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the HasBitFlags concept");
    static_assert(
        comp::HasBitFlags<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasBitFlags concept");
    static_assert(
        comp::HasBitFlags<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the HasBitFlags concept");
    static_assert(
        comp::HasBitFlags<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasBitFlags concept");

    static_assert(
        comp::HasTriangleBitFlags<TriMeshFace>,
        "TriMesh Face does not satisfy the HasTriangleBitFlags concept");
    static_assert(
        comp::HasTriangleBitFlags<const TriMeshFace>,
        "const TriMesh Face does not satisfy the HasTriangleBitFlags concept");
    static_assert(
        comp::HasTriangleBitFlags<TriMeshFace&>,
        "TriMesh Face& does not satisfy the HasTriangleBitFlags concept");
    static_assert(
        comp::HasTriangleBitFlags<const TriMeshFace&>,
        "const TriMesh Face& does not satisfy the HasTriangleBitFlags concept");
    static_assert(
        comp::HasTriangleBitFlags<TriMeshFace&&>,
        "TriMesh Face&& does not satisfy the HasTriangleBitFlags concept");

    static_assert(
        comp::HasPolygonBitFlags<PolyMeshFace>,
        "PolyMesh Face does not satisfy the HasPolygonBitFlags concept");
    static_assert(
        comp::HasPolygonBitFlags<const PolyMeshFace>,
        "const PolyMesh Face does not satisfy the HasPolygonBitFlags concept");
    static_assert(
        comp::HasPolygonBitFlags<PolyMeshFace&>,
        "PolyMesh Face& does not satisfy the HasPolygonBitFlags concept");
    static_assert(
        comp::HasPolygonBitFlags<const PolyMeshFace&>,
        "const PolyMesh Face& does not satisfy the HasPolygonBitFlags concept");
    static_assert(
        comp::HasPolygonBitFlags<PolyMeshFace&&>,
        "PolyMesh Face&& does not satisfy the HasPolygonBitFlags concept");

    static_assert(
        comp::HasFaceBitFlags<TriMeshFace>,
        "TriMesh Face does not satisfy the HasFaceBitFlags concept");
    static_assert(
        comp::HasFaceBitFlags<const TriMeshFace>,
        "const TriMesh Face does not satisfy the HasFaceBitFlags concept");
    static_assert(
        comp::HasFaceBitFlags<TriMeshFace&>,
        "TriMesh Face& does not satisfy the HasFaceBitFlags concept");
    static_assert(
        comp::HasFaceBitFlags<const TriMeshFace&>,
        "const TriMesh Face& does not satisfy the HasFaceBitFlags concept");
    static_assert(
        comp::HasFaceBitFlags<TriMeshFace&&>,
        "TriMesh Face&& does not satisfy the HasFaceBitFlags concept");

    static_assert(
        comp::HasFaceBitFlags<PolyMeshFace>,
        "PolyMesh Face does not satisfy the HasFaceBitFlags concept");
    static_assert(
        comp::HasFaceBitFlags<const PolyMeshFace>,
        "const PolyMesh Face does not satisfy the HasFaceBitFlags concept");
    static_assert(
        comp::HasFaceBitFlags<PolyMeshFace&>,
        "PolyMesh Face& does not satisfy the HasFaceBitFlags concept");
    static_assert(
        comp::HasFaceBitFlags<const PolyMeshFace&>,
        "const PolyMesh Face& does not satisfy the HasFaceBitFlags concept");
    static_assert(
        comp::HasFaceBitFlags<PolyMeshFace&&>,
        "PolyMesh Face&& does not satisfy the HasFaceBitFlags concept");
}

#endif // COMP_BIT_FLAGS_H
