// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef POLYGON_H
#define POLYGON_H

#include <vclib/space.h>

void polygonStaticAsserts()
{
    using namespace vcl;

    using Polygon2Type = Polygon2f;
    using Polygon3Type = Polygon3d;

    // polygon2
    static_assert(
        Polygon2Concept<Polygon2Type>,
        "Polygon2 does not satisfy the Polygon2Concept");
    static_assert(
        Polygon2Concept<const Polygon2Type>,
        "const Polygon2 does not satisfy the Polygon2Concept");
    static_assert(
        Polygon2Concept<Polygon2Type&>,
        "Polygon2& does not satisfy the Polygon2Concept");
    static_assert(
        Polygon2Concept<const Polygon2Type&>,
        "const Polygon2& does not satisfy the Polygon2Concept");
    static_assert(
        Polygon2Concept<Polygon2Type&&>,
        "Polygon2&& does not satisfy the Polygon2Concept");

    // polygon3
    static_assert(
        Polygon3Concept<Polygon3Type>,
        "Polygon3 does not satisfy the Polygon3Concept");
    static_assert(
        Polygon3Concept<const Polygon3Type>,
        "const Polygon3 does not satisfy the Polygon3Concept");
    static_assert(
        Polygon3Concept<Polygon3Type&>,
        "Polygon3& does not satisfy the Polygon3Concept");
    static_assert(
        Polygon3Concept<const Polygon3Type&>,
        "const Polygon3& does not satisfy the Polygon3Concept");
    static_assert(
        Polygon3Concept<Polygon3Type&&>,
        "Polygon3&& does not satisfy the Polygon3Concept");
}

#endif // POLYGON_H
