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
