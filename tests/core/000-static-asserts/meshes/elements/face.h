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

#ifndef ELEM_FACE_H
#define ELEM_FACE_H

#include <vclib/meshes.h>

void faceStaticAsserts()
{
    using namespace vcl;

    using TMFace  = trimesh::Face<float, false>;
    using TMFaceI = trimesh::Face<float, true>;
    using PMFace  = polymesh::Face<float, false>;
    using PMFaceI = polymesh::Face<float, true>;

    static_assert(
        FaceConcept<TMFace>, "TMFace does not satisfy the FaceConcept");
    static_assert(
        FaceConcept<const TMFace>,
        "const TMFace does not satisfy the FaceConcept");
    static_assert(
        FaceConcept<TMFace&>, "TMFace& does not satisfy the FaceConcept");
    static_assert(
        FaceConcept<const TMFace&>,
        "const TMFace& does not satisfy the FaceConcept");
    static_assert(
        FaceConcept<TMFace&&>, "TMFace&& does not satisfy the FaceConcept");

    static_assert(
        FaceConcept<TMFaceI>, "TMFaceI does not satisfy the FaceConcept");
    static_assert(
        FaceConcept<const TMFaceI>,
        "const TMFaceI does not satisfy the FaceConcept");
    static_assert(
        FaceConcept<TMFaceI&>, "TMFaceI& does not satisfy the FaceConcept");
    static_assert(
        FaceConcept<const TMFaceI&>,
        "const TMFaceI& does not satisfy the FaceConcept");
    static_assert(
        FaceConcept<TMFaceI&&>, "TMFaceI&& does not satisfy the FaceConcept");

    static_assert(
        FaceConcept<PMFace>, "PMFace does not satisfy the FaceConcept");
    static_assert(
        FaceConcept<const PMFace>,
        "const PMFace does not satisfy the FaceConcept");
    static_assert(
        FaceConcept<PMFace&>, "PMFace& does not satisfy the FaceConcept");
    static_assert(
        FaceConcept<const PMFace&>,
        "const PMFace& does not satisfy the FaceConcept");
    static_assert(
        FaceConcept<PMFace&&>, "PMFace&& does not satisfy the FaceConcept");

    static_assert(
        FaceConcept<PMFaceI>, "PMFaceI does not satisfy the FaceConcept");
    static_assert(
        FaceConcept<const PMFaceI>,
        "const PMFaceI does not satisfy the FaceConcept");
    static_assert(
        FaceConcept<PMFaceI&>, "PMFaceI& does not satisfy the FaceConcept");
    static_assert(
        FaceConcept<const PMFaceI&>,
        "const PMFaceI& does not satisfy the FaceConcept");
    static_assert(
        FaceConcept<PMFaceI&&>, "PMFaceI&& does not satisfy the FaceConcept");

    static_assert(
        TriangleFaceConcept<TMFace>,
        "TMFace does not satisfy the TriangleFaceConcept");
    static_assert(
        TriangleFaceConcept<const TMFace>,
        "const TMFace does not satisfy the TriangleFaceConcept");
    static_assert(
        TriangleFaceConcept<TMFace&>,
        "TMFace& does not satisfy the TriangleFaceConcept");
    static_assert(
        TriangleFaceConcept<const TMFace&>,
        "const TMFace& does not satisfy the TriangleFaceConcept");
    static_assert(
        TriangleFaceConcept<TMFace&&>,
        "TMFace&& does not satisfy the TriangleFaceConcept");

    static_assert(
        TriangleFaceConcept<TMFaceI>,
        "TMFaceI does not satisfy the TriangleFaceConcept");
    static_assert(
        TriangleFaceConcept<const TMFaceI>,
        "const TMFaceI does not satisfy the TriangleFaceConcept");
    static_assert(
        TriangleFaceConcept<TMFaceI&>,
        "TMFaceI& does not satisfy the TriangleFaceConcept");
    static_assert(
        TriangleFaceConcept<const TMFaceI&>,
        "const TMFaceI& does not satisfy the TriangleFaceConcept");
    static_assert(
        TriangleFaceConcept<TMFaceI&&>,
        "TMFaceI&& does not satisfy the TriangleFaceConcept");

    static_assert(
        PolygonFaceConcept<PMFace>,
        "PMFace does not satisfy the PolygonFaceConcept");
    static_assert(
        PolygonFaceConcept<const PMFace>,
        "const PMFace does not satisfy the PolygonFaceConcept");
    static_assert(
        PolygonFaceConcept<PMFace&>,
        "PMFace& does not satisfy the PolygonFaceConcept");
    static_assert(
        PolygonFaceConcept<const PMFace&>,
        "const PMFace& does not satisfy the PolygonFaceConcept");
    static_assert(
        PolygonFaceConcept<PMFace&&>,
        "PMFace&& does not satisfy the PolygonFaceConcept");

    static_assert(
        PolygonFaceConcept<PMFaceI>,
        "PMFaceI does not satisfy the PolygonFaceConcept");
    static_assert(
        PolygonFaceConcept<const PMFaceI>,
        "const PMFaceI does not satisfy the PolygonFaceConcept");
    static_assert(
        PolygonFaceConcept<PMFaceI&>,
        "PMFaceI& does not satisfy the PolygonFaceConcept");
    static_assert(
        PolygonFaceConcept<const PMFaceI&>,
        "const PMFaceI& does not satisfy the PolygonFaceConcept");
    static_assert(
        PolygonFaceConcept<PMFaceI&&>,
        "PMFaceI&& does not satisfy the PolygonFaceConcept");
}

#endif // ELEM_FACE_H
