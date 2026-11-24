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

    // test only the TriangleWedgeColors and PolygonWedgeColors components
    static_assert(
        comp::HasWedgeColors<face::TriangleWedgeColors>,
        "face::TriangleWedgeColors does not satisfy the HasWedgeColors "
        "concept");
    static_assert(
        comp::HasWedgeColors<const face::TriangleWedgeColors>,
        "const face::TriangleWedgeColors does not satisfy the HasWedgeColors "
        "concept");
    static_assert(
        comp::HasWedgeColors<face::TriangleWedgeColors&>,
        "face::TriangleWedgeColors& does not satisfy the HasWedgeColors "
        "concept");
    static_assert(
        comp::HasWedgeColors<const face::TriangleWedgeColors&>,
        "const face::TriangleWedgeColors& does not satisfy the HasWedgeColors "
        "concept");
    static_assert(
        comp::HasWedgeColors<face::TriangleWedgeColors&&>,
        "face::TriangleWedgeColors&& does not satisfy the HasWedgeColors "
        "concept");

    static_assert(
        comp::HasWedgeColors<face::PolygonWedgeColors>,
        "face::PolygonWedgeColors does not satisfy the HasWedgeColors "
        "concept");
    static_assert(
        comp::HasWedgeColors<const face::PolygonWedgeColors>,
        "const face::PolygonWedgeColors does not satisfy the HasWedgeColors "
        "concept");
    static_assert(
        comp::HasWedgeColors<face::PolygonWedgeColors&>,
        "face::PolygonWedgeColors& does not satisfy the HasWedgeColors "
        "concept");
    static_assert(
        comp::HasWedgeColors<const face::PolygonWedgeColors&>,
        "const face::PolygonWedgeColors& does not satisfy the HasWedgeColors "
        "concept");
    static_assert(
        comp::HasWedgeColors<face::PolygonWedgeColors&&>,
        "face::PolygonWedgeColors&& does not satisfy the HasWedgeColors "
        "concept");
}

#endif // COMP_WEDGE_COLORS_H
