// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef FACE_COMPONENTS_H
#define FACE_COMPONENTS_H

#include <vclib/meshes.h>

inline void faceComponentsStaticAsserts()
{
    using namespace vcl;

    static_assert(
        HasTriangles<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasTriangles");
    static_assert(
        HasTriangles<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasTriangles");
    static_assert(
        HasTriangles<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasTriangles");
    static_assert(
        HasTriangles<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasTriangles");
    static_assert(
        HasTriangles<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasTriangles");

    static_assert(
        HasPolygons<PolyMesh>, "PolyMesh does not satisfy the HasPolygons");
    static_assert(
        HasPolygons<const PolyMesh>,
        "const PolyMesh does not satisfy the HasPolygons");
    static_assert(
        HasPolygons<PolyMesh&>, "PolyMesh& does not satisfy the HasPolygons");
    static_assert(
        HasPolygons<const PolyMesh&>,
        "const PolyMesh& does not satisfy the HasPolygons");
    static_assert(
        HasPolygons<PolyMesh&&>, "PolyMesh&& does not satisfy the HasPolygons");

    static_assert(
        HasFaceAdjacentEdges<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasFaceAdjacentEdges");
    static_assert(
        HasFaceAdjacentEdges<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasFaceAdjacentEdges");
    static_assert(
        HasFaceAdjacentEdges<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasFaceAdjacentEdges");
    static_assert(
        HasFaceAdjacentEdges<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasFaceAdjacentEdges");
    static_assert(
        HasFaceAdjacentEdges<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasFaceAdjacentEdges");

    static_assert(
        HasFaceAdjacentFaces<TriMesh>,
        "TriMesh does not satisfy the HasFaceAdjacentFaces");
    static_assert(
        HasFaceAdjacentFaces<const TriMesh>,
        "const TriMesh does not satisfy the HasFaceAdjacentFaces");
    static_assert(
        HasFaceAdjacentFaces<TriMesh&>,
        "TriMesh& does not satisfy the HasFaceAdjacentFaces");
    static_assert(
        HasFaceAdjacentFaces<const TriMesh&>,
        "const TriMesh& does not satisfy the HasFaceAdjacentFaces");
    static_assert(
        HasFaceAdjacentFaces<TriMesh&&>,
        "TriMesh&& does not satisfy the HasFaceAdjacentFaces");

    static_assert(
        HasFaceOptionalAdjacentEdges<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasFaceOptionalAdjacentEdges");

    static_assert(
        HasFaceOptionalAdjacentFaces<TriMesh>,
        "TriMesh does not satisfy the HasFaceOptionalAdjacentFaces");

    static_assert(
        HasFaceColor<TriMesh>, "TriMesh does not satisfy the HasFaceColor");
    static_assert(
        HasFaceColor<const TriMesh>,
        "const TriMesh does not satisfy the HasFaceColor");
    static_assert(
        HasFaceColor<TriMesh&>, "TriMesh& does not satisfy the HasFaceColor");
    static_assert(
        HasFaceColor<const TriMesh&>,
        "const TriMesh& does not satisfy the HasFaceColor");
    static_assert(
        HasFaceColor<TriMesh&&>, "TriMesh&& does not satisfy the HasFaceColor");

    static_assert(
        HasFaceCustomComponents<TriMesh>,
        "TriMesh does not satisfy the HasFaceCustomComponents");
    static_assert(
        HasFaceCustomComponents<const TriMesh>,
        "const TriMesh does not satisfy the HasFaceCustomComponents");
    static_assert(
        HasFaceCustomComponents<TriMesh&>,
        "TriMesh& does not satisfy the HasFaceCustomComponents");
    static_assert(
        HasFaceCustomComponents<const TriMesh&>,
        "const TriMesh& does not satisfy the HasFaceCustomComponents");
    static_assert(
        HasFaceCustomComponents<TriMesh&&>,
        "TriMesh&& does not satisfy the HasFaceCustomComponents");

    static_assert(
        HasFaceNormal<PolyMesh>, "PolyMesh does not satisfy the HasFaceNormal");
    static_assert(
        HasFaceNormal<const PolyMesh>,
        "const PolyMesh does not satisfy the HasFaceNormal");
    static_assert(
        HasFaceNormal<PolyMesh&>,
        "PolyMesh& does not satisfy the HasFaceNormal");
    static_assert(
        HasFaceNormal<const PolyMesh&>,
        "const PolyMesh& does not satisfy the HasFaceNormal");
    static_assert(
        HasFaceNormal<PolyMesh&&>,
        "PolyMesh&& does not satisfy the HasFaceNormal");

    static_assert(
        HasFaceQuality<TriMesh>, "TriMesh does not satisfy the HasFaceQuality");
    static_assert(
        HasFaceQuality<const TriMesh>,
        "const TriMesh does not satisfy the HasFaceQuality");
    static_assert(
        HasFaceQuality<TriMesh&>,
        "TriMesh& does not satisfy the HasFaceQuality");
    static_assert(
        HasFaceQuality<const TriMesh&>,
        "const TriMesh& does not satisfy the HasFaceQuality");
    static_assert(
        HasFaceQuality<TriMesh&&>,
        "TriMesh&& does not satisfy the HasFaceQuality");

    static_assert(
        HasFaceWedgeTexCoords<TriMesh>,
        "TriMesh does not satisfy the HasFaceWedgeTexCoords");
    static_assert(
        HasFaceWedgeTexCoords<const TriMesh>,
        "const TriMesh does not satisfy the HasFaceWedgeTexCoords");
    static_assert(
        HasFaceWedgeTexCoords<TriMesh&>,
        "TriMesh& does not satisfy the HasFaceWedgeTexCoords");
    static_assert(
        HasFaceWedgeTexCoords<const TriMesh&>,
        "const TriMesh& does not satisfy the HasFaceWedgeTexCoords");
    static_assert(
        HasFaceWedgeTexCoords<TriMesh&&>,
        "TriMesh&& does not satisfy the HasFaceWedgeTexCoords");
}

#endif // FACE_COMPONENTS_H
