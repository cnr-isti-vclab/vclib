// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vclib/space.h>

void triangleStaticAsserts()
{
    using namespace vcl;

    using Triangle2Type = Triangle2f;
    using Triangle3Type = Triangle3d;

    using TriangleWrapper2Type = TriangleWrapper2f;
    using TriangleWrapper3Type = TriangleWrapper3d;

    // triangle2
    static_assert(
        Triangle2Concept<Triangle2Type>,
        "Triangle2 does not satisfy the Triangle2Concept");
    static_assert(
        Triangle2Concept<const Triangle2Type>,
        "const Triangle2 does not satisfy the Triangle2Concept");
    static_assert(
        Triangle2Concept<Triangle2Type&>,
        "Triangle2& does not satisfy the Triangle2Concept");
    static_assert(
        Triangle2Concept<const Triangle2Type&>,
        "const Triangle2& does not satisfy the Triangle2Concept");
    static_assert(
        Triangle2Concept<Triangle2Type&&>,
        "Triangle2&& does not satisfy the Triangle2Concept");

    // triangle3
    static_assert(
        Triangle3Concept<Triangle3Type>,
        "Triangle3 does not satisfy the Triangle3Concept");
    static_assert(
        Triangle3Concept<const Triangle3Type>,
        "const Triangle3 does not satisfy the Triangle3Concept");
    static_assert(
        Triangle3Concept<Triangle3Type&>,
        "Triangle3& does not satisfy the Triangle3Concept");
    static_assert(
        Triangle3Concept<const Triangle3Type&>,
        "const Triangle3& does not satisfy the Triangle3Concept");
    static_assert(
        Triangle3Concept<Triangle3Type&&>,
        "Triangle3&& does not satisfy the Triangle3Concept");

    // triangle wrapper2
    static_assert(
        Triangle2Concept<TriangleWrapper2Type>,
        "TriangleWrapper2 does not satisfy the Triangle2Concept");
    static_assert(
        Triangle2Concept<const TriangleWrapper2Type>,
        "const TriangleWrapper2 does not satisfy the Triangle2Concept");
    static_assert(
        Triangle2Concept<TriangleWrapper2Type&>,
        "TriangleWrapper2& does not satisfy the Triangle2Concept");
    static_assert(
        Triangle2Concept<const TriangleWrapper2Type&>,
        "const TriangleWrapper2& does not satisfy the Triangle2Concept");
    static_assert(
        Triangle2Concept<TriangleWrapper2Type&&>,
        "TriangleWrapper2&& does not satisfy the Triangle2Concept");

    // triangle wrapper3
    static_assert(
        Triangle3Concept<TriangleWrapper3Type>,
        "TriangleWrapper3 does not satisfy the Triangle3Concept");
    static_assert(
        Triangle3Concept<const TriangleWrapper3Type>,
        "const TriangleWrapper3 does not satisfy the Triangle3Concept");
    static_assert(
        Triangle3Concept<TriangleWrapper3Type&>,
        "TriangleWrapper3& does not satisfy the Triangle3Concept");
    static_assert(
        Triangle3Concept<const TriangleWrapper3Type&>,
        "const TriangleWrapper3& does not satisfy the Triangle3Concept");
    static_assert(
        Triangle3Concept<TriangleWrapper3Type&&>,
        "TriangleWrapper3&& does not satisfy the Triangle3Concept");
}

#endif // TRIANGLE_H
