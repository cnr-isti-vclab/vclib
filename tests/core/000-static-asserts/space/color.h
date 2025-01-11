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

#ifndef COLOR_H
#define COLOR_H

#include <vclib/space.h>

void colorStaticAsserts()
{
    using namespace vcl;

    // color
    static_assert(
        ColorConcept<Color>, "Color does not satisfy the ColorConcept");
    static_assert(
        ColorConcept<Color&>, "Color& does not satisfy the ColorConcept");
    static_assert(
        ColorConcept<const Color>,
        "const Color does not satisfy the ColorConcept");
    static_assert(
        ColorConcept<const Color&>,
        "const Color& does not satisfy the ColorConcept");
    static_assert(
        ColorConcept<Color&&>, "Color&& does not satisfy the ColorConcept");

    static_assert(Serializable<Color>, "Color is not serializable");
}

#endif // COLOR_H
