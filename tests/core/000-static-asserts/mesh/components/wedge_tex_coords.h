// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMP_WEDGE_TEX_COORDS_H
#define COMP_WEDGE_TEX_COORDS_H

#include <vclib/meshes.h>

void wedgeTexCoordsComponentStaticAsserts()
{
    using namespace vcl;

    using TriMeshFace  = trimesh::Face<float, true>;
    using PolyMeshFace = polymesh::Face<float, true>;

    // test only the TriangleWedgeTexCoords and PolygonWedgeTexCoords components
    static_assert(
        comp::HasWedgeTexCoords<face::TriangleWedgeTexCoordsd>,
        "face::TriangleWedgeTexCoordsd does not satisfy the HasWedgeTexCoords "
        "concept");
    static_assert(
        comp::HasWedgeTexCoords<const face::TriangleWedgeTexCoordsd>,
        "const face::TriangleWedgeTexCoordsd does not satisfy the "
        "HasWedgeTexCoords concept");
    static_assert(
        comp::HasWedgeTexCoords<face::TriangleWedgeTexCoordsd&>,
        "face::TriangleWedgeTexCoordsd& does not satisfy the HasWedgeTexCoords "
        "concept");
    static_assert(
        comp::HasWedgeTexCoords<const face::TriangleWedgeTexCoordsd&>,
        "const face::TriangleWedgeTexCoordsd& does not satisfy the "
        "HasWedgeTexCoords concept");
    static_assert(
        comp::HasWedgeTexCoords<face::TriangleWedgeTexCoordsd&&>,
        "face::TriangleWedgeTexCoordsd&& does not satisfy the "
        "HasWedgeTexCoords concept");

    static_assert(
        comp::HasWedgeTexCoords<face::PolygonWedgeTexCoordsd>,
        "face::PolygonWedgeTexCoordsd does not satisfy the HasWedgeTexCoords "
        "concept");
    static_assert(
        comp::HasWedgeTexCoords<const face::PolygonWedgeTexCoordsd>,
        "const face::PolygonWedgeTexCoordsd does not satisfy the "
        "HasWedgeTexCoords concept");
    static_assert(
        comp::HasWedgeTexCoords<face::PolygonWedgeTexCoordsd&>,
        "face::PolygonWedgeTexCoordsd& does not satisfy the HasWedgeTexCoords "
        "concept");
    static_assert(
        comp::HasWedgeTexCoords<const face::PolygonWedgeTexCoordsd&>,
        "const face::PolygonWedgeTexCoordsd& does not satisfy the "
        "HasWedgeTexCoords concept");
    static_assert(
        comp::HasWedgeTexCoords<face::PolygonWedgeTexCoordsd&&>,
        "face::PolygonWedgeTexCoordsd&& does not satisfy the "
        "HasWedgeTexCoords concept");

    static_assert(
        comp::HasWedgeTexCoords<TriMeshFace>,
        "TriMesh Face does not satisfy the HasWedgeTexCoords concept");
    static_assert(
        comp::HasWedgeTexCoords<const TriMeshFace>,
        "const TriMesh Face does not satisfy the HasWedgeTexCoords concept");
    static_assert(
        comp::HasWedgeTexCoords<TriMeshFace&>,
        "TriMesh Face& does not satisfy the HasWedgeTexCoords concept");
    static_assert(
        comp::HasWedgeTexCoords<const TriMeshFace&>,
        "const TriMesh Face& does not satisfy the HasWedgeTexCoords concept");
    static_assert(
        comp::HasWedgeTexCoords<TriMeshFace&&>,
        "TriMesh Face&& does not satisfy the HasWedgeTexCoords concept");

    static_assert(
        comp::HasOptionalWedgeTexCoords<TriMeshFace>,
        "TriMesh Face does not satisfy the HasOptionalWedgeTexCoords concept");
    static_assert(
        comp::HasOptionalWedgeTexCoords<const TriMeshFace>,
        "const TriMesh Face does not satisfy the HasOptionalWedgeTexCoords "
        "concept");
    static_assert(
        comp::HasOptionalWedgeTexCoords<TriMeshFace&>,
        "TriMesh Face& does not satisfy the HasOptionalWedgeTexCoords concept");
    static_assert(
        comp::HasOptionalWedgeTexCoords<const TriMeshFace&>,
        "const TriMesh Face& does not satisfy the HasOptionalWedgeTexCoords "
        "concept");
    static_assert(
        comp::HasOptionalWedgeTexCoords<TriMeshFace&&>,
        "TriMesh Face&& does not satisfy the HasOptionalWedgeTexCoords "
        "concept");

    static_assert(
        comp::HasWedgeTexCoords<PolyMeshFace>,
        "PolyMesh Face does not satisfy the HasWedgeTexCoords concept");
    static_assert(
        comp::HasWedgeTexCoords<const PolyMeshFace>,
        "const PolyMesh Face does not satisfy the HasWedgeTexCoords concept");
    static_assert(
        comp::HasWedgeTexCoords<PolyMeshFace&>,
        "PolyMesh Face& does not satisfy the HasWedgeTexCoords concept");
    static_assert(
        comp::HasWedgeTexCoords<const PolyMeshFace&>,
        "const PolyMesh Face& does not satisfy the HasWedgeTexCoords concept");
    static_assert(
        comp::HasWedgeTexCoords<PolyMeshFace&&>,
        "PolyMesh Face&& does not satisfy the HasWedgeTexCoords concept");

    static_assert(
        comp::HasOptionalWedgeTexCoords<PolyMeshFace>,
        "PolyMesh Face does not satisfy the HasOptionalWedgeTexCoords concept");
    static_assert(
        comp::HasOptionalWedgeTexCoords<const PolyMeshFace>,
        "const PolyMesh Face does not satisfy the HasOptionalWedgeTexCoords "
        "concept");
    static_assert(
        comp::HasOptionalWedgeTexCoords<PolyMeshFace&>,
        "PolyMesh Face& does not satisfy the HasOptionalWedgeTexCoords "
        "concept");
    static_assert(
        comp::HasOptionalWedgeTexCoords<const PolyMeshFace&>,
        "const PolyMesh Face& does not satisfy the HasOptionalWedgeTexCoords "
        "concept");
    static_assert(
        comp::HasOptionalWedgeTexCoords<PolyMeshFace&&>,
        "PolyMesh Face&& does not satisfy the HasOptionalWedgeTexCoords "
        "concept");
}

#endif // COMP_WEDGE_TEX_COORDS_H
