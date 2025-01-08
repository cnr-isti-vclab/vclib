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
