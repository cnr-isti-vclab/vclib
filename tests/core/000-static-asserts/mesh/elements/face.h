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

    using TMFace  = FaceType<TriMesh>;
    using PMFace  = FaceType<PolyMesh>;

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
}

#endif // ELEM_FACE_H
