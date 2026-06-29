// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
