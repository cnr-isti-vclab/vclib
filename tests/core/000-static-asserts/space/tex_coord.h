// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEX_COORD_H
#define TEX_COORD_H

#include <vclib/space.h>

void texCoordStaticAsserts()
{
    using namespace vcl;

    // TexCoord concept
    static_assert(
        TexCoordConcept<TexCoordf>,
        "TexCoordf does not satisfy the TexCoordConcept");
    static_assert(
        TexCoordConcept<const TexCoordf>,
        "const TexCoordf does not satisfy the TexCoordConcept");
    static_assert(
        TexCoordConcept<TexCoordf&>,
        "TexCoordf& does not satisfy the TexCoordConcept");
    static_assert(
        TexCoordConcept<const TexCoordf&>,
        "const TexCoordf& does not satisfy the TexCoordConcept");
    static_assert(
        TexCoordConcept<TexCoordf&&>,
        "TexCoordf&& does not satisfy the TexCoordConcept");

    static_assert(
        TexCoordConcept<TexCoordIndexedf>,
        "TexCoordIndexedf does not satisfy the TexCoordConcept");
    static_assert(
        TexCoordConcept<const TexCoordIndexedf>,
        "const TexCoordIndexedf does not satisfy the TexCoordConcept");
    static_assert(
        TexCoordConcept<TexCoordIndexedf&>,
        "TexCoordIndexedf& does not satisfy the TexCoordConcept");
    static_assert(
        TexCoordConcept<const TexCoordIndexedf&>,
        "const TexCoordIndexedf& does not satisfy the TexCoordConcept");
    static_assert(
        TexCoordConcept<TexCoordIndexedf&&>,
        "TexCoordIndexedf&& does not satisfy the TexCoordConcept");
}

#endif // TEX_COORD_H
