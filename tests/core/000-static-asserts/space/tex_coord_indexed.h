// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEX_COORD_INDEXED_H
#define TEX_COORD_INDEXED_H

#include <vclib/space.h>

void texCoordIndexedStaticAsserts()
{
    using namespace vcl;

    // TexCoord concept
    static_assert(
        TexCoordIndexedConcept<TexCoordIndexedf>,
        "TexCoordIndexedf does not satisfy the TexCoordIndexedConcept");
    static_assert(
        TexCoordIndexedConcept<const TexCoordIndexedf>,
        "const TexCoordIndexedf does not satisfy the TexCoordIndexedConcept");
    static_assert(
        TexCoordIndexedConcept<TexCoordIndexedf&>,
        "TexCoordIndexedf& does not satisfy the TexCoordIndexedConcept");
    static_assert(
        TexCoordIndexedConcept<const TexCoordIndexedf&>,
        "const TexCoordIndexedf& does not satisfy the TexCoordIndexedConcept");
    static_assert(
        TexCoordIndexedConcept<TexCoordIndexedf&&>,
        "TexCoordIndexedf&& does not satisfy the TexCoordIndexedConcept");
}

#endif // TEX_COORD_INDEXED_H
