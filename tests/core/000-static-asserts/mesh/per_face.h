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

#ifndef PER_FACE_H
#define PER_FACE_H

#include <vclib/meshes.h>

void perFaceStaticAsserts()
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
        HasPerFaceAdjacentEdges<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasPerFaceAdjacentEdges");
    static_assert(
        HasPerFaceAdjacentEdges<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasPerFaceAdjacentEdges");
    static_assert(
        HasPerFaceAdjacentEdges<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasPerFaceAdjacentEdges");
    static_assert(
        HasPerFaceAdjacentEdges<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasPerFaceAdjacentEdges");
    static_assert(
        HasPerFaceAdjacentEdges<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasPerFaceAdjacentEdges");

    static_assert(
        HasPerFaceAdjacentFaces<TriMesh>,
        "TriMesh does not satisfy the HasPerFaceAdjacentFaces");
    static_assert(
        HasPerFaceAdjacentFaces<const TriMesh>,
        "const TriMesh does not satisfy the HasPerFaceAdjacentFaces");
    static_assert(
        HasPerFaceAdjacentFaces<TriMesh&>,
        "TriMesh& does not satisfy the HasPerFaceAdjacentFaces");
    static_assert(
        HasPerFaceAdjacentFaces<const TriMesh&>,
        "const TriMesh& does not satisfy the HasPerFaceAdjacentFaces");
    static_assert(
        HasPerFaceAdjacentFaces<TriMesh&&>,
        "TriMesh&& does not satisfy the HasPerFaceAdjacentFaces");

    static_assert(
        HasPerFaceBitFlags<TriMesh>,
        "TriMesh does not satisfy the HasPerFaceBitFlags");
    static_assert(
        HasPerFaceBitFlags<const TriMesh>,
        "const TriMesh does not satisfy the HasPerFaceBitFlags");
    static_assert(
        HasPerFaceBitFlags<TriMesh&>,
        "TriMesh& does not satisfy the HasPerFaceBitFlags");
    static_assert(
        HasPerFaceBitFlags<const TriMesh&>,
        "const TriMesh& does not satisfy the HasPerFaceBitFlags");
    static_assert(
        HasPerFaceBitFlags<TriMesh&&>,
        "TriMesh&& does not satisfy the HasPerFaceBitFlags");

    static_assert(
        HasPerFaceColor<TriMesh>,
        "TriMesh does not satisfy the HasPerFaceColor");
    static_assert(
        HasPerFaceColor<const TriMesh>,
        "const TriMesh does not satisfy the HasPerFaceColor");
    static_assert(
        HasPerFaceColor<TriMesh&>,
        "TriMesh& does not satisfy the HasPerFaceColor");
    static_assert(
        HasPerFaceColor<const TriMesh&>,
        "const TriMesh& does not satisfy the HasPerFaceColor");
    static_assert(
        HasPerFaceColor<TriMesh&&>,
        "TriMesh&& does not satisfy the HasPerFaceColor");

    static_assert(
        HasPerFaceCustomComponents<TriMesh>,
        "TriMesh does not satisfy the HasPerFaceCustomComponents");
    static_assert(
        HasPerFaceCustomComponents<const TriMesh>,
        "const TriMesh does not satisfy the HasPerFaceCustomComponents");
    static_assert(
        HasPerFaceCustomComponents<TriMesh&>,
        "TriMesh& does not satisfy the HasPerFaceCustomComponents");
    static_assert(
        HasPerFaceCustomComponents<const TriMesh&>,
        "const TriMesh& does not satisfy the HasPerFaceCustomComponents");
    static_assert(
        HasPerFaceCustomComponents<TriMesh&&>,
        "TriMesh&& does not satisfy the HasPerFaceCustomComponents");

    static_assert(
        HasPerFaceMark<PolyMesh>,
        "PolyMesh does not satisfy the HasPerFaceMark");
    static_assert(
        HasPerFaceMark<const PolyMesh>,
        "const PolyMesh does not satisfy the HasPerFaceMark");
    static_assert(
        HasPerFaceMark<PolyMesh&>,
        "PolyMesh& does not satisfy the HasPerFaceMark");
    static_assert(
        HasPerFaceMark<const PolyMesh&>,
        "const PolyMesh& does not satisfy the HasPerFaceMark");
    static_assert(
        HasPerFaceMark<PolyMesh&&>,
        "PolyMesh&& does not satisfy the HasPerFaceMark");

    static_assert(
        HasPerFaceNormal<PolyMesh>,
        "PolyMesh does not satisfy the HasPerFaceNormal");
    static_assert(
        HasPerFaceNormal<const PolyMesh>,
        "const PolyMesh does not satisfy the HasPerFaceNormal");
    static_assert(
        HasPerFaceNormal<PolyMesh&>,
        "PolyMesh& does not satisfy the HasPerFaceNormal");
    static_assert(
        HasPerFaceNormal<const PolyMesh&>,
        "const PolyMesh& does not satisfy the HasPerFaceNormal");
    static_assert(
        HasPerFaceNormal<PolyMesh&&>,
        "PolyMesh&& does not satisfy the HasPerFaceNormal");

    static_assert(
        HasPerFacePolygonBitFlags<PolyMesh>,
        "PolyMesh does not satisfy the HasPerFacePolygonBitFlags");
    static_assert(
        HasPerFacePolygonBitFlags<const PolyMesh>,
        "const PolyMesh does not satisfy the HasPerFacePolygonBitFlags");
    static_assert(
        HasPerFacePolygonBitFlags<PolyMesh&>,
        "PolyMesh& does not satisfy the HasPerFacePolygonBitFlags");
    static_assert(
        HasPerFacePolygonBitFlags<const PolyMesh&>,
        "const PolyMesh& does not satisfy the HasPerFacePolygonBitFlags");
    static_assert(
        HasPerFacePolygonBitFlags<PolyMesh&&>,
        "PolyMesh&& does not satisfy the HasPerFacePolygonBitFlags");

    // static_assert(
    //     HasPerFacePrincipalCurvature<TriMesh>,
    //     "TriMesh does not satisfy the HasPerFacePrincipalCurvature");
    // static_assert(
    //     HasPerFacePrincipalCurvature<const TriMesh>,
    //     "const TriMesh does not satisfy the HasPerFacePrincipalCurvature");
    // static_assert(
    //     HasPerFacePrincipalCurvature<TriMesh&>,
    //     "TriMesh& does not satisfy the HasPerFacePrincipalCurvature");
    // static_assert(
    //     HasPerFacePrincipalCurvature<const TriMesh&>,
    //     "const TriMesh& does not satisfy the HasPerFacePrincipalCurvature");
    // static_assert(
    //     HasPerFacePrincipalCurvature<TriMesh&&>,
    //     "TriMesh&& does not satisfy the HasPerFacePrincipalCurvature");

    static_assert(
        HasPerFaceQuality<TriMesh>,
        "TriMesh does not satisfy the HasPerFaceQuality");
    static_assert(
        HasPerFaceQuality<const TriMesh>,
        "const TriMesh does not satisfy the HasPerFaceQuality");
    static_assert(
        HasPerFaceQuality<TriMesh&>,
        "TriMesh& does not satisfy the HasPerFaceQuality");
    static_assert(
        HasPerFaceQuality<const TriMesh&>,
        "const TriMesh& does not satisfy the HasPerFaceQuality");
    static_assert(
        HasPerFaceQuality<TriMesh&&>,
        "TriMesh&& does not satisfy the HasPerFaceQuality");

    static_assert(
        HasPerFaceTriangleBitFlags<TriMesh>,
        "TriMesh does not satisfy the HasPerFaceTriangleBitFlags");
    static_assert(
        HasPerFaceTriangleBitFlags<const TriMesh>,
        "const TriMesh does not satisfy the HasPerFaceTriangleBitFlags");
    static_assert(
        HasPerFaceTriangleBitFlags<TriMesh&>,
        "TriMesh& does not satisfy the HasPerFaceTriangleBitFlags");
    static_assert(
        HasPerFaceTriangleBitFlags<const TriMesh&>,
        "const TriMesh& does not satisfy the HasPerFaceTriangleBitFlags");
    static_assert(
        HasPerFaceTriangleBitFlags<TriMesh&&>,
        "TriMesh&& does not satisfy the HasPerFaceTriangleBitFlags");

    static_assert(
        HasPerFaceVertexReferences<TriMesh>,
        "TriMesh does not satisfy the HasPerFaceVertexReferences");
    static_assert(
        HasPerFaceVertexReferences<const TriMesh>,
        "const TriMesh does not satisfy the HasPerFaceVertexReferences");
    static_assert(
        HasPerFaceVertexReferences<TriMesh&>,
        "TriMesh& does not satisfy the HasPerFaceVertexReferences");
    static_assert(
        HasPerFaceVertexReferences<const TriMesh&>,
        "const TriMesh& does not satisfy the HasPerFaceVertexReferences");
    static_assert(
        HasPerFaceVertexReferences<TriMesh&&>,
        "TriMesh&& does not satisfy the HasPerFaceVertexReferences");

    // static_assert(
    //     HasPerFaceWedgeColors<TriMesh>,
    //     "TriMesh does not satisfy the HasPerFaceWedgeColors");
    // static_assert(
    //     HasPerFaceWedgeColors<const TriMesh>,
    //     "const TriMesh does not satisfy the HasPerFaceWedgeColors");
    // static_assert(
    //     HasPerFaceWedgeColors<TriMesh&>,
    //     "TriMesh& does not satisfy the HasPerFaceWedgeColors");
    // static_assert(
    //     HasPerFaceWedgeColors<const TriMesh&>,
    //     "const TriMesh& does not satisfy the HasPerFaceWedgeColors");
    // static_assert(
    //     HasPerFaceWedgeColors<TriMesh&&>,
    //     "TriMesh&& does not satisfy the HasPerFaceWedgeColors");

    static_assert(
        HasPerFaceWedgeTexCoords<TriMesh>,
        "TriMesh does not satisfy the HasPerFaceWedgeTexCoords");
    static_assert(
        HasPerFaceWedgeTexCoords<const TriMesh>,
        "const TriMesh does not satisfy the HasPerFaceWedgeTexCoords");
    static_assert(
        HasPerFaceWedgeTexCoords<TriMesh&>,
        "TriMesh& does not satisfy the HasPerFaceWedgeTexCoords");
    static_assert(
        HasPerFaceWedgeTexCoords<const TriMesh&>,
        "const TriMesh& does not satisfy the HasPerFaceWedgeTexCoords");
    static_assert(
        HasPerFaceWedgeTexCoords<TriMesh&&>,
        "TriMesh&& does not satisfy the HasPerFaceWedgeTexCoords");
}

#endif // PER_FACE_H
