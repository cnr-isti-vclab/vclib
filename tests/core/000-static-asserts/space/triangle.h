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

    // triangle3
    static_assert(
        Triangle3Concept<Triangle3Type>,
        "Triangle3 does not satisfy the Triangle3Concept");

    // triangle wrapper2
    static_assert(
        Triangle2Concept<TriangleWrapper2Type>,
        "TriangleWrapper2 does not satisfy the Triangle2Concept");

    static_assert(
        Triangle3Concept<TriangleWrapper3Type>,
        "TriangleWrapper3 does not satisfy the Triangle3Concept");
}

#endif // TRIANGLE_H
