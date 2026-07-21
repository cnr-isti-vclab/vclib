// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
