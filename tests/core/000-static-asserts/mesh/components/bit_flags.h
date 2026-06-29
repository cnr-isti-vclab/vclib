// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
