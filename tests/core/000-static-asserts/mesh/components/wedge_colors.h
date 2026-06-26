// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMP_WEDGE_COLORS_H
#define COMP_WEDGE_COLORS_H

#include <vclib/meshes.h>

void wedgeColorsComponentStaticAsserts()
{
    using namespace vcl;

    // construct face types that actually have wedge colors
    class TriFace :
            public vcl::Face<
                TriMeshT<double, false>,
                face::TriangleBitFlags,
                face::TriangleVertexRefs<
                    false,
                    trimesh::Vertex<double, false>,
                    TriFace>,
                face::OptionalTriangleWedgeColors<TriFace>>
    {
    };

    class PolyFace :
            public vcl::Face<
                PolyMeshT<double, false>,
                face::PolygonBitFlags,
                face::PolygonVertexRefs<
                    false,
                    trimesh::Vertex<double, false>,
                    PolyFace>,
                face::OptionalTriangleWedgeColors<PolyFace>>
    {
    };

    // test only the TriangleWedgeColors and PolygonWedgeColors components
    static_assert(
        comp::HasWedgeColors<face::TriangleWedgeColors>,
        "face::TriangleWedgeColors does not satisfy the HasWedgeColors "
        "concept");
    static_assert(
        comp::HasWedgeColors<const face::TriangleWedgeColors>,
        "const face::TriangleWedgeColors does not satisfy the "
        "HasWedgeColors concept");
    static_assert(
        comp::HasWedgeColors<face::TriangleWedgeColors&>,
        "face::TriangleWedgeColors& does not satisfy the HasWedgeColors "
        "concept");
    static_assert(
        comp::HasWedgeColors<const face::TriangleWedgeColors&>,
        "const face::TriangleWedgeColors& does not satisfy the "
        "HasWedgeColors concept");
    static_assert(
        comp::HasWedgeColors<face::TriangleWedgeColors&&>,
        "face::TriangleWedgeColors&& does not satisfy the "
        "HasWedgeColors concept");

    static_assert(
        comp::HasWedgeColors<face::PolygonWedgeColors>,
        "face::PolygonWedgeColors does not satisfy the HasWedgeColors "
        "concept");
    static_assert(
        comp::HasWedgeColors<const face::PolygonWedgeColors>,
        "const face::PolygonWedgeColors does not satisfy the "
        "HasWedgeColors concept");
    static_assert(
        comp::HasWedgeColors<face::PolygonWedgeColors&>,
        "face::PolygonWedgeColors& does not satisfy the HasWedgeColors "
        "concept");
    static_assert(
        comp::HasWedgeColors<const face::PolygonWedgeColors&>,
        "const face::PolygonWedgeColors& does not satisfy the "
        "HasWedgeColors concept");
    static_assert(
        comp::HasWedgeColors<face::PolygonWedgeColors&&>,
        "face::PolygonWedgeColors&& does not satisfy the "
        "HasWedgeColors concept");

    static_assert(
        comp::HasWedgeColors<TriFace>,
        "TriMesh Face does not satisfy the HasWedgeColors concept");
    static_assert(
        comp::HasWedgeColors<const TriFace>,
        "const TriMesh Face does not satisfy the HasWedgeColors concept");
    static_assert(
        comp::HasWedgeColors<TriFace&>,
        "TriMesh Face& does not satisfy the HasWedgeColors concept");
    static_assert(
        comp::HasWedgeColors<const TriFace&>,
        "const TriMesh Face& does not satisfy the HasWedgeColors concept");
    static_assert(
        comp::HasWedgeColors<TriFace&&>,
        "TriMesh Face&& does not satisfy the HasWedgeColors concept");

    static_assert(
        comp::HasOptionalWedgeColors<TriFace>,
        "TriMesh Face does not satisfy the HasOptionalWedgeColors concept");
    static_assert(
        comp::HasOptionalWedgeColors<const TriFace>,
        "const TriMesh Face does not satisfy the HasOptionalWedgeColors "
        "concept");
    static_assert(
        comp::HasOptionalWedgeColors<TriFace&>,
        "TriMesh Face& does not satisfy the HasOptionalWedgeColors concept");
    static_assert(
        comp::HasOptionalWedgeColors<const TriFace&>,
        "const TriMesh Face& does not satisfy the HasOptionalWedgeColors "
        "concept");
    static_assert(
        comp::HasOptionalWedgeColors<TriFace&&>,
        "TriMesh Face&& does not satisfy the HasOptionalWedgeColors "
        "concept");

    static_assert(
        comp::HasWedgeColors<PolyFace>,
        "PolyMesh Face does not satisfy the HasWedgeColors concept");
    static_assert(
        comp::HasWedgeColors<const PolyFace>,
        "const PolyMesh Face does not satisfy the HasWedgeColors concept");
    static_assert(
        comp::HasWedgeColors<PolyFace&>,
        "PolyMesh Face& does not satisfy the HasWedgeColors concept");
    static_assert(
        comp::HasWedgeColors<const PolyFace&>,
        "const PolyMesh Face& does not satisfy the HasWedgeColors concept");
    static_assert(
        comp::HasWedgeColors<PolyFace&&>,
        "PolyMesh Face&& does not satisfy the HasWedgeColors concept");

    static_assert(
        comp::HasOptionalWedgeColors<PolyFace>,
        "PolyMesh Face does not satisfy the HasOptionalWedgeColors concept");
    static_assert(
        comp::HasOptionalWedgeColors<const PolyFace>,
        "const PolyMesh Face does not satisfy the HasOptionalWedgeColors "
        "concept");
    static_assert(
        comp::HasOptionalWedgeColors<PolyFace&>,
        "PolyMesh Face& does not satisfy the HasOptionalWedgeColors "
        "concept");
    static_assert(
        comp::HasOptionalWedgeColors<const PolyFace&>,
        "const PolyMesh Face& does not satisfy the HasOptionalWedgeColors "
        "concept");
    static_assert(
        comp::HasOptionalWedgeColors<PolyFace&&>,
        "PolyMesh Face&& does not satisfy the HasOptionalWedgeColors "
        "concept");
}

#endif // COMP_WEDGE_COLORS_H
